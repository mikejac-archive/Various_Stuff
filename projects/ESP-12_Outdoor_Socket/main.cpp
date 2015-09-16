/* 
 * ESP8266 FreeRTOS Firmware
 * Copyright (C) 2015  Michael Jacobsen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * https://github.com/mikejac/
 * 
 */

#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
#include <com/tholusi/esp-open-rtos/date_time/system_time.h>

#include "espressif/esp_common.h"
#include "espressif/sdk_private.h"

#include <string.h>

#include "wifi.hpp"         // static wifi credentials - create your own :-)

#include "task_mqtt.hpp"
#include "task_pub.hpp"
#include "io.hpp"

#define DTXT(...)   printf(__VA_ARGS__)

extern "C" pid_t _getpid_r(void)
{
    //DTXT("_getpid_r()\n");
    return 0;
}

extern "C" int _kill_r(pid_t pid, int sig)
{
    //DTXT("_kill_r()\n");
    return 0;
}

extern "C" void _exit(int status)
{
    DTXT("_exit()\n");
    
    while(true) {
        sdk_os_delay_us(1000);
    }
}

/*void loghexdump(const void *pAddressIn, long lSize)
{
    char szBuf[100];
    long lIndent = 1;
    long lOutLen, lIndex, lIndex2, lOutLen2;
    long lRelPos;
    struct { char *pData; unsigned long lSize; } buf;
    unsigned char *pTmp,ucTmp;
    unsigned char *pAddress = (unsigned char *)pAddressIn;

    buf.pData   = (char *)pAddress;
    buf.lSize   = lSize;

    while (buf.lSize > 0) {
        pTmp     = (unsigned char *)buf.pData;
        lOutLen  = (int)buf.lSize;

        if (lOutLen > 16)
            lOutLen = 16;

        // create a 64-character formatted output line:
        sprintf(szBuf,  " >                            "
                        "                      "
                        "    %08X", pTmp-pAddress);

        lOutLen2 = lOutLen;

        for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
            lOutLen2;
            lOutLen2--, lIndex += 2, lIndex2++
        )

        {
            ucTmp = *pTmp++;

            sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
            
            if(ucTmp < 0x20 || ucTmp > 0x7E) {
                ucTmp = '.'; // nonprintable char
            }
            
            szBuf[lIndex2] = ucTmp;

            if (!(++lRelPos & 3))     // extra blank after 4 bytes
                {  lIndex++; szBuf[lIndex+2] = ' '; }
        }

        if (!(lRelPos & 3)) lIndex--;

        szBuf[lIndex  ]   = '<';
        szBuf[lIndex+1]   = ' ';

        printf("%s\n", szBuf);

        buf.pData   += lOutLen;
        buf.lSize   -= lOutLen;
    }
}*/

//#define BROKER      "test.mosquitto.org"
#define BROKER      "192.168.1.82"

// queue used between MyMqtt task and MqttPub task
esp_open_rtos::mqtt::mqtt_payload_recv_t::mqtt_queue_t  MqttQueue;

MyMqtt                                                  mqtt_client1;
MqttPub                                                 mqtt_pub;

MyGpio                                                  gpio_proj;
MyGpio_t                                                gpio(gpio_proj);

/**
 * 
 */
extern "C" void user_init(void)
{
    sdk_uart_div_modify(0, UART_CLK_FREQ / 115200);
    
    // give the UART some time to settle
    sdk_os_delay_us(500);
    
    printf("\n\n");
    printf("*****\n");
    printf("START ESP-12 Outdoor Socket\n");
    printf("*****\n");
    printf("SDK version : %s\n\n", sdk_system_get_sdk_version());
    
    // get our 1 second clock running
    esp_start_system_time();

    if(mqtt_pub.m_Mqtt_payload.init(5) != 0) {
        DTXT("main(): mqtt_pub.m_Mqtt_payload.init(4) != 0\n");
    }
    if(wifi_global.init(WIFI_SSID, WIFI_PASS) != 0) {
        DTXT("main(): wifi_global.init(WIFI_SSID, WIFI_PASS) != 0\n");
    }
    else if(wifi_global.task_create("wifi") != 0) {
        DTXT("main(): wifi_global.task_create(wifi) != 0\n");
    }
    else if(mqtt_client1.init(mqtt_pub.m_Mqtt_payload.mqtt_queue(), wifi_global, BROKER) != 0) {
        DTXT("main(): mqtt_client1.init() != 0\n");
    }
    else if(mqtt_client1.task_create("MQTT_task_1") != 0) {
        DTXT("main(): mqtt_client1.task_create(MQTT_task_1) != 0\n");
    }
    else if(mqtt_pub.init(mqtt_client1) != 0) {
        DTXT("main(): mqtt_pub.init(mqtt_client1) != 0\n");
    }
    else if(mqtt_pub.task_create("mqtt_pub_1") != 0) {
        DTXT("main(): mqtt_pub.task_create(mqtt_pub_1) != 0\n");
    }
}
