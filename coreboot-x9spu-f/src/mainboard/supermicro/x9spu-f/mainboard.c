/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2011-2012 Google Inc.
 * Copyright (C) 2014 Vladimir Serbinenko
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

#include <types.h>
#include <string.h>
#include <device/device.h>
#include <device/pci_def.h>
#include <device/pci_ops.h>
#include <console/console.h>
#include <drivers/intel/gma/int15.h>
#include <pc80/mc146818rtc.h>
#include <arch/acpi.h>
#include <arch/io.h>
#include <arch/interrupt.h>
#include <boot/coreboot_tables.h>
#include <southbridge/intel/bd82x6x/pch.h>
#include <smbios.h>
#include <device/pci.h>
#include <cbfs.h>

static void mainboard_init(device_t dev)
{  
    post_code(0x43);
	RCBA32(0x38c8) = 0x00002005;
	RCBA32(0x38c4) = 0x00802005;
	RCBA32(0x38c0) = 0x00000007;
    
	/*RCBA32(0x2240) = 0x00330e71;
	RCBA32(0x2244) = 0x003f0eb1;
	RCBA32(0x2248) = 0x002102cd;
	RCBA32(0x224c) = 0x00f60000;
	RCBA32(0x2250) = 0x00020000;
	RCBA32(0x2254) = 0x00e3004c;
	RCBA32(0x2258) = 0x00e20bef;
	RCBA32(0x2260) = 0x003304ed;
	RCBA32(0x2278) = 0x001107c1;
	RCBA32(0x227c) = 0x001d07e9;
	RCBA32(0x2280) = 0x00e20000;
	RCBA32(0x2284) = 0x00ee0000;
	RCBA32(0x2288) = 0x005b05d3;
	RCBA32(0x2318) = 0x04b8ff2e;
	RCBA32(0x231c) = 0x03930f2e;*/
    
RCBA32(0x2230) = 0x00010000;
RCBA32(0x2234) = 0x0000000f;
RCBA32(0x2238) = 0x00000941;
RCBA32(0x2240) = 0x0033023e;
RCBA32(0x2244) = 0x003f027e;
RCBA32(0x2248) = 0x007b059d;
RCBA32(0x224c) = 0x000e0000;
RCBA32(0x2250) = 0x001a0000;
RCBA32(0x2254) = 0x00e60f72;
RCBA32(0x2258) = 0x00e50bf7;
RCBA32(0x2260) = 0x00520aeb;
RCBA32(0x2268) = 0x000c0028;
RCBA32(0x2270) = 0x000c0000;
RCBA32(0x2274) = 0x00100040;
RCBA32(0x2278) = 0x00f00231;
RCBA32(0x227c) = 0x00fc0259;
RCBA32(0x2280) = 0x00830000;
RCBA32(0x2284) = 0x008f0000;
RCBA32(0x2288) = 0x00e80220;
RCBA32(0x228c) = 0x00000001;
RCBA32(0x2294) = 0x00120c48;
RCBA32(0x2298) = 0x001e0c70;
RCBA32(0x229c) = 0x00440000;
RCBA32(0x22a0) = 0x004f0000;
RCBA32(0x22a4) = 0x00540d50;
RCBA32(0x2304) = 0xc03b8400;
RCBA32(0x2308) = 0x28000016;
RCBA32(0x230c) = 0x4abcb5bc;
RCBA32(0x2310) = 0xa889605b;
RCBA32(0x2314) = 0x0a2c0020;
RCBA32(0x2318) = 0x04b8f023;
RCBA32(0x231c) = 0x08e40023;
RCBA32(0x2320) = 0x00008002;
RCBA32(0x2324) = 0x00854c74;
RCBA32(0x2328) = 0x00421731;
RCBA32(0x2330) = 0xea004000;
RCBA32(0x2334) = 0x00000084;
RCBA32(0x2338) = 0x00000600;
RCBA32(0x2340) = 0x003a001b;
RCBA32(0x2344) = 0xff0c000c;
    
    
    //SPIBAR
RCBA32(0x3808) = 0x00426d4d;
RCBA32(0x3810) = 0x00520000;

RCBA32(0x3814) = 0x21000000;
RCBA32(0x3818) = 0x03000002;
RCBA32(0x381c) = 0xf8000000;
RCBA32(0x3820) = 0x04000009;
RCBA32(0x3824) = 0x2a000000;
RCBA32(0x3828) = 0x0e0000e2;
RCBA32(0x3830) = 0x21000000;
RCBA32(0x3834) = 0x03000002;
RCBA32(0x3838) = 0xf8000000;
RCBA32(0x383c) = 0x04000009;
RCBA32(0x3840) = 0x28000000;
RCBA32(0x3844) = 0x0e0000e2;   
    post_code(0x44);
}

// mainboard_enable is executed as first thing after
// enumerate_buses().

static void mainboard_enable(device_t dev)
{
	dev->ops->init = mainboard_init;

	//install_intel_vga_int15_handler(GMA_INT15_ACTIVE_LFP_NONE, GMA_INT15_PANEL_FIT_DEFAULT, GMA_INT15_BOOT_DISPLAY_CRT, 0);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
