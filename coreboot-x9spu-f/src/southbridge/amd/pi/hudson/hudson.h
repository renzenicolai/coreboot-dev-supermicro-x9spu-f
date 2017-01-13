/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2010 Advanced Micro Devices, Inc.
 * Copyright (C) 2014 Sage Electronic Engineering, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef HUDSON_H
#define HUDSON_H

#include <device/pci_ids.h>
#include <device/device.h>
#include "chip.h"

/* Power management index/data registers */
#define BIOSRAM_INDEX	0xcd4
#define BIOSRAM_DATA	0xcd5
#define PM_INDEX	0xcd6
#define PM_DATA		0xcd7
#define PM2_INDEX	0xcd0
#define PM2_DATA	0xcd1

#define HUDSON_ACPI_IO_BASE 0x800

#define ACPI_PM_EVT_BLK		(HUDSON_ACPI_IO_BASE + 0x00) /* 4 bytes */
#define ACPI_PM1_CNT_BLK	(HUDSON_ACPI_IO_BASE + 0x04) /* 2 bytes */
#define ACPI_PM_TMR_BLK		(HUDSON_ACPI_IO_BASE + 0x18) /* 4 bytes */
#define ACPI_GPE0_BLK		(HUDSON_ACPI_IO_BASE + 0x10) /* 8 bytes */
#define ACPI_CPU_CONTROL	(HUDSON_ACPI_IO_BASE + 0x08) /* 6 bytes */

#define ACPI_SMI_CTL_PORT		0xb2
#define ACPI_SMI_CMD_CST_CONTROL	0xde
#define ACPI_SMI_CMD_PST_CONTROL	0xad
#define ACPI_SMI_CMD_DISABLE		0xbe
#define ACPI_SMI_CMD_ENABLE		0xef
#define ACPI_SMI_CMD_S4_REQ		0xc0

#define REV_HUDSON_A11	0x11
#define REV_HUDSON_A12	0x12

#define SPIROM_BASE_ADDRESS_REGISTER  0xA0
#define SPI_ROM_ENABLE                0x02
#define SPI_BASE_ADDRESS              0xFEC10000

#define LPC_IO_PORT_DECODE_ENABLE     0x44
#define DECODE_ENABLE_PARALLEL_PORT0  (1 << 0)
#define DECODE_ENABLE_PARALLEL_PORT1  (1 << 1)
#define DECODE_ENABLE_PARALLEL_PORT2  (1 << 2)
#define DECODE_ENABLE_PARALLEL_PORT3  (1 << 3)
#define DECODE_ENABLE_PARALLEL_PORT4  (1 << 4)
#define DECODE_ENABLE_PARALLEL_PORT5  (1 << 5)
#define DECODE_ENABLE_SERIAL_PORT0    (1 << 6)
#define DECODE_ENABLE_SERIAL_PORT1    (1 << 7)
#define DECODE_ENABLE_SERIAL_PORT2    (1 << 8)
#define DECODE_ENABLE_SERIAL_PORT3    (1 << 9)
#define DECODE_ENABLE_SERIAL_PORT4    (1 << 10)
#define DECODE_ENABLE_SERIAL_PORT5    (1 << 11)
#define DECODE_ENABLE_SERIAL_PORT6    (1 << 12)
#define DECODE_ENABLE_SERIAL_PORT7    (1 << 13)
#define DECODE_ENABLE_AUDIO_PORT0     (1 << 14)
#define DECODE_ENABLE_AUDIO_PORT1     (1 << 15)
#define DECODE_ENABLE_AUDIO_PORT2     (1 << 16)
#define DECODE_ENABLE_AUDIO_PORT3     (1 << 17)
#define DECODE_ENABLE_MIDI_PORT0      (1 << 18)
#define DECODE_ENABLE_MIDI_PORT1      (1 << 19)
#define DECODE_ENABLE_MIDI_PORT2      (1 << 20)
#define DECODE_ENABLE_MIDI_PORT3      (1 << 21)
#define DECODE_ENABLE_MSS_PORT0       (1 << 22)
#define DECODE_ENABLE_MSS_PORT1       (1 << 23)
#define DECODE_ENABLE_MSS_PORT2       (1 << 24)
#define DECODE_ENABLE_MSS_PORT3       (1 << 25)
#define DECODE_ENABLE_FDC_PORT0       (1 << 26)
#define DECODE_ENABLE_FDC_PORT1       (1 << 27)
#define DECODE_ENABLE_GAME_PORT       (1 << 28)
#define DECODE_ENABLE_KBC_PORT        (1 << 29)
#define DECODE_ENABLE_ACPIUC_PORT     (1 << 30)
#define DECODE_ENABLE_ADLIB_PORT      (1 << 31)

static inline int hudson_sata_enable(void)
{
	/* True if IDE or AHCI. */
	return (CONFIG_HUDSON_SATA_MODE == 0) || (CONFIG_HUDSON_SATA_MODE == 2);
}

static inline int hudson_ide_enable(void)
{
	/* True if IDE or LEGACY IDE. */
	return (CONFIG_HUDSON_SATA_MODE == 0) || (CONFIG_HUDSON_SATA_MODE == 3);
}

#ifndef __SMM__

void pm_write8(u8 reg, u8 value);
u8 pm_read8(u8 reg);
void pm_write16(u8 reg, u16 value);
u16 pm_read16(u16 reg);

#ifdef __PRE_RAM__
void hudson_lpc_port80(void);
void hudson_lpc_decode(void);
void hudson_pci_port80(void);
void hudson_clk_output_48Mhz(void);

int s3_save_nvram_early(u32 dword, int size, int  nvram_pos);
int s3_load_nvram_early(int size, u32 *old_dword, int nvram_pos);
#if IS_ENABLED(CONFIG_HUDSON_UART)
void configure_hudson_uart(void);
#endif

#else
void hudson_enable(device_t dev);
void s3_resume_init_data(void *FchParams);

#endif /* __PRE_RAM__ */
#endif /* __SMM__ */

#endif /* HUDSON_H */
