# 
# ESP8266 FreeRTOS Firmware
# Copyright (C) 2015  Michael Jacobsen
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# https://github.com/mikejac/
# 
#

#
#  There exist several targets which are by default empty and which can be 
#  used for execution of your targets. These targets are usually executed 
#  before and after some main targets. They are: 
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#  
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help mesage
#  
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in nbproject/makefile-impl.mk.
#
#  Available make variables:
#
#     CND_BASEDIR                base directory for relative paths
#     CND_DISTDIR                default top distribution directory (build artifacts)
#     CND_BUILDDIR               default top build directory (object files, ...)
#     CONF                       name of current configuration
#     CND_PLATFORM_${CONF}       platform name (current configuration)
#     CND_ARTIFACT_DIR_${CONF}   directory of build artifact (current configuration)
#     CND_ARTIFACT_NAME_${CONF}  name of build artifact (current configuration)
#     CND_ARTIFACT_PATH_${CONF}  path to build artifact (current configuration)
#     CND_PACKAGE_DIR_${CONF}    directory of package (current configuration)
#     CND_PACKAGE_NAME_${CONF}   name of package (current configuration)
#     CND_PACKAGE_PATH_${CONF}   path to package (current configuration)
#
# NOCDDL

#
# sudo hdiutil mount ~/Documents/case-sensitive.dmg
#

# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin

ifeq ($(OS),Windows_NT)
#    CCFLAGS += -D WIN32
#    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
#        CCFLAGS += -D AMD64
#    endif
#    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
#        CCFLAGS += -D IA32
#    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        XTENSA_BASE = /opt/esp-open-sdk/xtensa-lx106-elf
        ROOT        = /opt/esp-open-rtos
        ESPTOOL     = /opt/esptool/esptool.py
        ESPPORT     = /dev/ttyUSB0
    endif
    ifeq ($(UNAME_S),Darwin)
        XTENSA_BASE = /Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf
        ROOT        = /Volumes/case-sensitive/esp-open-rtos
        ESPTOOL     = /Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf/bin/esptool.py
        ESPPORT     = /dev/tty.wchusbserial1410
    endif
#    UNAME_P := $(shell uname -p)
#    ifeq ($(UNAME_P),x86_64)
#        CCFLAGS += -D AMD64
#    endif
#    ifneq ($(filter %86,$(UNAME_P)),)
#        CCFLAGS += -D IA32
#    endif
#    ifneq ($(filter arm%,$(UNAME_P)),)
#        CCFLAGS += -D ARM
#    endif
endif


XTENSA_BIN     = ${XTENSA_BASE}/bin
SIZE           = ${XTENSA_BIN}/xtensa-lx106-elf-size

PROGRAM_DIR    = ${ROOT}/examples/simple_cplusplus
BUILD_DIR      = ${PROGRAM_DIR}/build
LD_SCRIPT1     = ${BUILD_DIR}/ld-16/eagle.app.v6.ld
LD_SCRIPT2     = ${BUILD_DIR}/ld-16/eagle.rom.addr.v6.ld
MAP_FILE       = ${CND_ARTIFACT_PATH_${CONF}}.map
ENTRY_SYMBOL   = call_user_start
LIB_GROUP      = ${XTENSA_BASE}/xtensa-lx106-elf/sysroot/usr/lib/libhal.a ${XTENSA_BASE}/lib/gcc/xtensa-lx106-elf/4.8.2/libgcc.a ${ROOT}/libc/xtensa-lx106-elf/lib/libc.a -lmain -lnet80211 -lphy -lpp -lwpa ${BUILD_DIR}/core.a ${BUILD_DIR}/freertos.a ${BUILD_DIR}/lwip.a ${BUILD_DIR}/axtls.a

# esptool defaults
ESPBAUD        = 230400

# we create two different files for uploading into the flash
FW_1           = 0x00000
FW_2           = 0x40000
FW_FILE_1      = ${FW_1}.bin
FW_FILE_2      = ${FW_2}.bin

# build
build: .build-post

.build-pre:
# Add your pre 'build' code here...

.build-post: .build-impl
# Add your post 'build' code here...
	${SIZE} --format=sysv ${CND_ARTIFACT_PATH_${CONF}}
	${ESPTOOL} elf2image ${CND_ARTIFACT_PATH_${CONF}} -o ${CND_ARTIFACT_DIR_${CONF}}/
	${ESPTOOL} -p ${ESPPORT} --baud $(ESPBAUD) write_flash ${FW_2} ${CND_ARTIFACT_DIR_${CONF}}/${FW_FILE_2} ${FW_1} ${CND_ARTIFACT_DIR_${CONF}}/${FW_FILE_1}

# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...

.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre: build-tests
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...



# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
