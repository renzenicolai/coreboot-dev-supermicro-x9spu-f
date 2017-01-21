deps_config := \
	vgasrc/Kconfig \
	/home/renze/coreboot/payloads/external/SeaBIOS/seabios/src/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
