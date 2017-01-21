ENTRY(_start)
MEMORY {
 rom : ORIGIN = 0xffff0000, LENGTH = 64K
}
TARGET(binary)
SECTIONS
{
 .bogus ROMLOC_MIN : {
  . = 0 ? ALIGN(4096) : ALIGN(4);
  ROMLOC = .;
 } >rom = 0xff
 .rom ROMLOC : {
  _rom = .;
  ap_sipi_vector = .;
  *(.rom.text);
  *(.rom.data);
  *(.rom.data.*);
  *(.text);
  *(.text.*);
  *(.rodata);
  *(.rodata.*);
  _erom = .;
 } >rom = 0xff
 ROMLOC_MIN = 0xffffff00 - (_erom - _rom + 16) -
  (0 ? 4096 : 0);
 _bogus = ASSERT(!0 || ((ap_sipi_vector & 0x0fff) == 0x0),
  "Bad SIPI vector alignment");
 _bogus = ASSERT(!0 || (ap_sipi_vector == 0xfffff000),
  "Address mismatch on AP_SIPI_VECTOR");
 /DISCARD/ : {
  *(.comment)
  *(.note)
  *(.comment.*)
  *(.note.*)
  *(.eh_frame)
  *(.iplt)
  *(.rel.*)
  *(.igot.*)
 }
}
 gdtptr16_offset = gdtptr16 & 0xffff;
 nullidt_offset = nullidt & 0xffff;
SECTIONS {
 _bogus = ASSERT(_start16bit >= 0xffff0000, "_start16bit too low. Please report.");
 _ROMTOP = 0xfffffff0;
 . = _ROMTOP;
 .reset . : {
  *(.reset);
  . = 15;
  BYTE(0x00);
 }
}
SECTIONS {
 . = (0xffffffff - 0x80) - (__id_end - __id_start) + 1;
 .id (.): {
  KEEP(*(.id))
 }
}
