# ----- PROJECT INFO ----- 

PROJ_NAME := fileio
PROJ_DESC := "file input output"
PROJ_VER := 2.4

# ----- COMPILER CONFIGURATION -----

CC := gcc
CC_OPTS := \
	-Wall \
	-Werror \
	-fpic \
	# -g \
    # -DDEBUG_MODE
CC_LIBS := \
	-lerr \
	-lmem

# ----- REAL TARGETS -----

.ver: build/lib/lib${PROJ_NAME}.so
	@ver -s -n lib${PROJ_NAME} -d ${PROJ_DESC}
	@echo "Updated project metadata"
	
.__lib__:
	@touch .__lib__
	@echo "Updated library marker file"

build/lib/lib${PROJ_NAME}.so: build/obj/${PROJ_NAME}.o
	@mkdir -p build/lib
	@${CC} -shared -o $@ $< ${CC_LIBS}
	@echo "Built target $@"

build/obj/${PROJ_NAME}.o: ${PROJ_NAME}.c ${PROJ_NAME}.h Makefile
	@mkdir -p build/obj
	@${CC} ${CC_OPTS} -c -o $@ $<
	@echo "Built target $@"

# ----- PHONY TARGETS -----

.PHONY: \
	all \
	clean \
    init \
    init-git \
	install \
	uninstall \
	ver

all: build/lib/lib${PROJ_NAME}.so .ver .__lib__
	@echo "Built lib${PROJ_NAME} version ${PROJ_VER} (build $$(ver -b))"

clean:
	@rm -rf build
	@echo "All build artifacts removed"

init:
	@touch ${PROJ_NAME}.c
	@echo "Created ${PROJ_NAME}.c"
	@touch ${PROJ_NAME}.h
	@echo "Created ${PROJ_NAME}.h"
	@touch .gitignore
	@echo "Created .gitignore"
	@touch .__lib__
	@echo "Created .__lib__"
	@echo "Project successfully initialized"

init-git:
	@git init
	@echo "Initialized empty git repository"
	@git add --all
	@echo "Added all existing files to the repository"
	@git commit -m "v${PROJ_VER}; init commit"
	@echo "Created first commit"
	@git branch -M main
	@echo "Set the primary branch to 'main'"
	@git remote add origin git@github.com:kawright/${PROJ_NAME}.git
	@echo "Linked repository to remote GitHub repository"
	@git push -u origin main
	@echo "Pushed first commit to remote repository"
	@echo "Git repository successfully initialized"

install: build/lib/lib${PROJ_NAME}.so
	@mkdir -p /usr/local/etc/lib${PROJ_NAME}	
	@cp ${PROJ_NAME}.h /usr/local/include/${PROJ_NAME}.h
	@echo "Copied fileio.h"
	@cp build/lib/lib${PROJ_NAME}.so /usr/local/lib/lib${PROJ_NAME}.so
	@echo "Copied lib${PROJ_NAME}.so"
	@cp .ver /usr/local/etc/lib${PROJ_NAME}/.ver
	@echo "Copied .ver"
	@cp .__lib__ /usr/local/etc/lib${PROJ_NAME}/.__lib__
	@echo "Copied .__lib__"
	@echo "lib${PROJ_NAME} version ${PROJ_VER} successfully installed"

uninstall:
	@rm /usr/local/include/${PROJ_NAME}.h
	@echo "Deleted ${PROJ_NAME}.h"
	@rm /usr/local/lib/lib${PROJ_NAME}.so
	@echo "Deleted lib${PROJ_NAME}.so"
	@rm -rf /usr/local/etc/lib${PROJ_NAME}
	@echo "Deleted all configuration files"
	@echo "lib${PROJ_NAME} successfully uninstalled"

ver:
	@ver -V ${PROJ_VER}
	@echo "Updated version number to ${PROJ_VER}"

