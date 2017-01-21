/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 * Copyright (C) 2010 Siemens AG, Inc.
 * (Written by Josef Kellermann <joseph.kellermann@heitec.de> for Siemens AG, Inc.)
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

Name(PCIF, 0)

Method(_PIC, 1, NotSerialized)
{
	Store(Arg0, PCIF)
	If (Arg0)
	{
		\_SB.PCI0.LPC0.CIRQ()
	}
}

External (\_PR.CP00, DeviceObj)
External (\_PR.CP01, DeviceObj)

Scope(\_SB)
{

	Method(_INI, 0)
	{
		Store (2000, OSYS)

		If (CondRefOf(_OSI)) {

			If (_OSI("Linux")) {
				Store (1, LINX)
			}

			If (_OSI("Windows 2001")) {
				Store (2001, OSYS)
			}

			If (_OSI("Windows 2001 SP1")) {
				Store (2001, OSYS)
			}

			If (_OSI("Windows 2001 SP2")) {
				Store (2002, OSYS)
			}

			If (_OSI("Windows 2006")) {
				Store (2006, OSYS)
			}
		}
	}
}
