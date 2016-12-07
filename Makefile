# Compatible with GNU make and BSD make.

include config.mk

install:
	@echo Installing to to ${PREFIX}/bin ...
	@mkdir -p ${PREFIX}/bin
	@cp -f disk-speed.sh ${PREFIX}/bin/disk-speed
	@chmod 755 ${PREFIX}/bin/disk-speed

uninstall:
	@echo Uninstalling from ${PREFIX}/bin ...
	@rm -f ${PREFIX}/bin/disk-speed
