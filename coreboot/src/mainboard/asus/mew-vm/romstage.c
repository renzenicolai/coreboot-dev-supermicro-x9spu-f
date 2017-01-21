/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007 Corey Osgood <corey.osgood@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdint.h>
#include <device/pci_def.h>
#include <arch/io.h>
#include <device/pnp_def.h>
#include <stdlib.h>
#include <console/console.h>
#include <superio/smsc/lpc47b272/lpc47b272.h>
#include <northbridge/intel/i82810/raminit.h>
#include <cpu/x86/bist.h>
#include <cpu/intel/romstage.h>
#include <southbridge/intel/i82801ax/i82801ax.h>
#include <lib.h>

#define SERIAL_DEV PNP_DEV(0x2e, LPC47B272_SP1)

void mainboard_romstage_entry(unsigned long bist)
{
	lpc47b272_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
	console_init();
	enable_smbus();
	report_bist_failure(bist);
	dump_spd_registers();
	sdram_set_registers();
	sdram_set_spd_registers();
	sdram_enable();
}
