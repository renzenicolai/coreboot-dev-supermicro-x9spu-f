PHDRS
{
 to_load PT_LOAD;
}
ENTRY(_start)
SECTIONS
{
 _ = ASSERT(. <= 0x2000000, "romstage overlaps the previous region!"); . = 0x2000000; _romstage = .; _eromstage = _romstage + 1M; _ = ASSERT(_eprogram - _program <= 1M, "Romstage exceeded its allotted size! (1M)"); INCLUDE "romstage/lib/program.ld"
. = 0xfefe0000;
.car.data . (NOLOAD) : {
 _car_region_start = . ;
 _ = ASSERT(. <= ., "preram_cbmem_console overlaps the previous region!"); . = .; _preram_cbmem_console = .; _ = ASSERT(. == ALIGN(4), "preram_cbmem_console must be aligned to 4!"); _ = ASSERT(. <= . + (0 ? 0 : 0xc00), "epreram_cbmem_console overlaps the previous region!"); . = . + (0 ? 0 : 0xc00); _epreram_cbmem_console = .;
 _car_relocatable_data_start = .;
 _ = ASSERT(. <= ., "timestamp overlaps the previous region!"); . = .; _timestamp = .; _ = ASSERT(. == ALIGN(8), "timestamp must be aligned to 8!"); _ = ASSERT(. <= . + 0x100, "etimestamp overlaps the previous region!"); . = . + 0x100; _etimestamp = .; _ = ASSERT(0x100 >= 212, "Timestamp region must fit timestamp_cache!");
 _car_global_start = .;
 *(.car.global_data);
 . = ALIGN(8);
 _car_global_end = .;
 _car_relocatable_data_end = .;
 _car_region_end = . + 0x20000 - (. - _car_region_start);
}
. = 0xffffff00;
.illegal_globals . : {
 *(EXCLUDE_FILE ("*/libagesa.*.a:" "*/romstage*/buildOpts.o" "*/romstage*/agesawrapper.o" "*/vendorcode/amd/agesa/*" "*/vendorcode/amd/cimx/*") .data)
  *(EXCLUDE_FILE ("*/libagesa.*.a:" "*/romstage*/buildOpts.o" "*/romstage*/agesawrapper.o" "*/vendorcode/amd/agesa/*" "*/vendorcode/amd/cimx/*") .data.*)
 *(.bss)
 *(.bss.*)
 *(.sbss)
 *(.sbss.*)
}
_bogus = ASSERT((0x20000 == 0) || (SIZEOF(.car.data) <= 0x20000), "Cache as RAM area is too full");
}
