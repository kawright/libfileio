# ----- PROJECT INFO -----

PROJ_NAME := libfileio
PROJ_DESC := "file input/output"
PROJ_VER := 2.3

# ----- COMPILER CONFIGURATION -----

CC := gcc
CC_OPTS := \
	-Wall \
	-Werror \
	-fpic \
	-g

CC_LIBS := \
	-lerr \
	-lmem

# ----- REAL TARGETS -----

.ver: build/lib/libfileio.so
	@ver -s -n ${PROJ_NAME} -d ${PROJ_DESC}
	@echo "Updated project metadata"
	
.__lib__:
	@touch .__lib__
	@echo "Touched library marker file"

build/lib/libfileio.so: build/obj/fileio.o
	@mkdir -p build/lib
	@${CC} -shared -o $@ $< ${CC_LIBS}
	@echo "Built target $@"

build/obj/fileio.o: fileio.c fileio.h Makefile
	@mkdir -p build/obj
	@${CC} ${CC_OPTS} -c -o $@ $<
	@echo "Built target $@"

# ----- PHONY TARGETS -----

.PHONY: \
	all \
	clean \
	install \
	uninstall \
	ver

all: build/lib/libfileio.so .ver .__lib__
	@echo "Built ${PROJ_NAME} version ${PROJ_VER} (build $$(ver -b))"
clean:
	@rm -rf build
	@echo "All build artifacts removed"

install: build/lib/libfileio.so
	@mkdir -p /usr/local/etc/libfileio
	
	@cp fileio.h /usr/local/include/fileio.h
	@echo "Copied fileio.h"
	
	@cp build/lib/libfileio.so /usr/local/lib/libfileio.so
	@echo "Copied libfileio.so"

	@cp .ver /usr/local/etc/libfileio/.ver
	@echo "Copied .ver"
	
	@cp .__lib__ /usr/local/etc/libfileio/.__lib__
	@echo "Copied .__lib__"
	
	@echo "${PROJ_NAME} version ${PROJ_VER} successfully installed"

uninstall:
	@rm /usr/local/include/fileio.h
	@echo "Deleted fileio.h"
	
	@rm /usr/local/lib/libfileio.so
	@echo "Deleted libfileio.so"
	
	@rm -rf /usr/local/etc/libfileio
	@echo "Deleted all configuration files"
	
	@echo "${PROJ_NAME} successfully uninstalled"

ver:
	@ver -V ${PROJ_VER}
	@echo "Updated version number to ${PROJ_VER}"
