include config.mk

disk-speed: disk-speed.c
	${CC} ${CFLAGS} -o disk-speed disk-speed.c

install:
	@echo Installing to to ${PREFIX}/bin ...
	@mkdir -p ${PREFIX}/bin
	@cp -f disk-speed ${PREFIX}/bin
	@chmod 755 ${PREFIX}/bin/disk-speed

uninstall:
	@echo Uninstalling from ${PREFIX}/bin ...
	@rm -f ${PREFIX}/bin/disk-speed

clean:
	@rm -f disk-speed
