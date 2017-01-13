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

/*static void it8728f_b75md3v_disable_reboot(pnp_devfn_t dev)
{*/
	/* GPIO SIO settings */
	/*ite_reg_write(dev, 0xEF, 0x7E); // magic

	ite_reg_write(dev, 0x25, 0x40); // gpio pin function -> gp16
	ite_reg_write(dev, 0x27, 0x10); // gpio pin function -> gp34
	ite_reg_write(dev, 0x2c, 0x80); // smbus isolation on parallel port
	ite_reg_write(dev, 0x62, 0x0a); // simple iobase 0xa00
	ite_reg_write(dev, 0x72, 0x20); // watchdog timeout clear!
	ite_reg_write(dev, 0x73, 0x00); // watchdog timeout clear!
	ite_reg_write(dev, 0xcb, 0x00); // simple io set4 direction -> in
	ite_reg_write(dev, 0xe9, 0x27); // bus select disable
	ite_reg_write(dev, 0xf0, 0x10); // ?
	ite_reg_write(dev, 0xf1, 0x42); // ?
	ite_reg_write(dev, 0xf6, 0x1c); // hardware monitor alert beep -> gp36(pin12)*/

	/* EC SIO settings */
	/*ite_reg_write(IT8728F_EC, 0xf1, 0xc0);
	ite_reg_write(IT8728F_EC, 0xf6, 0xf0);
	ite_reg_write(IT8728F_EC, 0xf9, 0x48);
	ite_reg_write(IT8728F_EC, 0x60, 0x0a);
	ite_reg_write(IT8728F_EC, 0x61, 0x30);
	ite_reg_write(IT8728F_EC, 0x62, 0x0a);
	ite_reg_write(IT8728F_EC, 0x63, 0x20);
	ite_reg_write(IT8728F_EC, 0x30, 0x01);*/
//}

void rcba_config(void)
{
	/* Disable unused devices (board specific) */
	//RCBA32(FD) = 0x17ee1fe1;

	/* Enable HECI */
	RCBA32(FD2) &= ~0x2;
    post_code(0x42);
}

void pch_enable_lpc(void)
{
    post_code(0x11);
	/*
	 * Enable:
	 *  EC Decode Range PortA30/A20
	 *  SuperIO Port2E/2F
	 *  PS/2 Keyboard/Mouse Port60/64
	 *  FDD Port3F0h-3F5h and Port3F7h
	 */
	pci_write_config16(PCH_LPC_DEV, LPC_EN, KBC_LPC_EN | MC_LPC_EN |
			CNF1_LPC_EN | CNF2_LPC_EN | COMA_LPC_EN | COMB_LPC_EN);

	pci_write_config32(PCH_LPC_DEV, LPC_GEN1_DEC, 0x3c0a01);
	pci_write_config16(PCH_LPC_DEV, LPC_IO_DEC, 0x10);
	pci_write_config32(PCH_LPC_DEV, 0xac, 0x10000);

	/* Initialize SuperIO */
    wpcm450_enable_dev(WPCM450_SP1, WPCM450_BASE, CONFIG_TTYS0_BASE);
	//nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
	//it8728f_b75md3v_disable_reboot(SUPERIO_GPIO);
    post_code(0x12);
    console_init();
    post_code(0x13);
    printk(BIOS_DEBUG, "Hello world 1!\n");
    delay(5);
    printk(BIOS_DEBUG, "Hello world 2!\n");
    post_code(0x14);
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
    printk(BIOS_DEBUG, "Hello world 3!\n");
    delay(5);
}

void mainboard_config_superio(void)
{
}
