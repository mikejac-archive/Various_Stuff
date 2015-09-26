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
CND_PLATFORM=GNU_ESP_Open_SDK_RTOS-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1995282567/dhcpserver.o \
	${OBJECTDIR}/_ext/328798101/AllocatorIntf.o \
	${OBJECTDIR}/_ext/328798101/BaAtoi.o \
	${OBJECTDIR}/_ext/328798101/BufPrint.o \
	${OBJECTDIR}/_ext/328798101/JDecoder.o \
	${OBJECTDIR}/_ext/328798101/JEncoder.o \
	${OBJECTDIR}/_ext/328798101/JParser.o \
	${OBJECTDIR}/_ext/328798101/JVal.o \
	${OBJECTDIR}/_ext/959539040/system_time.o \
	${OBJECTDIR}/_ext/2029485570/mqtt_bluemix.o \
	${OBJECTDIR}/_ext/2029485570/mqtt_client.o \
	${OBJECTDIR}/_ext/2029485570/mqtt_fabric.o \
	${OBJECTDIR}/_ext/1450941183/mdns.o \
	${OBJECTDIR}/_ext/2029775347/wifi.o \
	${OBJECTDIR}/_ext/2029775347/wifi_smartlink.o \
	${OBJECTDIR}/_ext/2029775347/wifi_smartweb.o \
	${OBJECTDIR}/_ext/1338027254/mdns.o \
	${OBJECTDIR}/_ext/1338027254/mdnsd.o \
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
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/task_mqtt.o \
	${OBJECTDIR}/task_pub.o


# C Compiler Flags
CFLAGS=-Wall -Werror -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals -std=gnu99 -g -O2

# CC Compiler Flags
CCFLAGS=-Wall -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals -g -fno-exceptions -fno-rtti
CXXFLAGS=-Wall -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals -g -fno-exceptions -fno-rtti

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ESP-12_Outdoor_Socket

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ESP-12_Outdoor_Socket: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	xtensa-lx106-elf-gcc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ESP-12_Outdoor_Socket ${OBJECTFILES} ${LDLIBSOPTIONS} -Wl,--no-check-sections -L${BUILD_DIR}/sdklib -nostdlib -fno-rtti -g -O2 -Wl,-EL -T${LD_SCRIPT1} -T${LD_SCRIPT2} -u ${ENTRY_SYMBOL} -Wl,-static -Wl,-Map=${MAP_FILE} -Wl,--start-group ${LIB_GROUP} -Wl,--end-group

${OBJECTDIR}/_ext/1995282567/dhcpserver.o: ../../shared/com/github/esp-open-rtos/dhcpserver/dhcpserver.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1995282567
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1995282567/dhcpserver.o ../../shared/com/github/esp-open-rtos/dhcpserver/dhcpserver.c

${OBJECTDIR}/_ext/328798101/AllocatorIntf.o: ../../shared/com/realtimelogic/json/esp_open_rtos/AllocatorIntf.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/AllocatorIntf.o ../../shared/com/realtimelogic/json/esp_open_rtos/AllocatorIntf.c

${OBJECTDIR}/_ext/328798101/BaAtoi.o: ../../shared/com/realtimelogic/json/esp_open_rtos/BaAtoi.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/BaAtoi.o ../../shared/com/realtimelogic/json/esp_open_rtos/BaAtoi.c

${OBJECTDIR}/_ext/328798101/BufPrint.o: ../../shared/com/realtimelogic/json/esp_open_rtos/BufPrint.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/BufPrint.o ../../shared/com/realtimelogic/json/esp_open_rtos/BufPrint.c

${OBJECTDIR}/_ext/328798101/JDecoder.o: ../../shared/com/realtimelogic/json/esp_open_rtos/JDecoder.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/JDecoder.o ../../shared/com/realtimelogic/json/esp_open_rtos/JDecoder.c

${OBJECTDIR}/_ext/328798101/JEncoder.o: ../../shared/com/realtimelogic/json/esp_open_rtos/JEncoder.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/JEncoder.o ../../shared/com/realtimelogic/json/esp_open_rtos/JEncoder.c

${OBJECTDIR}/_ext/328798101/JParser.o: ../../shared/com/realtimelogic/json/esp_open_rtos/JParser.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/JParser.o ../../shared/com/realtimelogic/json/esp_open_rtos/JParser.c

${OBJECTDIR}/_ext/328798101/JVal.o: ../../shared/com/realtimelogic/json/esp_open_rtos/JVal.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/328798101
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/328798101/JVal.o ../../shared/com/realtimelogic/json/esp_open_rtos/JVal.c

${OBJECTDIR}/_ext/959539040/system_time.o: ../../shared/com/tholusi/esp-open-rtos/date_time/system_time.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/959539040
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/959539040/system_time.o ../../shared/com/tholusi/esp-open-rtos/date_time/system_time.c

${OBJECTDIR}/_ext/2029485570/mqtt_bluemix.o: ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_bluemix.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029485570
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029485570/mqtt_bluemix.o ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_bluemix.cpp

${OBJECTDIR}/_ext/2029485570/mqtt_client.o: ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_client.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029485570
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029485570/mqtt_client.o ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_client.cpp

${OBJECTDIR}/_ext/2029485570/mqtt_fabric.o: ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_fabric.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029485570
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029485570/mqtt_fabric.o ../../shared/com/tholusi/esp-open-rtos/mqtt/mqtt_fabric.cpp

${OBJECTDIR}/_ext/1450941183/mdns.o: ../../shared/com/tholusi/esp-open-rtos/net/mdns.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1450941183
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1450941183/mdns.o ../../shared/com/tholusi/esp-open-rtos/net/mdns.cpp

${OBJECTDIR}/_ext/2029775347/wifi.o: ../../shared/com/tholusi/esp-open-rtos/wifi/wifi.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029775347
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029775347/wifi.o ../../shared/com/tholusi/esp-open-rtos/wifi/wifi.cpp

${OBJECTDIR}/_ext/2029775347/wifi_smartlink.o: ../../shared/com/tholusi/esp-open-rtos/wifi/wifi_smartlink.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029775347
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029775347/wifi_smartlink.o ../../shared/com/tholusi/esp-open-rtos/wifi/wifi_smartlink.cpp

${OBJECTDIR}/_ext/2029775347/wifi_smartweb.o: ../../shared/com/tholusi/esp-open-rtos/wifi/wifi_smartweb.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2029775347
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/2029775347/wifi_smartweb.o ../../shared/com/tholusi/esp-open-rtos/wifi/wifi_smartweb.cpp

${OBJECTDIR}/_ext/1338027254/mdns.o: ../../shared/org/bitbucket/geekman/tinysvcmdns/mdns.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1338027254
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1338027254/mdns.o ../../shared/org/bitbucket/geekman/tinysvcmdns/mdns.c

${OBJECTDIR}/_ext/1338027254/mdnsd.o: ../../shared/org/bitbucket/geekman/tinysvcmdns/mdnsd.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1338027254
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1338027254/mdnsd.o ../../shared/org/bitbucket/geekman/tinysvcmdns/mdnsd.c

${OBJECTDIR}/_ext/1318104846/MQTTConnectClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTConnectClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectClient.c

${OBJECTDIR}/_ext/1318104846/MQTTConnectServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTConnectServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTConnectServer.c

${OBJECTDIR}/_ext/1318104846/MQTTDeserializePublish.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTDeserializePublish.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTDeserializePublish.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTDeserializePublish.c

${OBJECTDIR}/_ext/1318104846/MQTTFormat.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTFormat.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTFormat.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTFormat.c

${OBJECTDIR}/_ext/1318104846/MQTTPacket.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTPacket.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTPacket.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTPacket.c

${OBJECTDIR}/_ext/1318104846/MQTTSerializePublish.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSerializePublish.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSerializePublish.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSerializePublish.c

${OBJECTDIR}/_ext/1318104846/MQTTSubscribeClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSubscribeClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeClient.c

${OBJECTDIR}/_ext/1318104846/MQTTSubscribeServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTSubscribeServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTSubscribeServer.c

${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeClient.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeClient.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c

${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeServer.o: ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1318104846
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1318104846/MQTTUnsubscribeServer.o ../../shared/org/eclipse/paho/mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/task_mqtt.o: task_mqtt.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/task_mqtt.o task_mqtt.cpp

${OBJECTDIR}/task_pub.o: task_pub.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DMQTT_WITH_LWT -DREMOTE_BUILD -I../../shared -I../../shared/org/eclipse/paho/mqtt/MQTTPacket/src -I/Volumes/case-sensitive/esp-open-rtos/include -I/Volumes/case-sensitive/esp-open-rtos/libc/xtensa-lx106-elf/include -I/Volumes/case-sensitive/esp-open-rtos/core/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/include -I/Volumes/case-sensitive/esp-open-rtos/FreeRTOS/Source/portable/esp8266 -I/Volumes/case-sensitive/esp-open-rtos/lwip/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4 -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/ipv4/lwip -I/Volumes/case-sensitive/esp-open-rtos/lwip/lwip/src/include/lwip -I/Volumes/case-sensitive/esp-open-rtos/axtls/include -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/ssl -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/crypto -I/Volumes/case-sensitive/esp-open-rtos/axtls/axtls/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/task_pub.o task_pub.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ESP-12_Outdoor_Socket

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
