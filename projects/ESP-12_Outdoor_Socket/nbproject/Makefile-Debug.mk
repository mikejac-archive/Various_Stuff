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
CCC=xtensa-lx106-elf-g++
CXX=xtensa-lx106-elf-g++
FC=gfortran
AS=xtensa-lx106-elf-as

# Macros
CND_PLATFORM=GNU_ESP_Open_SDK-Linux-x86
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
	${OBJECTDIR}/_ext/1108374663/cplusplus.o \
	${OBJECTDIR}/_ext/2029485570/mqtt_client.o \
	${OBJECTDIR}/_ext/1318104846/MQTTConnectClient.o \
	${OBJECTDIR}/_ext/1318104846/MQTTConnectServer.o \
	${OBJECTDIR}/_ext/1318104846/MQTTDeserializePublish.o \
	${OBJECTDIR}/_ext/1318104846/MQTTFormat.o \
	${OBJECTDIR}/_ext/1318104846/MQTTPacket.o \
	${OBJECTDIR}/_ext/1318104846/MQTTSerializePublish.o \
	${OBJECTDIR}/_ext/1318104846/MQTTSubscribeClient.o \
	${OBJECTDIR}/_ext/1318104846/MQTTSubscribeServer.o \
	${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeClient.o \
	${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeServer.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_freertos_test1

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_freertos_test1: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_freertos_test1 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1108374663/cplusplus.o: ../../shared/com/tholusi/esp-open-rtos/cplusplus/cplusplus.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1108374663
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1108374663/cplusplus.o ../../shared/com/tholusi/esp-open-rtos/cplusplus/cplusplus.cpp

${OBJECTDIR}/_ext/2029485570/mqtt_client.o: ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_client.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029485570
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029485570/mqtt_client.o ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_client.cpp

${OBJECTDIR}/_ext/1318104846/MQTTConnectClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTConnectClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectClient.c

${OBJECTDIR}/_ext/1318104846/MQTTConnectServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTConnectServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectServer.c

${OBJECTDIR}/_ext/1318104846/MQTTDeserializePublish.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTDeserializePublish.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTDeserializePublish.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTDeserializePublish.c

${OBJECTDIR}/_ext/1318104846/MQTTFormat.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTFormat.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTFormat.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTFormat.c

${OBJECTDIR}/_ext/1318104846/MQTTPacket.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTPacket.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTPacket.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTPacket.c

${OBJECTDIR}/_ext/1318104846/MQTTSerializePublish.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSerializePublish.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSerializePublish.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSerializePublish.c

${OBJECTDIR}/_ext/1318104846/MQTTSubscribeClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSubscribeClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeClient.c

${OBJECTDIR}/_ext/1318104846/MQTTSubscribeServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSubscribeServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeServer.c

${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c

${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/esp8266_freertos_test1

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
