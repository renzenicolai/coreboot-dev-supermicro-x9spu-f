/**
 * @file
 *
 * AMD Family_10 Microcode patch.
 *
 * Fam10 Microcode Patch rev 010000B6 for 1043 or equivalent.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/FAMILY/0x10/REVC
 * @e \$Revision: 44323 $   @e \$Date: 2010-12-22 01:24:58 -0700 (Wed, 22 Dec 2010) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright (c) 2011, Advanced Micro Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Advanced Micro Devices, Inc. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "cpuRegisters.h"
#include "cpuEarlyInit.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

// Patch code 010000b6 for 1043 and equivalent
CONST MICROCODE_PATCHES ROMDATA CpuF10MicrocodePatch010000b6 =
{
0x09,
0x20,
0x31,
0x07,
0xb6,
0x00,
0x00,
0x01,
0x00,
0x80,
0x20,
0x00,
0xe9,
0x98,
0xda,
0x6b,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x43,
0x10,
0x00,
0x00,
0x00,
0xaa,
0xaa,
0xaa,
0x10,
0x0c,
0x00,
0x00,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0xff,
0x18,
0x80,
0x38,
0xc0,
0x83,
0x37,
0x80,
0xff,
0xb8,
0xff,
0xff,
0x13,
0x0e,
0xbf,
0x0c,
0xb6,
0x7a,
0xc4,
0xff,
0x2f,
0x3c,
0xfc,
0x6b,
0xfd,
0x40,
0x03,
0xd4,
0x00,
0x97,
0xff,
0xff,
0xff,
0xe7,
0xe1,
0x1f,
0xe0,
0x00,
0xfe,
0xbf,
0xf5,
0x9f,
0x87,
0x7e,
0x22,
0x01,
0xc6,
0x00,
0xc4,
0x7c,
0x1e,
0xfa,
0x01,
0x00,
0xe0,
0xff,
0x7b,
0x1f,
0xc0,
0x7f,
0xe0,
0xe0,
0xdf,
0xf0,
0x0f,
0x7f,
0x00,
0xff,
0x81,
0x80,
0x7f,
0xc3,
0x3f,
0xfe,
0x01,
0xfc,
0x07,
0x00,
0xfe,
0x0d,
0xff,
0x3d,
0x00,
0xf0,
0xff,
0xf0,
0x0f,
0xe0,
0x3f,
0x07,
0xf0,
0x6f,
0xf8,
0xc0,
0x3f,
0x80,
0xff,
0x1f,
0xc0,
0xbf,
0xe1,
0x03,
0xff,
0x00,
0xfe,
0x7f,
0x00,
0xff,
0x86,
0xff,
0x1e,
0x00,
0xf8,
0x1f,
0xf8,
0x07,
0xf0,
0xfc,
0x03,
0xf8,
0x37,
0x7f,
0xe0,
0x1f,
0xc0,
0xf0,
0x0f,
0xe0,
0xdf,
0xff,
0x81,
0x7f,
0x00,
0xc3,
0x3f,
0x80,
0x7f,
0xfc,
0x7f,
0x0f,
0x00,
0xf8,
0x0f,
0xfc,
0x03,
0x1b,
0xfe,
0x01,
0xfc,
0xe0,
0x3f,
0xf0,
0x0f,
0x6f,
0xf8,
0x07,
0xf0,
0x80,
0xff,
0xc0,
0x3f,
0xbf,
0xe1,
0x1f,
0xc0,
0x00,
0xfe,
0xbf,
0x07,
0x01,
0xfc,
0x07,
0xfe,
0xfe,
0x0d,
0xff,
0x00,
0x07,
0xf0,
0x1f,
0xf8,
0xf8,
0x37,
0xfc,
0x03,
0x1f,
0xc0,
0x7f,
0xe0,
0xe0,
0xdf,
0xf0,
0x0f,
0x03,
0x00,
0xff,
0xdf,
0xff,
0x00,
0xfe,
0x03,
0x00,
0xff,
0x86,
0x7f,
0xfc,
0x03,
0xf8,
0x0f,
0x01,
0xfc,
0x1b,
0xfe,
0xf0,
0x0f,
0xe0,
0x3f,
0x07,
0xf0,
0x6f,
0xf8,
0xef,
0x01,
0x80,
0xff,
0x81,
0x7f,
0x00,
0xff,
0x3f,
0x80,
0x7f,
0xc3,
0x07,
0xfe,
0x01,
0xfc,
0xff,
0x00,
0xfe,
0x0d,
0x1f,
0xf8,
0x07,
0xf0,
0xfc,
0x03,
0xf8,
0x37,
0xff,
0xf7,
0x00,
0xc0,
0xff,
0xc0,
0x3f,
0x80,
0xe1,
0x1f,
0xc0,
0xbf,
0xfe,
0x03,
0xff,
0x00,
0x86,
0x7f,
0x00,
0xff,
0xf8,
0x0f,
0xfc,
0x03,
0x1b,
0xfe,
0x01,
0xfc,
0xe0,
0xff,
0x7b,
0x00,
0xc0,
0x7f,
0xe0,
0x1f,
0xdf,
0xf0,
0x0f,
0xe0,
0x00,
0xff,
0x81,
0x7f,
0x7f,
0xc3,
0x3f,
0x80,
0x01,
0xfc,
0x07,
0xfe,
0xfe,
0x0d,
0xff,
0x00,
0x00,
0xf0,
0xff,
0x3d,
0x0f,
0xe0,
0x3f,
0xf0,
0xf0,
0x6f,
0xf8,
0x07,
0x3f,
0x80,
0xff,
0xc0,
0xc0,
0xbf,
0xe1,
0x1f,
0xff,
0x00,
0xfe,
0x03,
0x00,
0xff,
0x86,
0x7f,
0x1e,
0x00,
0xf8,
0xff,
0xf8,
0x07,
0xf0,
0x1f,
0x03,
0xf8,
0x37,
0xfc,
0xe0,
0x1f,
0xc0,
0x7f,
0x0f,
0xe0,
0xdf,
0xf0,
0x81,
0x7f,
0x00,
0xff,
0x3f,
0x80,
0x7f,
0xc3,
0x7f,
0x0f,
0x00,
0xfc,
0x0f,
0xfc,
0x03,
0xf8,
0xfe,
0x01,
0xfc,
0x1b,
0x3f,
0xf0,
0x0f,
0xe0,
0xf8,
0x07,
0xf0,
0x6f,
0xff,
0xc0,
0x3f,
0x80,
0xe1,
0x1f,
0xc0,
0xbf,
0xfe,
0xbf,
0x07,
0x00,
0xfc,
0x07,
0xfe,
0x01,
0x0d,
0xff,
0x00,
0xfe,
0xf0,
0x1f,
0xf8,
0x07,
0x37,
0xfc,
0x03,
0xf8,
0xc0,
0x7f,
0xe0,
0x1f,
0xdf,
0xf0,
0x0f,
0xe0,
0x00,
0xff,
0xdf,
0x03,
0x00,
0xfe,
0x03,
0xff,
0xff,
0x86,
0x7f,
0x00,
0x03,
0xf8,
0x0f,
0xfc,
0xfc,
0x1b,
0xfe,
0x01,
0x0f,
0xe0,
0x3f,
0xf0,
0xf0,
0x6f,
0xf8,
0x07,
0x01,
0x80,
0xff,
0xef,
0x7f,
0x00,
0xff,
0x81,
0x80,
0x7f,
0xc3,
0x3f,
0xfe,
0x01,
0xfc,
0x07,
0x00,
0xfe,
0x0d,
0xff,
0xf8,
0x07,
0xf0,
0x1f,
0x03,
0xf8,
0x37,
0xfc,
0xd7,
0x00,
0x80,
0xf8,
0xc0,
0x3f,
0x80,
0xff,
0x1f,
0xc0,
0xbf,
0xe1,
0x03,
0xff,
0x00,
0xfe,
0x7f,
0x00,
0xff,
0x86,
0x0f,
0xfc,
0x03,
0xf8,
0xfe,
0x01,
0xfc,
0x1b,
0xfc,
0x6b,
0x00,
0x20,
0x04,
0xff,
0xbf,
0xe8,
0xf0,
0xaf,
0xf5,
0xf3,
0xff,
0xd9,
0x7a,
0x00,
0x83,
0x3f,
0x31,
0xc0,
0x0c,
0x7d,
0xe3,
0x00,
0x0f,
0xfe,
0x80,
0x5e,
0xf0,
0xff,
0x3d,
0x00,
0x65,
0xfe,
0xff,
0x9f,
0x7f,
0xf8,
0xc7,
0xba,
0x96,
0xf2,
0xff,
0x7f,
0xfa,
0xe1,
0x1f,
0xeb,
0x45,
0x0e,
0xf8,
0xff,
0x9f,
0x87,
0x7f,
0x80,
0x00,
0xf8,
0xff,
0x1e,
0x07,
0xf0,
0x5f,
0x8c,
0x7b,
0x1d,
0xf8,
0x13,
0xbf,
0xe8,
0x1a,
0xff,
0xf4,
0xf3,
0xf0,
0x4f,
0xff,
0x2f,
0xe3,
0xff,
0xd7,
0xf5,
0xc3,
0xbf,
0x0f,
0x00,
0xfc,
0x7f,
0xd6,
0x03,
0xf8,
0xdf,
0x89,
0x01,
0x1e,
0xfc,
0xfb,
0x0f,
0xe0,
0x3f,
0xd6,
0xa2,
0x7f,
0xf8,
0xff,
0x7f,
0x82,
0xff,
0x97,
0xc1,
0xd6,
0xe1,
0x40,
0x02,
0x00,
0x14,
0x7f,
0xff,
0x01,
0xfc,
0xdf,
0x5a,
0xf4,
0x0f,
0xfe,
0xff,
0xef,
0x32,
0xfc,
0x03,
0x9c,
0x35,
0x7f,
0xf7,
0x5f,
0xcb,
0xf0,
0xaf,
0xf5,
0xff,
0xff,
0xdf,
0x03,
0x00,
0xfe,
0x03,
0xff,
0x5a,
0x87,
0x5f,
0xad,
0x2b,
0xf8,
0xdf,
0xd6,
0x03,
0x1e,
0xfa,
0x89,
0x7c,
0x20,
0x7d,
0xc0,
0x9f,
0x75,
0xf8,
0x65,
0xb0,
0x80,
0xff,
0xef,
0x01,
0x00,
0xff,
0xdb,
0x7a,
0xc0,
0x83,
0x3f,
0x31,
0x01,
0xfc,
0x07,
0xfe,
0xf4,
0x0f,
0xdf,
0x5a,
0x4f,
0xa0,
0x3e,
0xe0,
0xd8,
0x3a,
0xfc,
0x32,
0x00,
0xc0,
0x01,
0x48,
0x3f,
0x80,
0xff,
0xc0,
0xc0,
0xbf,
0xe1,
0x1f,
0xff,
0x00,
0xfe,
0x03,
0x00,
0xff,
0x86,
0x7f,
0xfc,
0x03,
0xf8,
0x0f,
0x01,
0xfc,
0x1b,
0xfe,
0x7b,
0x00,
0xe0,
0xff,
0xe0,
0x1f,
0xc0,
0x7f,
0x0f,
0xe0,
0xdf,
0xf0,
0x81,
0x7f,
0x00,
0xff,
0x3f,
0x80,
0x7f,
0xc3,
0x07,
0xfe,
0x01,
0xfc,
0xff,
0x00,
0xfe,
0x0d,
0xff,
0x3d,
0x00,
0xf0,
0x3f,
0xf0,
0x0f,
0xe0,
0xf8,
0x07,
0xf0,
0x6f,
0xff,
0xc0,
0x3f,
0x80,
0xe1,
0x1f,
0xc0,
0xbf,
0xfe,
0x03,
0xff,
0x00,
0x86,
0x7f,
0x00,
0xff,
0xf8,
0xff,
0x1e,
0x00,
0xf0,
0x1f,
0xf8,
0x07,
0x37,
0xfc,
0x03,
0xf8,
0xc0,
0x7f,
0xe0,
0x1f,
0xdf,
0xf0,
0x0f,
0xe0,
0x00,
0xff,
0x81,
0x7f,
0x7f,
0xc3,
0x3f,
0x80,
0x00,
0xfc,
0x7f,
0x0f,
0x03,
0xf8,
0x0f,
0xfc,
0xfc,
0x1b,
0xfe,
0x01,
0x0f,
0xe0,
0x3f,
0xf0,
0xf0,
0x6f,
0xf8,
0x07,
0x3f,
0x80,
0xff,
0xc0,
0xc0,
0xbf,
0xe1,
0x1f,
0x07,
0x00,
0xfe,
0xbf,
0xfe,
0x01,
0xfc,
0x07,
0x00,
0xfe,
0x0d,
0xff,
0xf8,
0x07,
0xf0,
0x1f,
0x03,
0xf8,
0x37,
0xfc,
0xe0,
0x1f,
0xc0,
0x7f,
0x0f,
0xe0,
0xdf,
0xf0,
0xdf,
0x03,
0x00,
0xff,
0x03,
0xff,
0x00,
0xfe,
0x7f,
0x00,
0xff,
0x86,
0x0f,
0xfc,
0x03,
0xf8,
0xfe,
0x01,
0xfc,
0x1b,
0x3f,
0xf0,
0x0f,
0xe0,
0xf8,
0x07,
0xf0,
0x6f,
0xff,
0xef,
0x01,
0x80
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
