#include <device/device.h>
#include <device/pci.h>
#include "cpu/intel/model_206ax/chip.h"
#include "northbridge/intel/sandybridge/chip.h"
#include "southbridge/intel/bd82x6x/chip.h"

#ifndef __PRE_RAM__
__attribute__((weak)) struct chip_operations mainboard_ops = {};
__attribute__((weak)) struct chip_operations cpu_intel_model_206ax_ops = {};
__attribute__((weak)) struct chip_operations cpu_intel_socket_LGA1155_ops = {};
__attribute__((weak)) struct chip_operations northbridge_intel_sandybridge_ops = {};
__attribute__((weak)) struct chip_operations southbridge_intel_bd82x6x_ops = {};
__attribute__((weak)) struct chip_operations superio_nuvoton_nct6776_ops = {};
#endif

/* pass 0 */
ROMSTAGE_CONST struct bus dev_root_links[];
ROMSTAGE_CONST static struct device _dev2;
ROMSTAGE_CONST struct bus _dev2_links[];
ROMSTAGE_CONST static struct device _dev7;
ROMSTAGE_CONST struct bus _dev7_links[];
ROMSTAGE_CONST static struct device _dev4;
ROMSTAGE_CONST static struct device _dev6;
ROMSTAGE_CONST static struct device _dev8;
ROMSTAGE_CONST static struct device _dev10;
ROMSTAGE_CONST static struct device _dev11;
ROMSTAGE_CONST static struct device _dev12;
ROMSTAGE_CONST static struct device _dev13;
ROMSTAGE_CONST static struct device _dev14;
ROMSTAGE_CONST static struct device _dev15;
ROMSTAGE_CONST static struct device _dev16;
ROMSTAGE_CONST static struct device _dev17;
ROMSTAGE_CONST static struct device _dev18;
ROMSTAGE_CONST static struct device _dev19;
ROMSTAGE_CONST static struct device _dev20;
ROMSTAGE_CONST static struct device _dev21;
ROMSTAGE_CONST static struct device _dev22;
ROMSTAGE_CONST static struct device _dev23;
ROMSTAGE_CONST static struct device _dev24;
ROMSTAGE_CONST static struct device _dev25;
ROMSTAGE_CONST static struct device _dev26;
ROMSTAGE_CONST static struct device _dev27;
ROMSTAGE_CONST static struct device _dev28;
ROMSTAGE_CONST struct bus _dev28_links[];
ROMSTAGE_CONST static struct device _dev41;
ROMSTAGE_CONST static struct device _dev42;
ROMSTAGE_CONST static struct device _dev43;
ROMSTAGE_CONST static struct device _dev44;
ROMSTAGE_CONST static struct device _dev30;
ROMSTAGE_CONST static struct device _dev31;
ROMSTAGE_CONST static struct device _dev32;
ROMSTAGE_CONST struct resource _dev32_res[];
ROMSTAGE_CONST static struct device _dev33;
ROMSTAGE_CONST struct resource _dev33_res[];
ROMSTAGE_CONST static struct device _dev34;
ROMSTAGE_CONST struct resource _dev34_res[];
ROMSTAGE_CONST static struct device _dev35;
ROMSTAGE_CONST static struct device _dev36;
ROMSTAGE_CONST static struct device _dev37;
ROMSTAGE_CONST static struct device _dev38;
ROMSTAGE_CONST static struct device _dev39;
ROMSTAGE_CONST static struct device _dev40;

/* pass 1 */
ROMSTAGE_CONST struct device * ROMSTAGE_CONST last_dev = &_dev44;
ROMSTAGE_CONST struct device dev_root = {
#ifndef __PRE_RAM__
	.ops = &default_dev_ops_root,
#endif
	.bus = &dev_root_links[0],
	.path = { .type = DEVICE_PATH_ROOT },
	.enabled = 1,
	.on_mainboard = 1,
	.link_list = &dev_root_links[0],
#ifndef __PRE_RAM__
	.chip_ops = &mainboard_ops,
	.name = mainboard_name,
#endif
	.next=&_dev2
};
ROMSTAGE_CONST struct bus dev_root_links[] = {
		[0] = {
			.link_num = 0,
			.dev = &dev_root,
			.children = &_dev2,
			.next = NULL,
		},
	};
ROMSTAGE_CONST struct northbridge_intel_sandybridge_config northbridge_intel_sandybridge_info_1 = {
	.gfx.did = { 0x80000100, 0x80000240, 0x80000410, 0x80000410, 0x00000005 },
	.gfx.ndid = 3,
	.pci_mmio_size = 2048,
};

static ROMSTAGE_CONST struct device _dev2 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &dev_root_links[0],
	.path = {.type=DEVICE_PATH_CPU_CLUSTER,{.cpu_cluster={ .cluster = 0x0 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.link_list = &_dev2_links[0],
	.sibling = &_dev7,
#ifndef __PRE_RAM__
	.chip_ops = &northbridge_intel_sandybridge_ops,
#endif
	.chip_info = &northbridge_intel_sandybridge_info_1,
	.next=&_dev4
};
ROMSTAGE_CONST struct bus _dev2_links[] = {
		[0] = {
			.link_num = 0,
			.dev = &_dev2,
			.children = &_dev4,
			.next = NULL,
		},
	};
static ROMSTAGE_CONST struct device _dev7 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &dev_root_links[0],
	.path = {.type=DEVICE_PATH_DOMAIN,{.domain={ .domain = 0x0 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = &_dev7_links[0],
#ifndef __PRE_RAM__
	.chip_ops = &northbridge_intel_sandybridge_ops,
#endif
	.chip_info = &northbridge_intel_sandybridge_info_1,
	.next=&_dev8
};
ROMSTAGE_CONST struct bus _dev7_links[] = {
		[0] = {
			.link_num = 0,
			.dev = &_dev7,
			.children = &_dev8,
			.next = NULL,
		},
	};
ROMSTAGE_CONST struct cpu_intel_model_206ax_config cpu_intel_model_206ax_info_5 = {
	.c1_acpower = 1,
	.c1_battery = 1,
	.c2_acpower = 3,
	.c2_battery = 3,
	.c3_acpower = 5,
	.c3_battery = 5,
};

static ROMSTAGE_CONST struct device _dev4 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev2_links[0],
	.path = {.type=DEVICE_PATH_APIC,{.apic={ .apic_id = 0x0 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.link_list = NULL,
	.sibling = &_dev6,
#ifndef __PRE_RAM__
	.chip_ops = &cpu_intel_socket_LGA1155_ops,
#endif
	.next=&_dev6
};
static ROMSTAGE_CONST struct device _dev6 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev2_links[0],
	.path = {.type=DEVICE_PATH_APIC,{.apic={ .apic_id = 0xacac }}},
	.enabled = 0,
	.on_mainboard = 1,
	.link_list = NULL,
#ifndef __PRE_RAM__
	.chip_ops = &cpu_intel_model_206ax_ops,
#endif
	.chip_info = &cpu_intel_model_206ax_info_5,
	.next=&_dev7
};
static ROMSTAGE_CONST struct device _dev8 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x0,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev10,
#ifndef __PRE_RAM__
	.chip_ops = &northbridge_intel_sandybridge_ops,
#endif
	.chip_info = &northbridge_intel_sandybridge_info_1,
	.next=&_dev10
};
ROMSTAGE_CONST struct southbridge_intel_bd82x6x_config southbridge_intel_bd82x6x_info_9 = {
	.alt_gp_smi_en = 0x0000,
	.c2_latency = 0x0065,
	.docking_supported = 0,
	.gen1_dec = 0x003c0a01,
	.p_cnt_throttling_supported = 0,
	.pcie_port_coalesce = 0,
	.sata_interface_speed_support = 0x3,
	.sata_port_map = 0x3f,
};

static ROMSTAGE_CONST struct device _dev10 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x14,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5007,
	.link_list = NULL,
	.sibling = &_dev11,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev11
};
static ROMSTAGE_CONST struct device _dev11 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x16,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev12,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev12
};
static ROMSTAGE_CONST struct device _dev12 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x16,1)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev13,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev13
};
static ROMSTAGE_CONST struct device _dev13 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x16,2)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev14,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev14
};
static ROMSTAGE_CONST struct device _dev14 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x16,3)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev15,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev15
};
static ROMSTAGE_CONST struct device _dev15 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x19,0)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev16,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev16
};
static ROMSTAGE_CONST struct device _dev16 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1a,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5006,
	.link_list = NULL,
	.sibling = &_dev17,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev17
};
static ROMSTAGE_CONST struct device _dev17 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1b,0)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev18,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev18
};
static ROMSTAGE_CONST struct device _dev18 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev19,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev19
};
static ROMSTAGE_CONST struct device _dev19 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,1)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev20,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev20
};
static ROMSTAGE_CONST struct device _dev20 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,2)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev21,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev21
};
static ROMSTAGE_CONST struct device _dev21 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,3)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev22,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev22
};
static ROMSTAGE_CONST struct device _dev22 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,4)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev23,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev23
};
static ROMSTAGE_CONST struct device _dev23 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,5)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev24,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev24
};
static ROMSTAGE_CONST struct device _dev24 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,6)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev25,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev25
};
static ROMSTAGE_CONST struct device _dev25 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1c,7)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev26,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev26
};
static ROMSTAGE_CONST struct device _dev26 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1d,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5006,
	.link_list = NULL,
	.sibling = &_dev27,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev27
};
static ROMSTAGE_CONST struct device _dev27 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1e,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev28,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev28
};
static ROMSTAGE_CONST struct device _dev28 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1f,0)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5001,
	.link_list = &_dev28_links[0],
	.sibling = &_dev41,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev30
};
ROMSTAGE_CONST struct bus _dev28_links[] = {
		[0] = {
			.link_num = 0,
			.dev = &_dev28,
			.children = &_dev30,
			.next = NULL,
		},
	};
static ROMSTAGE_CONST struct device _dev41 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1f,2)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0xb005,
	.link_list = NULL,
	.sibling = &_dev42,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev42
};
static ROMSTAGE_CONST struct device _dev42 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1f,3)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5001,
	.link_list = NULL,
	.sibling = &_dev43,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev43
};
static ROMSTAGE_CONST struct device _dev43 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1f,4)}}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev44,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
	.next=&_dev44
};
static ROMSTAGE_CONST struct device _dev44 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev7_links[0],
	.path = {.type=DEVICE_PATH_PCI,{.pci={ .devfn = PCI_DEVFN(0x1f,5)}}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
#ifndef __PRE_RAM__
	.chip_ops = &southbridge_intel_bd82x6x_ops,
#endif
	.chip_info = &southbridge_intel_bd82x6x_info_9,
};
static ROMSTAGE_CONST struct device _dev30 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x0 }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev31,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev31
};
static ROMSTAGE_CONST struct device _dev31 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x1 }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev32,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev32
};
static ROMSTAGE_CONST struct device _dev32 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x2 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.resource_list = &_dev32_res[0],
	.link_list = NULL,
	.sibling = &_dev33,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev33
};
ROMSTAGE_CONST struct resource _dev32_res[] = {
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IO, .index=0x60, .base=0x3f8,.next=&_dev32_res[1]},
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IRQ, .index=0x70, .base=0x4,.next=NULL },
	 };
static ROMSTAGE_CONST struct device _dev33 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x3 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.resource_list = &_dev33_res[0],
	.link_list = NULL,
	.sibling = &_dev34,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev34
};
ROMSTAGE_CONST struct resource _dev33_res[] = {
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IO, .index=0x60, .base=0x2f8,.next=&_dev33_res[1]},
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IRQ, .index=0x70, .base=0x3,.next=NULL },
	 };
static ROMSTAGE_CONST struct device _dev34 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x5 }}},
	.enabled = 1,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.resource_list = &_dev34_res[0],
	.link_list = NULL,
	.sibling = &_dev35,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev35
};
ROMSTAGE_CONST struct resource _dev34_res[] = {
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IO, .index=0x60, .base=0x60,.next=&_dev34_res[1]},
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IO, .index=0x62, .base=0x64,.next=&_dev34_res[2]},
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IRQ, .index=0x70, .base=0x1,.next=&_dev34_res[3]},
		{ .flags=IORESOURCE_FIXED | IORESOURCE_ASSIGNED | IORESOURCE_IRQ, .index=0x72, .base=0xc,.next=NULL },
	 };
static ROMSTAGE_CONST struct device _dev35 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x6 }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev36,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev36
};
static ROMSTAGE_CONST struct device _dev36 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0xa }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev37,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev37
};
static ROMSTAGE_CONST struct device _dev37 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0xb }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev38,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev38
};
static ROMSTAGE_CONST struct device _dev38 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0xd }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev39,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev39
};
static ROMSTAGE_CONST struct device _dev39 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0xe }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
	.sibling = &_dev40,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev40
};
static ROMSTAGE_CONST struct device _dev40 = {
#ifndef __PRE_RAM__
	.ops = 0,
#endif
	.bus = &_dev28_links[0],
	.path = {.type=DEVICE_PATH_PNP,{.pnp={ .port = 0x2e, .device = 0x16 }}},
	.enabled = 0,
	.on_mainboard = 1,
	.subsystem_vendor = 0x1458,
	.subsystem_device = 0x5000,
	.link_list = NULL,
#ifndef __PRE_RAM__
	.chip_ops = &superio_nuvoton_nct6776_ops,
#endif
	.next=&_dev41
};
