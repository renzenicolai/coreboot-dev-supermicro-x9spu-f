.section ".rom.data"
.balign 16
L1:
.int 0xff, 0, 0, 0
L2:
.int 0xffff, 0, 0, 0
.section ".rom.text"
	/* ,(null):0.0 */
L1001:
L1002:
	/* intel_microcode_find,microcode.c:127.68 */
	/* intel_microcode_find,microcode.c:127.69 */
.section ".rom.data"
.balign 1
L1003:
.byte 0x63
.byte 0x70
.byte 0x75
.byte 0x5f
.byte 0x6d
.byte 0x69
.byte 0x63
.byte 0x72
.byte 0x6f
.byte 0x63
.byte 0x6f
.byte 0x64
.byte 0x65
.byte 0x5f
.byte 0x62
.byte 0x6c
.byte 0x6f
.byte 0x62
.byte 0x2e
.byte 0x62
.byte 0x69
.byte 0x6e
.byte 0x00
.section ".rom.text"
	/* main,bootblock_simple.c:34.50 */
.section ".rom.data"
.balign 1
L1004:
.byte 0x66
.byte 0x61
.byte 0x6c
.byte 0x6c
.byte 0x62
.byte 0x61
.byte 0x63
.byte 0x6b
.byte 0x2f
.byte 0x72
.byte 0x6f
.byte 0x6d
.byte 0x73
.byte 0x74
.byte 0x61
.byte 0x67
.byte 0x65
.byte 0x00
.section ".rom.text"
	/* ,(null):0.0 */
	/* ,:0.0 */
#ASM
	
#NOT ASM
	/* ,:0.0 */
	movd %eax, %xmm0
	movdqa %xmm0, %xmm1
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * boot_cpu,boot_cpu.c:9.20
	 * main,bootblock_simple.c:20.21
	 */
	mov  $27 , %ecx
	rdmsr
	/*
	 * boot_cpu,boot_cpu.c:10.34
	 * main,bootblock_simple.c:20.21
	 */
	/*
	 * boot_cpu,boot_cpu.c:10.35
	 * main,bootblock_simple.c:20.21
	 */
	and  $256 , %eax
	/* main,bootblock_simple.c:20.23 */
	test %eax, %eax
	jz L1046
L1005:
	/*
	 * bootblock_northbridge_init,bootblock.c:22.16
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * pci_io_write_config32,pci_io_cfg.h:93.38
	 * bootblock_northbridge_init,bootblock.c:23.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_outl,<built-in>:1.0
	 * outl,io.h:44.23
	 * pci_io_write_config32,pci_io_cfg.h:93.13
	 * bootblock_northbridge_init,bootblock.c:23.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $2147483748 , %eax
	mov  $3320 , %dx
	outl %eax, %dx
	/* ,:0.0 */
	/*
	 * __builtin_outl,<built-in>:1.0
	 * outl,io.h:44.23
	 * pci_io_write_config32,pci_io_cfg.h:94.13
	 * bootblock_northbridge_init,bootblock.c:23.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %eax
	mov  $3324 , %dx
	outl %eax, %dx
	/*
	 * bootblock_northbridge_init,bootblock.c:24.49
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * pci_io_write_config32,pci_io_cfg.h:93.38
	 * bootblock_northbridge_init,bootblock.c:25.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_outl,<built-in>:1.0
	 * outl,io.h:44.23
	 * pci_io_write_config32,pci_io_cfg.h:93.13
	 * bootblock_northbridge_init,bootblock.c:25.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $2147483744 , %eax
	mov  $3320 , %dx
	outl %eax, %dx
	/* ,:0.0 */
	/*
	 * __builtin_outl,<built-in>:1.0
	 * outl,io.h:44.23
	 * pci_io_write_config32,pci_io_cfg.h:94.13
	 * bootblock_northbridge_init,bootblock.c:25.30
	 * bootblock_mainboard_init,bootblock_romcc.h:38.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4160749573 , %eax
	mov  $3324 , %dx
	outl %eax, %dx
	/*
	 * rdtsc,tsc.h:27.10
	 * store_initial_timestamp,bootblock.c:26.26
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	mfence
rdtsc
#NOT ASM
	/*
	 * pci_mmio_write_config32,pci_mmio_cfg.h:67.74
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:27.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1056:
.int 4160749788
.section ".rom.text"
	movd L1056, %xmm1
	/*
	 * write32,io.h:186.38
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:27.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %esp
	/*
	 * write32,io.h:186.47
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:27.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl %eax, (%esp)
	/*
	 * pci_write_config32,io.h:299.12
	 * store_initial_timestamp,bootblock.c:27.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1006
	/*
	 * pci_write_config32,io.h:299.12
	 * store_initial_timestamp,bootblock.c:27.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
L1006:
	/*
	 * pci_mmio_write_config32,pci_mmio_cfg.h:67.74
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:28.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1057:
.int 4161773776
.section ".rom.text"
	movd L1057, %xmm1
	/*
	 * write32,io.h:186.38
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:28.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %esp
	/*
	 * write32,io.h:186.47
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * store_initial_timestamp,bootblock.c:28.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl %edx, (%esp)
	/*
	 * pci_write_config32,io.h:299.12
	 * store_initial_timestamp,bootblock.c:28.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1007
	/*
	 * pci_write_config32,io.h:299.12
	 * store_initial_timestamp,bootblock.c:28.27
	 * bootblock_southbridge_init,bootblock.c:85.32
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
L1007:
	/*
	 * pci_mmio_read_config8,pci_mmio_cfg.h:27.67
	 * pci_read_config8,io.h:253.45
	 * enable_spi_prefetch,bootblock.c:41.32
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1058:
.int 4161765596
.section ".rom.text"
	movd L1058, %xmm1
	/*
	 * read8,io.h:161.44
	 * pci_mmio_read_config8,pci_mmio_cfg.h:28.21
	 * pci_read_config8,io.h:253.45
	 * enable_spi_prefetch,bootblock.c:41.32
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %esp
	/*
	 * read8,io.h:161.45
	 * pci_mmio_read_config8,pci_mmio_cfg.h:28.21
	 * pci_read_config8,io.h:253.45
	 * enable_spi_prefetch,bootblock.c:41.32
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movzbl (%esp), %esp
	/*
	 * pci_read_config8,io.h:254.12
	 * enable_spi_prefetch,bootblock.c:41.32
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1008
	/*
	 * pci_read_config8,io.h:251.1
	 * enable_spi_prefetch,bootblock.c:41.32
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
L1008:
	/*
	 * enable_spi_prefetch,bootblock.c:42.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	and  $4294967283 , %esp
	/*
	 * enable_spi_prefetch,bootblock.c:43.24
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_spi_prefetch,bootblock.c:43.25
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	or  $8 , %esp
	mov %esp, %eax
	/*
	 * pci_mmio_write_config8,pci_mmio_cfg.h:51.67
	 * pci_write_config8,io.h:280.39
	 * enable_spi_prefetch,bootblock.c:44.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1059:
.int 4161765596
.section ".rom.text"
	movd L1059, %xmm1
	/*
	 * write8,io.h:176.37
	 * pci_mmio_write_config8,pci_mmio_cfg.h:52.15
	 * pci_write_config8,io.h:280.39
	 * enable_spi_prefetch,bootblock.c:44.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %esp
	/*
	 * write8,io.h:176.46
	 * pci_mmio_write_config8,pci_mmio_cfg.h:52.15
	 * pci_write_config8,io.h:280.39
	 * enable_spi_prefetch,bootblock.c:44.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movb %al, (%esp)
	/*
	 * pci_write_config8,io.h:281.12
	 * enable_spi_prefetch,bootblock.c:44.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1009
	/*
	 * pci_write_config8,io.h:281.12
	 * enable_spi_prefetch,bootblock.c:44.26
	 * bootblock_southbridge_init,bootblock.c:87.28
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
L1009:
	/*
	 * enable_port80_on_lpc,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * enable_port80_on_lpc,bootblock.c:52.63
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_port80_on_lpc,bootblock.c:52.66
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	or  $1 , %esp
	/*
	 * pci_mmio_write_config32,pci_mmio_cfg.h:67.74
	 * pci_write_config32,io.h:298.40
	 * enable_port80_on_lpc,bootblock.c:52.27
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1060:
.int 4161765616
.section ".rom.text"
	movd L1060, %xmm1
	/*
	 * write32,io.h:186.38
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * enable_port80_on_lpc,bootblock.c:52.27
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %ebp
	/*
	 * write32,io.h:186.47
	 * pci_mmio_write_config32,pci_mmio_cfg.h:68.16
	 * pci_write_config32,io.h:298.40
	 * enable_port80_on_lpc,bootblock.c:52.27
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl %esp, (%ebp)
	/*
	 * pci_write_config32,io.h:299.12
	 * enable_port80_on_lpc,bootblock.c:52.27
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1010
	/*
	 * pci_write_config32,io.h:299.12
	 * enable_port80_on_lpc,bootblock.c:52.27
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
L1010:
	/*
	 * enable_port80_on_lpc,bootblock.c:56.64
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_port80_on_lpc,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * enable_port80_on_lpc,bootblock.c:56.64
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $13328 , %esp
	/*
	 * enable_port80_on_lpc,bootblock.c:56.65
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_port80_on_lpc,bootblock.c:57.25
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl (%esp), %ebp
	/*
	 * enable_port80_on_lpc,bootblock.c:58.30
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	and  $4294967291 , %ebp
	/*
	 * enable_port80_on_lpc,bootblock.c:59.21
	 * bootblock_southbridge_init,bootblock.c:88.29
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl %ebp, (%esp)
	/*
	 * set_spi_speed,RCBA32:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * set_spi_speed,RCBA32:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $14512 , %esp
	/*
	 * set_spi_speed,RCBA32:1.42
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,bootblock.c:69.32
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4096 , %ebp
	movl %ebp, (%esp)
	/*
	 * set_spi_speed,RCBA32:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * set_spi_speed,RCBA32:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $14516 , %esp
	/*
	 * set_spi_speed,RCBA32:1.42
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,bootblock.c:72.30
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movl (%esp), %esp
	/*
	 * set_spi_speed,bootblock.c:73.20
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	shr  $24 , %esp
	/*
	 * set_spi_speed,bootblock.c:74.18
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	and  $7 , %esp
	/*
	 * set_spi_speed,RCBA8:1.40
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %ebp
	/*
	 * set_spi_speed,RCBA8:1.40
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $14483 , %ebp
	/*
	 * set_spi_speed,RCBA8:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,bootblock.c:77.29
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movzbl (%ebp), %ebp
	/*
	 * set_spi_speed,bootblock.c:78.19
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	and  $4294967288 , %ebp
	/*
	 * set_spi_speed,bootblock.c:79.21
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	or %esp, %ebp
	mov %ebp, %eax
	/*
	 * set_spi_speed,RCBA8:1.40
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,DEFAULT_RCBA:1.18
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * set_spi_speed,RCBA8:1.40
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $14483 , %esp
	/*
	 * set_spi_speed,RCBA8:1.41
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_spi_speed,bootblock.c:80.29
	 * bootblock_southbridge_init,bootblock.c:89.22
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	movb %al, (%esp)
	/*
	 * bootblock_southbridge_init,RCBA32:1.41
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * bootblock_southbridge_init,DEFAULT_RCBA:1.18
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * bootblock_southbridge_init,RCBA32:1.41
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	add  $13312 , %esp
	/*
	 * bootblock_southbridge_init,RCBA32:1.42
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * bootblock_southbridge_init,bootblock.c:92.30
	 * bootblock_mainboard_init,bootblock_romcc.h:41.35
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4 , %ebp
	movl %ebp, (%esp)
	/* ,:0.0 */
	/*
	 * cpuid_eax,cpu.h:101.39
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:70.22
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $1 , %eax
#ASM
	mov %ebx, %edi;cpuid;mov %edi, %ebx;
#NOT ASM
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:70.52
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $198306 , %eax
	jae L1012
L1011:
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:74.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1021
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:70.52
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1012:
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:74.27
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $404 , %ecx
	rdmsr
	movd %eax, %xmm2
	movd %edx, %xmm1
	/*
	 * set_flex_ratio_to_tdp_nominal,FLEX_RATIO_EN:1.9
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:75.44
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm2, %esp
	and  $65536 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:75.45
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %esp, %esp
	jnz L1014
L1013:
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:79.11
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1021
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:75.45
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1014:
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:79.20
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $206 , %ecx
	rdmsr
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:80.26
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $1 , %edx
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:80.31
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $3 , %edx
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:80.37
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $0 , %edx
	jnz L1016
L1015:
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:84.11
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1021
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:80.37
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1016:
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:84.20
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $1608 , %ecx
	rdmsr
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:85.38
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $255 , %eax
	mov %eax, %esp
	and $0xff, %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:88.33
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm2, %ebp
	shr  $8 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:88.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $255 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:88.59
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp %esp, %ebp
	jnz L1018
L1017:
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:92.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1021
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:88.59
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1018:
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:92.33
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm2, %eax
	and  $4294902015 , %eax
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:93.44
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %esp, %ebp
	shl  $8 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:93.44
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %eax
	/*
	 * set_flex_ratio_to_tdp_nominal,FLEX_RATIO_LOCK:1.9
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:94.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or  $1048576 , %eax
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:95.14
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $404 , %ecx
	movd %xmm1, %edx
	wrmsr
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,DEFAULT_RCBA:1.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $14584 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:99.45
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:100.35
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $4294963263 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:101.44
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $63 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:101.50
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shl  $6 , %esp
	or %esp, %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,DEFAULT_RCBA:1.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.41
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $14584 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:102.45
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl %ebp, (%esp)
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,DEFAULT_RCBA:1.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $14580 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.43
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA_AND_OR:1.37
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%esp), %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA_AND_OR:1.45
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA_AND_OR:1.46
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $4294967295 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA_AND_OR:1.54
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA_AND_OR:1.55
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or  $1 , %esp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,DEFAULT_RCBA:1.18
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4275159040 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.42
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $14580 , %ebp
	/*
	 * set_flex_ratio_to_tdp_nominal,RCBA32:1.43
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:105.38
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl %esp, (%ebp)
	/* ,:0.0 */
	/*
	 * __builtin_outb,<built-in>:1.0
	 * outb,io.h:34.23
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:108.13
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %al
	mov  $3321 , %dx
	outb %al, %dx
	/* ,:0.0 */
	/*
	 * __builtin_outb,<built-in>:1.0
	 * outb,io.h:34.23
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:109.13
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $6 , %al
	mov  $3321 , %dx
	outb %al, %dx
	/*
	 * halt,halt.c:21.17
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:110.13
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1020
L1019:
	/*
	 * __builtin_hlt,<built-in>:1.0
	 * hlt,hlt.h:20.22
	 * halt,halt.c:22.20
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:110.13
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	hlt
	/*
	 * halt,halt.c:21.17
	 * set_flex_ratio_to_tdp_nominal,bootblock.c:110.13
	 * bootblock_cpu_init,bootblock.c:116.38
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1020:
	jmp L1019
L1021:
	/*
	 * read_cr0,cr.h:48.10
	 * disable_cache,cache.h:75.23
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	mov %cr0, %esp
#NOT ASM
	/*
	 * disable_cache,cache.h:75.25
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * disable_cache,CR0_CD:1.9
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * disable_cache,cache.h:76.22
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or  $1073741824 , %esp
	/*
	 * wbinvd,cache.h:43.32
	 * disable_cache,cache.h:77.15
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	wbinvd
#NOT ASM
	/* ,:0.0 */
	/*
	 * write_cr0,cr.h:59.10
	 * disable_cache,cache.h:78.18
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	mov %esp, %cr0
#NOT ASM
	/*
	 * wbinvd,cache.h:43.32
	 * disable_cache,cache.h:79.15
	 * enable_rom_caching,bootblock.c:53.22
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	wbinvd
#NOT ASM
	/*
	 * set_var_mtrr,bootblock.c:41.31
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_var_mtrr,bootblock.c:42.21
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * set_var_mtrr,bootblock.c:43.14
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4286578693 , %eax
	mov  $0 , %edx
	mov  $514 , %ecx
	wrmsr
	/*
	 * set_var_mtrr,bootblock.c:44.54
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * set_var_mtrr,bootblock.c:45.58
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * set_var_mtrr,bootblock.c:46.14
	 * enable_rom_caching,bootblock.c:54.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $4286580736 , %eax
	mov  $15 , %edx
	mov  $515 , %ecx
	wrmsr
	/*
	 * read_cr0,cr.h:48.10
	 * enable_cache,cache.h:66.23
	 * enable_rom_caching,bootblock.c:55.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	mov %cr0, %esp
#NOT ASM
	/*
	 * enable_cache,cache.h:66.25
	 * enable_rom_caching,bootblock.c:55.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_cache,cache.h:67.34
	 * enable_rom_caching,bootblock.c:55.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $2684354559 , %esp
	/* ,:0.0 */
	/*
	 * write_cr0,cr.h:59.10
	 * enable_cache,cache.h:68.18
	 * enable_rom_caching,bootblock.c:55.21
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	mov %esp, %cr0
#NOT ASM
	/*
	 * enable_rom_caching,bootblock.c:58.28
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * enable_rom_caching,bootblock.c:59.28
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * enable_rom_caching,bootblock.c:60.14
	 * bootblock_cpu_init,bootblock.c:117.27
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %edx
	mov  $2048 , %eax
	mov  $767 , %ecx
	wrmsr
	/*
	 * intel_microcode_find,microcode.c:127.68
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * walkcbfs_head,cbfs.h:30.85
	 * intel_microcode_find,microcode.c:127.39
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $L1003+0 , %esi
#ASM
	mov $1f, %esp
	jmp walkcbfs_asm
	1:
	
#NOT ASM
	/*
	 * walkcbfs_head,cbfs.h:32.1
	 * intel_microcode_find,microcode.c:127.39
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:128.28
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %eax, %eax
	jnz L1023
L1022:
	/*
	 * intel_microcode_find,NULL:1.11
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
.section ".rom.data"
.balign 4
L1061:
.int 0
.section ".rom.text"
	movd L1061, %xmm1
	movd %xmm1, %esp
	/*
	 * intel_microcode_find,microcode.c:131.21
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1040
	/*
	 * intel_microcode_find,microcode.c:128.28
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1023:
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %esp
	add  $20 , %esp
	/*
	 * intel_microcode_find,swab32:1.61
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%esp), %esp
	/*
	 * intel_microcode_find,swab32:1.89
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $255 , %esp
	/*
	 * intel_microcode_find,swab32:1.96
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shl  $24 , %esp
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * intel_microcode_find,swab32:1.143
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * intel_microcode_find,swab32:1.171
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $65280 , %ebp
	/*
	 * intel_microcode_find,swab32:1.177
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shl  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.179
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %esp
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * intel_microcode_find,swab32:1.224
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * intel_microcode_find,swab32:1.252
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $16711680 , %ebp
	/*
	 * intel_microcode_find,swab32:1.258
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.260
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %esp
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * intel_microcode_find,swab32:1.305
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * intel_microcode_find,swab32:1.333
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $4278190080 , %ebp
	/*
	 * intel_microcode_find,swab32:1.340
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $24 , %ebp
	/*
	 * intel_microcode_find,swab32:1.342
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %esp
	/*
	 * intel_microcode_find,swab32:1.343
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,CBFS_SUBHEADER:1.78
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,CBFS_SUBHEADER:1.43
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	/*
	 * intel_microcode_find,CBFS_SUBHEADER:1.78
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add %esp, %ebp
	/*
	 * intel_microcode_find,CBFS_SUBHEADER:1.81
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %ebp, %xmm1
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %esp
	add  $8 , %esp
	/*
	 * intel_microcode_find,swab32:1.56
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%esp), %esp
	/*
	 * intel_microcode_find,swab32:1.84
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $255 , %esp
	/*
	 * intel_microcode_find,swab32:1.91
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shl  $24 , %esp
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	add  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.133
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * intel_microcode_find,swab32:1.161
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $65280 , %ebp
	/*
	 * intel_microcode_find,swab32:1.167
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shl  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.169
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %esp
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %eax, %ebp
	add  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.209
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ebp), %ebp
	/*
	 * intel_microcode_find,swab32:1.237
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $16711680 , %ebp
	/*
	 * intel_microcode_find,swab32:1.243
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $8 , %ebp
	/*
	 * intel_microcode_find,swab32:1.245
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %ebp, %esp
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $8 , %eax
	/*
	 * intel_microcode_find,swab32:1.285
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%eax), %eax
	/*
	 * intel_microcode_find,swab32:1.313
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $4278190080 , %eax
	/*
	 * intel_microcode_find,swab32:1.320
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $24 , %eax
	/*
	 * intel_microcode_find,swab32:1.322
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	or %eax, %esp
	/*
	 * intel_microcode_find,swab32:1.323
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %esp, %xmm2
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:142.19
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:143.19
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * intel_microcode_find,microcode.c:144.14
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %eax
	mov  $0 , %edx
	mov  $139 , %ecx
	wrmsr
	/* ,:0.0 */
	/*
	 * cpuid_eax,cpu.h:101.39
	 * intel_microcode_find,microcode.c:145.24
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $1 , %eax
#ASM
	mov %ebx, %edi;cpuid;mov %edi, %ebx;
#NOT ASM
	mov %eax, %esp
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * intel_microcode_find,microcode.c:146.20
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $139 , %ecx
	rdmsr
	/*
	 * intel_microcode_find,microcode.c:148.29
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %esp, %ebp
	shr  $4 , %ebp
	/*
	 * intel_microcode_find,microcode.c:148.37
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $15 , %ebp
	/*
	 * intel_microcode_find,microcode.c:149.30
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %esp, %edi
	shr  $8 , %edi
	/*
	 * intel_microcode_find,microcode.c:149.38
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $15 , %edi
	/*
	 * intel_microcode_find,microcode.c:152.15
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:153.28
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $5 , %ebp
	setae %al
	movzx %al, %eax
	movd %eax, %xmm3
	/*
	 * intel_microcode_find,microcode.c:153.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $5 , %ebp
	jae L1025
L1024:
	/*
	 * intel_microcode_find,microcode.c:153.44
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $6 , %edi
	seta %al
	movzx %al, %eax
	movd %eax, %xmm3
	/*
	 * intel_microcode_find,microcode.c:153.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1025:
	/*
	 * intel_microcode_find,microcode.c:152.15
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %ebp
	/*
	 * intel_microcode_find,microcode.c:153.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm3, %edi
	/*
	 * intel_microcode_find,microcode.c:153.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %edi, %edi
	jz L1027
L1026:
	/* ,:0.0 */
	/*
	 * __builtin_rdmsr,<built-in>:1.0
	 * rdmsr,msr.h:10.31
	 * intel_microcode_find,microcode.c:154.28
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $23 , %ecx
	rdmsr
	/*
	 * intel_microcode_find,microcode.c:155.41
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	shr  $18 , %edx
	/*
	 * intel_microcode_find,microcode.c:155.46
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and  $7 , %edx
	/*
	 * intel_microcode_find,microcode.c:155.25
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:155.47
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movb %dl, %cl
	mov  $1 , %ebp
	shl %cl, %ebp
	/*
	 * intel_microcode_find,microcode.c:153.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1027:
	/*
	 * intel_microcode_find,microcode.c:131.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %edx
	/*
	 * intel_microcode_find,microcode.c:132.51
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm2, %edi
	/*
	 * intel_microcode_find,microcode.c:120.15
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:165.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1038
L1028:
	/*
	 * intel_microcode_find,microcode.c:168.46
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %edx, %esi
	add  $32 , %esi
	movl (%esi), %esi
	test %esi, %esi
	jz L1030
L1029:
	/*
	 * intel_microcode_find,microcode.c:169.64
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %edx, %esi
	add  $32 , %esi
	movl (%esi), %esi
	/*
	 * intel_microcode_find,microcode.c:170.22
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1031
	/*
	 * intel_microcode_find,microcode.c:168.46
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1030:
	/*
	 * intel_microcode_find,microcode.c:174.43
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $2048 , %esi
	/*
	 * intel_microcode_find,microcode.c:170.22
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1031:
	/*
	 * intel_microcode_find,microcode.c:120.38
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:178.48
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp %edi, %esi
	jbe L1033
L1032:
	/*
	 * intel_microcode_find,microcode.c:182.30
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1039
	/*
	 * intel_microcode_find,microcode.c:178.48
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1033:
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %edx, %ecx
	add  $12 , %ecx
	/*
	 * intel_microcode_find,microcode.c:185.42
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ecx), %ecx
	/*
	 * intel_microcode_find,microcode.c:185.47
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp %esp, %ecx
	setz %al
	movzx %al, %ecx
	movd %ecx, %xmm1
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %ecx, %ecx
	jz L1035
L1034:
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %edx, %ecx
	add  $24 , %ecx
	/*
	 * intel_microcode_find,microcode.c:185.71
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movl (%ecx), %ecx
	/*
	 * intel_microcode_find,microcode.c:185.75
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	and %ebp, %ecx
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %ecx, %ecx
	setnz %al
	movzx %al, %eax
	movd %eax, %xmm1
L1035:
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %xmm1, %ecx
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %ecx, %ecx
	jz L1037
L1036:
	/*
	 * intel_microcode_find,microcode.c:188.29
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %edx, %esp
	jmp L1040
	/*
	 * intel_microcode_find,microcode.c:185.76
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1037:
	/*
	 * intel_microcode_find,microcode.c:188.77
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	movd %esi, %xmm1
	movd %xmm1, %ecx
	/*
	 * intel_microcode_find,microcode.c:188.64
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:188.77
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add %ecx, %edx
	/*
	 * intel_microcode_find,microcode.c:188.78
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:189.45
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	sub %esi, %edi
	/*
	 * intel_microcode_find,microcode.c:165.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1038:
	/*
	 * intel_microcode_find,microcode.c:118.29
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:117.46
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_find,microcode.c:165.55
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	cmp  $48 , %edi
	jae L1028
L1039:
	/*
	 * intel_microcode_find,microcode.c:193.25
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %esp
	/*
	 * intel_microcode_find,microcode.c:116.1
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1040:
	/*
	 * intel_microcode_find,microcode.c:116.1
	 * intel_update_microcode_from_cbfs,microcode.c:198.49
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* main,bootblock_simple.c:19.1 */
	/*
	 * intel_microcode_load_unlocked,microcode.c:84.52
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_load_unlocked,microcode.c:86.15
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	test %esp, %esp
	jnz L1042
L1041:
	/*
	 * intel_microcode_load_unlocked,microcode.c:89.19
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1045
	/*
	 * intel_microcode_load_unlocked,microcode.c:86.15
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1042:
	/*
	 * read_microcode_rev,microcode.c:74.10
	 * intel_microcode_load_unlocked,microcode.c:89.41
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
#ASM
	xorl %eax, %eax
	xorl %edx, %edx
	movl $0x8b, %ecx
	wrmsr
	movl $0x01, %eax
	cpuid
	movl $0x08b, %ecx
	rdmsr
	
#NOT ASM
	/*
	 * read_microcode_rev,microcode.c:76.1
	 * intel_microcode_load_unlocked,microcode.c:89.41
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_load_unlocked,microcode.c:92.34
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %esp, %ebp
	add  $4 , %ebp
	movl (%ebp), %ebp
	cmp %ebp, %edx
	jnz L1044
L1043:
	/*
	 * intel_microcode_load_unlocked,microcode.c:103.11
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	jmp L1045
	/*
	 * intel_microcode_load_unlocked,microcode.c:92.34
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1044:
	/*
	 * intel_microcode_load_unlocked,microcode.c:103.35
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov %esp, %eax
	/*
	 * intel_microcode_load_unlocked,microcode.c:103.60
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/*
	 * intel_microcode_load_unlocked,microcode.c:103.61
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	add  $48 , %eax
	/*
	 * intel_microcode_load_unlocked,microcode.c:104.19
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	/* ,:0.0 */
	/*
	 * __builtin_wrmsr,<built-in>:1.0
	 * wrmsr,msr.h:15.24
	 * intel_microcode_load_unlocked,microcode.c:105.14
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
	mov  $0 , %edx
	mov  $121 , %ecx
	wrmsr
	/*
	 * intel_microcode_load_unlocked,microcode.c:81.1
	 * intel_update_microcode_from_cbfs,microcode.c:204.38
	 * bootblock_cpu_init,bootblock.c:118.41
	 * bootblock_mainboard_init,bootblock_romcc.h:44.27
	 * main,bootblock_simple.c:21.41
	 */
L1045:
	/* main,bootblock_simple.c:19.1 */
	movdqa %xmm0, %xmm1
	/* main,bootblock_simple.c:20.23 */
L1046:
	/* main,bootblock_simple.c:19.1 */
	movdqa %xmm1, %xmm0
	/* main,bootblock_simple.c:34.50 */
	/*
	 * walkcbfs_head,cbfs.h:30.85
	 * walkcbfs,cbfs.h:36.47
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov  $L1004+0 , %esi
#ASM
	mov $1f, %esp
	jmp walkcbfs_asm
	1:
	
#NOT ASM
	/*
	 * walkcbfs_head,cbfs.h:32.1
	 * walkcbfs,cbfs.h:36.47
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * walkcbfs,cbfs.h:37.24
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %esp
	/*
	 * walkcbfs,cbfs.h:37.27
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	cmp  $0 , %esp
	jz L1048
L1047:
	/*
	 * walkcbfs,cbfs.h:41.14
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %esp
	add  $20 , %esp
	/*
	 * walkcbfs,swab32:1.51
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	movl (%esp), %esp
	/*
	 * walkcbfs,swab32:1.79
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	and  $255 , %esp
	/*
	 * walkcbfs,swab32:1.86
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	shl  $24 , %esp
	/*
	 * walkcbfs,cbfs.h:41.14
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * walkcbfs,swab32:1.123
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	movl (%ebp), %ebp
	/*
	 * walkcbfs,swab32:1.151
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	and  $65280 , %ebp
	/*
	 * walkcbfs,swab32:1.157
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	shl  $8 , %ebp
	/*
	 * walkcbfs,swab32:1.159
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	or %ebp, %esp
	/*
	 * walkcbfs,cbfs.h:41.14
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * walkcbfs,swab32:1.194
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	movl (%ebp), %ebp
	/*
	 * walkcbfs,swab32:1.222
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	and  $16711680 , %ebp
	/*
	 * walkcbfs,swab32:1.228
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	shr  $8 , %ebp
	/*
	 * walkcbfs,swab32:1.230
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	or %ebp, %esp
	/*
	 * walkcbfs,cbfs.h:41.14
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %ebp
	add  $20 , %ebp
	/*
	 * walkcbfs,swab32:1.265
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	movl (%ebp), %ebp
	/*
	 * walkcbfs,swab32:1.293
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	and  $4278190080 , %ebp
	/*
	 * walkcbfs,swab32:1.300
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	shr  $24 , %ebp
	/*
	 * walkcbfs,swab32:1.302
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	or %ebp, %esp
	/*
	 * walkcbfs,swab32:1.303
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * walkcbfs,CBFS_SUBHEADER:1.58
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * walkcbfs,CBFS_SUBHEADER:1.33
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * walkcbfs,CBFS_SUBHEADER:1.58
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	add %esp, %eax
	/*
	 * walkcbfs,CBFS_SUBHEADER:1.61
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	mov %eax, %esp
	/*
	 * walkcbfs,cbfs.h:41.14
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	jmp L1049
	/*
	 * walkcbfs,cbfs.h:37.27
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
L1048:
	/*
	 * walkcbfs,cbfs.h:41.25
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
.section ".rom.data"
.balign 4
L1062:
.int 0
.section ".rom.text"
	movd L1062, %xmm1
	movd %xmm1, %esp
	/*
	 * walkcbfs,cbfs.h:35.1
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
L1049:
	/*
	 * walkcbfs,cbfs.h:35.1
	 * findstage,cbfs.h:52.55
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * findstage,cbfs.h:52.63
	 * main,bootblock_simple.c:38.26
	 */
	/*
	 * findstage,cbfs.h:53.25
	 * main,bootblock_simple.c:38.26
	 */
	mov %esp, %ebp
	/*
	 * findstage,cbfs.h:53.28
	 * main,bootblock_simple.c:38.26
	 */
	cmp  $0 , %ebp
	jz L1051
L1050:
	/*
	 * findstage,cbfs.h:57.14
	 * main,bootblock_simple.c:38.26
	 */
	add  $4 , %esp
	/*
	 * findstage,cbfs.h:54.36
	 * main,bootblock_simple.c:38.26
	 */
	movl (%esp), %esp
	/*
	 * findstage,cbfs.h:57.14
	 * main,bootblock_simple.c:38.26
	 */
	jmp L1052
	/*
	 * findstage,cbfs.h:53.28
	 * main,bootblock_simple.c:38.26
	 */
L1051:
	/*
	 * findstage,cbfs.h:58.1
	 * main,bootblock_simple.c:38.26
	 */
	mov  $0 , %esp
	/*
	 * findstage,cbfs.h:51.1
	 * main,bootblock_simple.c:38.26
	 */
L1052:
	/*
	 * findstage,cbfs.h:51.1
	 * main,bootblock_simple.c:38.26
	 */
	/* main,bootblock_simple.c:39.18 */
	test %esp, %esp
	jz L1055
L1053:
	/*
	 * call,cbfs.h:62.64
	 * main,bootblock_simple.c:39.24
	 */
	movd %xmm0, %eax
#ASM
	jmp *%esp
	
#NOT ASM
	/*
	 * halt,halt.c:21.17
	 * main,bootblock_simple.c:40.13
	 */
	jmp L1055
L1054:
	/*
	 * __builtin_hlt,<built-in>:1.0
	 * hlt,hlt.h:20.22
	 * halt,halt.c:22.20
	 * main,bootblock_simple.c:40.13
	 */
	hlt
	/*
	 * halt,halt.c:21.17
	 * main,bootblock_simple.c:40.13
	 */
L1055:
	jmp L1054
	/* ,:0.0 */
 /* unknown %eax */
 /* unknown %ebx */
 /* unknown %ecx */
 /* unknown %edx */
#ASM
	
#NOT ASM
/* max inline depth 7 */
