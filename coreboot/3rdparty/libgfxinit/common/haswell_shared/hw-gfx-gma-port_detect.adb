--
-- Copyright (C) 2016 secunet Security Networks AG
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--

with HW.GFX.GMA.Config;
with HW.GFX.GMA.Registers;

package body HW.GFX.GMA.Port_Detect
is

   PCH_ADPA_CRT_HPD_CHANNEL_MASK       : constant := 3 * 2 ** 24;
   PCH_ADPA_CRT_HPD_ENABLE             : constant := 1 * 2 ** 23;

   SFUSE_STRAP_CRT_DAC_CAP_DISABLE     : constant := 1 * 2 **  6;

   HOTPLUG_CTL_DDI_A_HPD_INPUT_ENABLE  : constant := 1 * 2 **  4;
   HOTPLUG_CTL_DDI_A_HPD_STATUS        : constant := 3 * 2 **  0;
   HOTPLUG_CTL_DDI_A_HPD_LONG_DETECT   : constant := 1 * 2 **  1;

   SHOTPLUG_CTL_DETECT_MASK            : constant := 16#0303_0303#;

   type Digital_Port_Value is array (Digital_Port) of Word32;
   DDI_PORT_DETECTED : constant Digital_Port_Value :=
     (DIGI_B => 1 * 2 **  2,
      DIGI_C => 1 * 2 **  1,
      DIGI_D => 1 * 2 **  0,
      DIGI_A => 1 * 2 **  0,
      others => 0);
   SHOTPLUG_CTL_HPD_INPUT_ENABLE : constant Digital_Port_Value :=
     (DIGI_B => 1 * 2 **  4,
      DIGI_C => 1 * 2 ** 12,
      DIGI_D => 1 * 2 ** 20,
      DIGI_A => 1 * 2 ** 28,
      others => 0);
   SHOTPLUG_CTL_HPD_STATUS : constant Digital_Port_Value :=
     (DIGI_B => 3 * 2 **  0,
      DIGI_C => 3 * 2 **  8,
      DIGI_D => 3 * 2 ** 16,
      DIGI_A => 3 * 2 ** 24,
      others => 0);
   SHOTPLUG_CTL_LONG_DETECT : constant Digital_Port_Value :=
     (DIGI_B => 1 * 2 **  1,
      DIGI_C => 1 * 2 **  9,
      DIGI_D => 1 * 2 ** 17,
      DIGI_A => 1 * 2 ** 25,
      others => 0);

   procedure Initialize
   is
      DAC_Disabled,
      Internal_Detected,
      DDI_Detected : Boolean;

      Last_Digital_Port : constant Digital_Port :=
        (if Config.Has_DDI_D then DIGI_D else DIGI_C);

      subtype Ext_Digital_Port is
         Digital_Port range DIGI_B .. DIGI_D;
      type Digital_Port_To_GMA_Port is array (Ext_Digital_Port) of Port_Type;
      To_HDMI_Port : constant Digital_Port_To_GMA_Port :=
        (DIGI_B => Digital1,
         DIGI_C => Digital2,
         DIGI_D => Digital3);
      To_DP_Port : constant Digital_Port_To_GMA_Port :=
        (DIGI_B => DP1,
         DIGI_C => DP2,
         DIGI_D => DP3);
   begin
      if Config.Has_PCH_DAC then
         -- PCH_DAC (_A)
         Registers.Is_Set_Mask
           (Register => Registers.SFUSE_STRAP,
            Mask     => SFUSE_STRAP_CRT_DAC_CAP_DISABLE,
            Result   => DAC_Disabled);
         if not DAC_Disabled then
            Registers.Set_Mask
              (Register => Registers.PCH_ADPA,
               Mask     => PCH_ADPA_CRT_HPD_CHANNEL_MASK or   -- clear status
                           PCH_ADPA_CRT_HPD_ENABLE);
         end if;
         Config.Valid_Port (Analog) := not DAC_Disabled;
      end if;

      if Config.Internal_Is_EDP then
         -- DDI_A
         Registers.Is_Set_Mask
           (Register => Registers.DDI_BUF_CTL_A,
            Mask     => DDI_PORT_DETECTED (DIGI_A),
            Result   => Internal_Detected);
         if Internal_Detected then
            if Config.Has_HOTPLUG_CTL then
               Registers.Set_Mask
                 (Register => Registers.HOTPLUG_CTL,
                  Mask     => HOTPLUG_CTL_DDI_A_HPD_INPUT_ENABLE or
                              HOTPLUG_CTL_DDI_A_HPD_STATUS);   -- clear status
               if Config.Has_SHOTPLUG_CTL_A then
                  -- Have to enable south hotplug too on SoCs.
                  Registers.Unset_And_Set_Mask
                    (Register    => Registers.SHOTPLUG_CTL,
                     Mask_Unset  => SHOTPLUG_CTL_DETECT_MASK,
                     Mask_Set    => SHOTPLUG_CTL_HPD_INPUT_ENABLE (DIGI_A));
               end if;
            else
               Registers.Unset_And_Set_Mask
                 (Register    => Registers.SHOTPLUG_CTL,
                  Mask_Unset  => SHOTPLUG_CTL_DETECT_MASK,
                  Mask_Set    => SHOTPLUG_CTL_HPD_INPUT_ENABLE (DIGI_A) or
                                 SHOTPLUG_CTL_HPD_STATUS (DIGI_A));  -- clear
            end if;
         end if;
      else
         Internal_Detected := False;
      end if;
      Config.Valid_Port (Internal) := Internal_Detected;

      -- DDI_[BCD]
      for Port in Ext_Digital_Port range DIGI_B .. Last_Digital_Port loop
         Registers.Is_Set_Mask
           (Register => Registers.SFUSE_STRAP,
            Mask     => DDI_PORT_DETECTED (Port),
            Result   => DDI_Detected);
         Config.Valid_Port (To_HDMI_Port (Port)) :=
            Config.Valid_Port (To_HDMI_Port (Port)) and DDI_Detected;
         Config.Valid_Port (To_DP_Port (Port)) :=
            Config.Valid_Port (To_DP_Port (Port)) and DDI_Detected;

         if DDI_Detected then
            Registers.Unset_And_Set_Mask
              (Register    => Registers.SHOTPLUG_CTL,
               Mask_Unset  => SHOTPLUG_CTL_DETECT_MASK,
               Mask_Set    => SHOTPLUG_CTL_HPD_INPUT_ENABLE (Port) or
                              SHOTPLUG_CTL_HPD_STATUS (Port)); -- clear status
         else
            Registers.Unset_Mask
              (Register => Registers.SHOTPLUG_CTL,
               Mask     => SHOTPLUG_CTL_DETECT_MASK or
                           SHOTPLUG_CTL_HPD_INPUT_ENABLE (Port));
         end if;
      end loop;
   end Initialize;

   procedure Hotplug_Detect (Port_Cfg : in Port_Config; Detected : out Boolean)
   is
      Ctl32 : Word32;
   begin
      if Port_Cfg.Display = VGA then
         Registers.Read (Registers.PCH_ADPA, Ctl32, Verbose => False);
         Ctl32 := Ctl32 and PCH_ADPA_CRT_HPD_CHANNEL_MASK;
         Detected := Ctl32 = PCH_ADPA_CRT_HPD_CHANNEL_MASK;
         if Ctl32 /= 0 then
            Registers.Set_Mask
              (Register => Registers.PCH_ADPA,
               Mask     => PCH_ADPA_CRT_HPD_CHANNEL_MASK);
         end if;
      elsif Config.Has_HOTPLUG_CTL and then Port_Cfg.Port = DIGI_A then
         Registers.Read (Registers.HOTPLUG_CTL, Ctl32, Verbose => False);
         Detected := (Ctl32 and HOTPLUG_CTL_DDI_A_HPD_LONG_DETECT) /= 0;

         if (Ctl32 and HOTPLUG_CTL_DDI_A_HPD_STATUS) /= 0 then
            Registers.Set_Mask
              (Register => Registers.HOTPLUG_CTL,
               Mask     => HOTPLUG_CTL_DDI_A_HPD_STATUS);
         end if;
      elsif Port_Cfg.Port in DIGI_A .. DIGI_D then
         Registers.Read (Registers.SHOTPLUG_CTL, Ctl32, Verbose => False);
         Detected :=
           (Ctl32 and SHOTPLUG_CTL_LONG_DETECT (Port_Cfg.Port)) /= 0;

         if (Ctl32 and SHOTPLUG_CTL_HPD_STATUS (Port_Cfg.Port)) /= 0 then
            Registers.Unset_And_Set_Mask
              (Register    => Registers.SHOTPLUG_CTL,
               Mask_Unset  => SHOTPLUG_CTL_DETECT_MASK,
               Mask_Set    => SHOTPLUG_CTL_HPD_STATUS (Port_Cfg.Port));
         end if;
      else
         Detected := False;
      end if;
   end Hotplug_Detect;

end HW.GFX.GMA.Port_Detect;
