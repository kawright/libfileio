# ----- PROJECT INFO -----

PROJ_NAME := libfileio
VER := 2.1

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
	uninstall

all: build/lib/libfileio.so
	@echo Built ${PROJ_NAME} version ${VER}

clean:
	@rm -rf build
	@echo "All build artifacts removed"

install: build/lib/libfileio.so
	@cp fileio.h /usr/local/include/fileio.h
	@echo "Copied fileio.h"
	@cp build/lib/libfileio.so /usr/local/lib/libfileio.so
	@echo "Copied libfileio.so"
	@echo "${PROJ_NAME} version ${VER} successfully installed"

uninstall:
	@rm /usr/local/include/fileio.h
	@echo "Deleted fileio.h"
	@rm /usr/local/lib/libfileio.so
	@echo "Deleted libfileio.so"
	@echo "${PROJ_NAME} successfully uninstalled"

