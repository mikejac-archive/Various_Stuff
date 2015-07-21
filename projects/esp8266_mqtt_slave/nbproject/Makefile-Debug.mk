#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=xtensa-lx106-elf-gcc
CCC=xtensa-lx106-elf-cpp
CXX=xtensa-lx106-elf-cpp
FC=gfortran
AS=xtensa-lx106-elf-as

# Macros
CND_PLATFORM=GNU_ESP8266-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/818053526/uart.o \
	${OBJECTDIR}/_ext/220865800/mqtt_msg.o \
	${OBJECTDIR}/_ext/70144565/utils.o \
	${OBJECTDIR}/_ext/517527575/AllocatorIntf.o \
	${OBJECTDIR}/_ext/517527575/BaAtoi.o \
	${OBJECTDIR}/_ext/517527575/BufPrint.o \
	${OBJECTDIR}/_ext/517527575/JDecoder.o \
	${OBJECTDIR}/_ext/517527575/JEncoder.o \
	${OBJECTDIR}/_ext/517527575/JParser.o \
	${OBJECTDIR}/_ext/517527575/JVal.o \
	${OBJECTDIR}/_ext/517527575/atof.o \
	${OBJECTDIR}/_ext/581867209/cplusplus.o \
	${OBJECTDIR}/_ext/1786407325/json_rpc_t.o \
	${OBJECTDIR}/_ext/1780124954/mqtt_client_t.o \
	${OBJECTDIR}/_ext/1780124954/mqtt_handler_t.o \
	${OBJECTDIR}/_ext/55020571/task_t.o \
	${OBJECTDIR}/_ext/54923915/wifi_t.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_mqtt_slave

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_mqtt_slave: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_mqtt_slave ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/818053526/uart.o: ../../shared/com/espressif/esp8266/uart/uart.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/818053526
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/818053526/uart.o ../../shared/com/espressif/esp8266/uart/uart.c

${OBJECTDIR}/_ext/220865800/mqtt_msg.o: ../../shared/com/github/esar/contiki-mqtt/mqtt_msg.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/220865800
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/220865800/mqtt_msg.o ../../shared/com/github/esar/contiki-mqtt/mqtt_msg.c

${OBJECTDIR}/_ext/70144565/utils.o: ../../shared/com/github/taunpmt/esp_mqtt/utils/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/70144565
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/70144565/utils.o ../../shared/com/github/taunpmt/esp_mqtt/utils/utils.cpp

${OBJECTDIR}/_ext/517527575/AllocatorIntf.o: ../../shared/com/realtimelogic/json/esp8266/AllocatorIntf.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/AllocatorIntf.o ../../shared/com/realtimelogic/json/esp8266/AllocatorIntf.c

${OBJECTDIR}/_ext/517527575/BaAtoi.o: ../../shared/com/realtimelogic/json/esp8266/BaAtoi.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/BaAtoi.o ../../shared/com/realtimelogic/json/esp8266/BaAtoi.c

${OBJECTDIR}/_ext/517527575/BufPrint.o: ../../shared/com/realtimelogic/json/esp8266/BufPrint.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/BufPrint.o ../../shared/com/realtimelogic/json/esp8266/BufPrint.c

${OBJECTDIR}/_ext/517527575/JDecoder.o: ../../shared/com/realtimelogic/json/esp8266/JDecoder.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/JDecoder.o ../../shared/com/realtimelogic/json/esp8266/JDecoder.c

${OBJECTDIR}/_ext/517527575/JEncoder.o: ../../shared/com/realtimelogic/json/esp8266/JEncoder.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/JEncoder.o ../../shared/com/realtimelogic/json/esp8266/JEncoder.c

${OBJECTDIR}/_ext/517527575/JParser.o: ../../shared/com/realtimelogic/json/esp8266/JParser.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/JParser.o ../../shared/com/realtimelogic/json/esp8266/JParser.c

${OBJECTDIR}/_ext/517527575/JVal.o: ../../shared/com/realtimelogic/json/esp8266/JVal.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/JVal.o ../../shared/com/realtimelogic/json/esp8266/JVal.c

${OBJECTDIR}/_ext/517527575/atof.o: ../../shared/com/realtimelogic/json/esp8266/atof.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/517527575
	${RM} "$@.d"
	$(COMPILE.c) -g -DICACHE_FLASH -D__ets__ -include ../../shared/com/espressif/esp8266/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/517527575/atof.o ../../shared/com/realtimelogic/json/esp8266/atof.c

${OBJECTDIR}/_ext/581867209/cplusplus.o: ../../shared/com/tholusi/esp8266/cplusplus/cplusplus.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/581867209
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/581867209/cplusplus.o ../../shared/com/tholusi/esp8266/cplusplus/cplusplus.cpp

${OBJECTDIR}/_ext/1786407325/json_rpc_t.o: ../../shared/com/tholusi/esp8266/jsonrpc/json_rpc_t.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1786407325
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1786407325/json_rpc_t.o ../../shared/com/tholusi/esp8266/jsonrpc/json_rpc_t.cpp

${OBJECTDIR}/_ext/1780124954/mqtt_client_t.o: ../../shared/com/tholusi/esp8266/mqtt_client/mqtt_client_t.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1780124954
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1780124954/mqtt_client_t.o ../../shared/com/tholusi/esp8266/mqtt_client/mqtt_client_t.cpp

${OBJECTDIR}/_ext/1780124954/mqtt_handler_t.o: ../../shared/com/tholusi/esp8266/mqtt_client/mqtt_handler_t.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1780124954
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1780124954/mqtt_handler_t.o ../../shared/com/tholusi/esp8266/mqtt_client/mqtt_handler_t.cpp

${OBJECTDIR}/_ext/55020571/task_t.o: ../../shared/com/tholusi/esp8266/task/task_t.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/55020571
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/55020571/task_t.o ../../shared/com/tholusi/esp8266/task/task_t.cpp

${OBJECTDIR}/_ext/54923915/wifi_t.o: ../../shared/com/tholusi/esp8266/wifi/wifi_t.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/54923915
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/54923915/wifi_t.o ../../shared/com/tholusi/esp8266/wifi/wifi_t.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_mqtt_slave

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
