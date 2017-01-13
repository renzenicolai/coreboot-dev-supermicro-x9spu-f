--
-- Copyright (C) 2015-2016 secunet Security Networks AG
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

with System.Storage_Elements;

with HW.Time;
with HW.MMIO_Range;
pragma Elaborate_All (HW.MMIO_Range);

with HW.Debug;
with GNAT.Source_Info;

use type System.Address;
use type HW.Word64;

package body HW.GFX.GMA.Registers
with
   Refined_State =>
     (Address_State  => (Regs.Base_Address, GTT.Base_Address),
      Register_State => Regs.State,
      GTT_State      => GTT.State)
is
   pragma Disable_Atomic_Synchronization;

   type Registers_Range is
      new Natural range 0 .. 16#0020_0000# / Register_Width - 1;
   type Registers_Type is array (Registers_Range) of Word32
   with
      Atomic_Components,
      Size => 16#20_0000# * 8;
   package Regs is new MMIO_Range
     (Base_Addr   => Config.Default_MMIO_Base,
      Element_T   => Word32,
      Index_T     => Registers_Range,
      Array_T     => Registers_Type);

   ----------------------------------------------------------------------------

   GTT_Offset  : constant := (case Config.CPU is
                                 when Ironlake .. Haswell   => 16#0020_0000#,
                                 when Broadwell .. Skylake  => 16#0080_0000#);

   GTT_Size    : constant := (case Config.CPU is
                                 when Ironlake .. Haswell   => 16#0020_0000#,
                                 -- Limit Broadwell to 4MiB to have a stable
                                 -- interface (i.e. same number of entries):
                                 when Broadwell .. Skylake  => 16#0040_0000#);

   GTT_PTE_Size   : constant := (case Config.CPU is
                                    when Ironlake .. Haswell   => 4,
                                    when Broadwell .. Skylake  => 8);


   type GTT_PTE_Type is mod 2 ** (GTT_PTE_Size * 8);
   type GTT_Registers_Type is array (GTT_Range) of GTT_PTE_Type
   with
      Volatile_Components,
      Size => GTT_Size * 8;
   package GTT is new MMIO_Range
     (Base_Addr   => Config.Default_MMIO_Base + GTT_Offset,
      Element_T   => GTT_PTE_Type,
      Index_T     => GTT_Range,
      Array_T     => GTT_Registers_Type);

   GTT_PTE_Valid : constant Word32 := 1;

   ----------------------------------------------------------------------------

   procedure Write_GTT
     (GTT_Page       : GTT_Range;
      Device_Address : GTT_Address_Type;
      Valid          : Boolean)
   is
   begin
      if Config.Fold_39Bit_GTT_PTE then
         GTT.Write
           (Index => GTT_Page,
            Value => GTT_PTE_Type (Device_Address and 16#ffff_f000#) or
                     GTT_PTE_Type (Shift_Right (Word64 (Device_Address), 32 - 4)
                                   and 16#0000_07f0#) or
                     Boolean'Pos (Valid));
      else
         GTT.Write
           (Index => GTT_Page,
            Value => GTT_PTE_Type (Device_Address and 16#7f_ffff_f000#) or
                     Boolean'Pos (Valid));
      end if;
   end Write_GTT;

   ----------------------------------------------------------------------------

   package Rep is
      function Index (Reg : Registers_Index) return Registers_Range;
   end Rep;

   package body Rep is
      function Index (Reg : Registers_Index) return Registers_Range
      with
         SPARK_Mode => Off
      is
      begin
         return Reg'Enum_Rep;
      end Index;
   end Rep;

   -- Read a specific register
   procedure Read
     (Register : in     Registers_Index;
      Value    :    out Word32;
      Verbose  : in     Boolean := True)
   is
   begin
      Regs.Read (Value, Rep.Index (Register));

      pragma Debug (Verbose, Debug.Put (GNAT.Source_Info.Enclosing_Entity & ":  "));
      pragma Debug (Verbose, Debug.Put_Word32 (Value));
      pragma Debug (Verbose, Debug.Put (" <- "));
      pragma Debug (Verbose, Debug.Put_Word32 (Register'Enum_Rep * Register_Width));
      pragma Debug (Verbose, Debug.Put (":"));
      pragma Debug (Verbose, Debug.Put_Line (Registers_Index'Image (Register)));
   end Read;

   ----------------------------------------------------------------------------

   -- Read a specific register to post a previous write
   procedure Posting_Read (Register : Registers_Index)
   is
      Discard_Value : Word32;
   begin
      pragma Warnings
        (Off, "unused assignment to ""Discard_Value""",
         Reason => "Intentional dummy read to affect hardware.");

      Read (Register, Discard_Value);

      pragma Warnings
        (On, "unused assignment to ""Discard_Value""");
   end Posting_Read;

   ----------------------------------------------------------------------------

   -- Write a specific register
   procedure Write (Register : Registers_Index; Value : Word32)
   is
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Word32 (Value));
      pragma Debug (Debug.Put (" -> "));
      pragma Debug (Debug.Put_Word32 (Register'Enum_Rep * Register_Width));
      pragma Debug (Debug.Put (":"));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Regs.Write (Rep.Index (Register), Value);
      pragma Debug (Debug.Register_Write_Wait);
   end Write;

   ----------------------------------------------------------------------------

   -- Check whether all bits in @Register@ indicated by @Mask@ are set
   procedure Is_Set_Mask
      (Register : in     Registers_Index;
       Mask     : in     Word32;
       Result   :    out Boolean)
   is
      Value : Word32;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Read (Register, Value);
      Result := (Value and Mask) = Mask;

   end Is_Set_Mask;

   ----------------------------------------------------------------------------

   -- TODO: Should have Success parameter
   -- Wait for all bits in @Register@ indicated by @Mask@ to be set
   procedure Wait_Set_Mask
     (Register : in     Registers_Index;
      Mask     : in     Word32;
      TOut_MS  : in     Natural := Default_Timeout_MS;
      Verbose  : in     Boolean := False)
   is
      Value : Word32;
      Timeout : Time.T;
      Timed_Out : Boolean;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Timeout := Time.MS_From_Now (TOut_MS);
      loop
         Timed_Out := Time.Timed_Out (Timeout);
         Read (Register, Value, Verbose);
         if (Value and Mask) = Mask then
            exit;
         end if;
         pragma Debug (Timed_Out, Debug.Put (GNAT.Source_Info.Enclosing_Entity));
         pragma Debug (Timed_Out, Debug.Put_Line (": Timed Out!"));
         exit when Timed_Out;
      end loop;

   end Wait_Set_Mask;

   ----------------------------------------------------------------------------

   -- TODO: Should have Success parameter
   -- Wait for bits in @Register@ indicated by @Mask@ to be clear
   procedure Wait_Unset_Mask
     (Register : Registers_Index;
      Mask     : Word32;
      TOut_MS  : in     Natural := Default_Timeout_MS;
      Verbose  : in     Boolean := False)
   is
      Value : Word32;
      Timeout : Time.T;
      Timed_Out : Boolean;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Timeout := Time.MS_From_Now (TOut_MS);
      loop
         Timed_Out := Time.Timed_Out (Timeout);
         Read (Register, Value, Verbose);
         if (Value and Mask) = 0 then
            exit;
         end if;
         pragma Debug (Timed_Out, Debug.Put (GNAT.Source_Info.Enclosing_Entity));
         pragma Debug (Timed_Out, Debug.Put_Line (": Timed Out!"));
         exit when Timed_Out;
      end loop;

   end Wait_Unset_Mask;

   ----------------------------------------------------------------------------

   -- Set bits from @Mask@ in @Register@
   procedure Set_Mask
      (Register : Registers_Index;
       Mask     : Word32)
   is
      Value : Word32;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Word32 (Mask));
      pragma Debug (Debug.Put (" .S "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Read (Register, Value);
      Value := Value or Mask;
      Write (Register, Value);
   end Set_Mask;

   ----------------------------------------------------------------------------

   -- Mask out @Mask@ in @Register@
   procedure Unset_Mask
      (Register : Registers_Index;
       Mask     : Word32)
   is
      Value : Word32;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Word32 (Mask));
      pragma Debug (Debug.Put (" !S "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Read (Register, Value);
      Value := Value and not Mask;
      Write (Register, Value);
   end Unset_Mask;

   ----------------------------------------------------------------------------

   -- Mask out @Unset_Mask@ and set @Set_Mask@ in @Register@
   procedure Unset_And_Set_Mask
      (Register   : Registers_Index;
       Mask_Unset : Word32;
       Mask_Set   : Word32)
   is
      Value : Word32;
   begin
      pragma Debug (Debug.Put (GNAT.Source_Info.Enclosing_Entity & ": "));
      pragma Debug (Debug.Put_Line (Registers_Index'Image (Register)));

      Read (Register, Value);
      Value := (Value and not Mask_Unset) or Mask_Set;
      Write (Register, Value);
   end Unset_And_Set_Mask;

   ----------------------------------------------------------------------------

   procedure Set_Register_Base (Base : Word64)
   is
   begin
      Regs.Set_Base_Address (Base);
      GTT.Set_Base_Address (Base + GTT_Offset);
   end Set_Register_Base;

end HW.GFX.GMA.Registers;
