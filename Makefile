SDK_DIR ?= sdk
VERSION ?= vdev
DEVICE ?= /dev/ttyUSB1

CFLAGS += -D'VERSION="${VERSION}"'

-include sdk/Makefile.mk

.PHONY: all
all: debug

.PHONY: flash
flash: debug
	bcf -d ${DEVICE} flash firmware.bin

.PHONY: sdk
sdk: sdk/Makefile.mk

.PHONY: update
update:
	@git submodule update --remote --merge sdk

sdk/Makefile.mk:
	@git submodule update --init sdk
