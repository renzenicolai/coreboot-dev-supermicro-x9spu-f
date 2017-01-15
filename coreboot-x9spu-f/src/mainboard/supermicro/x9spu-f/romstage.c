/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2017 Renze Nicolai <renze@rnplus.nl>
 * Copyright (C) 2014 Damien Zammit <damien@zamaudio.com>
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

#define SUPERIO_BASE 0x2e
#define SUPERIO_DEV PNP_DEV(SUPERIO_BASE, 0)
#define SUPERIO_GPIO6789 PNP_DEV(SUPERIO_BASE, NCT6776_GPIO6789)
#define SUPERIO_WDT1_GPIO01A PNP_DEV(SUPERIO_BASE, NCT6776_WDT1_GPIO01A)
#define SUPERIO_GPIO1234567 PNP_DEV(SUPERIO_BASE, NCT6776_GPIO1234567)
#define SERIAL_DEV PNP_DEV(SUPERIO_BASE, NCT6776_SP1)

#define WPCM450_BASE 0x164e

#include <arch/io.h>
#include <device/pci_def.h>
#include <device/pnp_def.h>
#include <cpu/x86/lapic.h>
#include <arch/acpi.h>
#include <console/console.h>
#include <superio/nuvoton/wpcm450/wpcm450.h>
#include <superio/nuvoton/nct6776/nct6776.h>
#include <superio/nuvoton/common/nuvoton.h>
#include <northbridge/intel/sandybridge/sandybridge.h>
#include <northbridge/intel/sandybridge/raminit_native.h>
#include <southbridge/intel/bd82x6x/pch.h>
#include <southbridge/intel/common/gpio.h>
#include <arch/cpu.h>
#include <cpu/x86/msr.h>
#include <delay.h>

void rcba_config(void)
{
	/* Disable unused devices (board specific) */
    RCBA32(FD) = 0x143E1FF1;
    
    //OLD 0000 0000 0001 0111 1110 1110 0001 1111 1110 0001: 0x17ee1fe1
    //NEW 0000 0000 0001 0100 0011 1110 0001 1111 1111 0001: 0x143E1FF1
    
    

	/* Enable HECI */
	//RCBA32(FD2) &= ~0x2;
    
    post_code(0x42);
}

void pch_enable_lpc(void)
{
	pci_write_config16(PCH_LPC_DEV, LPC_EN, KBC_LPC_EN | MC_LPC_EN |
			CNF1_LPC_EN | COMA_LPC_EN | COMB_LPC_EN);

	pci_write_config32(PCH_LPC_DEV, LPC_GEN1_DEC, 0x3c0a01);
	pci_write_config16(PCH_LPC_DEV, LPC_IO_DEC, 0x10);
	pci_write_config32(PCH_LPC_DEV, 0xac, 0x10000);

	/* Initialize SuperIO */
	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
    //wpcm450_enable_dev(WPCM450_SP1, WPCM450_BASE, CONFIG_TTYS0_BASE);
    console_init();
}

const struct southbridge_usb_port mainboard_usb_ports[] = {
	{ 1, 5, 0 },
	{ 1, 5, 0 },
	{ 1, 5, 1 },
	{ 1, 5, 1 },
	{ 1, 5, 2 },
	{ 1, 5, 2 },
	{ 1, 5, 3 },
	{ 1, 5, 3 },
	{ 1, 5, 4 },
	{ 1, 5, 4 },
	{ 1, 5, 6 },
	{ 1, 5, 5 },
	{ 1, 5, 5 },
	{ 1, 5, 6 },
};

void mainboard_get_spd(spd_raw_data *spd, bool id_only) {
	read_spd (&spd[0], 0x50, id_only);
	read_spd (&spd[1], 0x51, id_only);
	read_spd (&spd[2], 0x52, id_only);
	read_spd (&spd[3], 0x53, id_only);
}

void mainboard_early_init(int s3resume) {
    post_code(0x99);
    printk(BIOS_DEBUG, "mainboard_early_init (%d).\n", s3resume);
}

void mainboard_config_superio(void)
{
}
