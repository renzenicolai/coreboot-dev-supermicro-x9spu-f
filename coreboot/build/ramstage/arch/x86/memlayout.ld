PHDRS
{
 to_load PT_LOAD;
}
ENTRY(_start)
SECTIONS
{
 _ = ASSERT(. <= 0x100000, "ramstage overlaps the previous region!"); . = 0x100000; _ramstage = .; _eramstage = _ramstage + 0x200000 - 0x100000; _ = ASSERT(_eprogram - _program <= 0x200000 - 0x100000, "Ramstage exceeded its allotted size! (0x200000 - 0x100000)"); INCLUDE "ramstage/lib/program.ld"
}
