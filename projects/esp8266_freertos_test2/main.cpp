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

#include "ssl.h"

#include "wifi.hpp"         // static wifi credentials - create your own :-)

#include "task_mqtt.hpp"
#include "task_pub.hpp"
#include "task_mdns.hpp"

#define DTXT(...)   printf(__VA_ARGS__)

namespace esp_open_rtos {
namespace io {

const int MAX_GPIO = 16;

#define GPIO_initializer { GPIO_INPUT_PULLUP, 0 }

/******************************************************************************************************************
 * gpio_t
 *
 */
template<class Valid>
class gpio_t
{
public:
    /**
     * 
     * @param valid
     */
    gpio_t(Valid& valid)
    {
        m_Valid = valid;
        
        for(int i = 0; i < MAX_GPIO + 1; ++i) {
            m_GpioStat[i] = GPIO_initializer;
        }
    }
    /**
     * 
     * @param gpio_num
     * @param direction
     * @return 
     */
    int enable(uint8_t gpio_num, gpio_direction_t direction)
    {
        if(m_Valid.pinIsValid(gpio_num) == 0) {
            m_GpioStat[gpio_num].gpio_direction = direction;
            
            gpio_enable(gpio_num, direction);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int disable(uint8_t gpio_num)
    {
        if(m_Valid.pinIsValid(gpio_num) == 0) {
            m_GpioStat[gpio_num].gpio_direction = (gpio_direction_t) 0xFF;
            
            gpio_disable(gpio_num);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @param set
     * @return 
     */
    int write(uint8_t gpio_num, bool set)
    {
        if(m_Valid.pinIsValid(gpio_num) == 0 && (m_GpioStat[gpio_num].gpio_direction == GPIO_OUTPUT || m_GpioStat[gpio_num].gpio_direction == GPIO_OUT_OPEN_DRAIN)) {
            gpio_write(gpio_num, set);

            // record when it was changed
            m_GpioStat[gpio_num].millis = millis();
        }
        else {
            return -1;
        }

        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int read(uint8_t gpio_num)
    {
        if(m_Valid.pinIsValid(gpio_num) == 0) {
            // record when it was read
            m_GpioStat[gpio_num].millis = millis();
            
            return gpio_read(gpio_num);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int toggle(uint8_t gpio_num)
    {
        if(m_Valid.pinIsValid(gpio_num) == 0 && (m_GpioStat[gpio_num].gpio_direction == GPIO_OUTPUT || m_GpioStat[gpio_num].gpio_direction == GPIO_OUT_OPEN_DRAIN)) {
            gpio_toggle(gpio_num);
            
            // record when it was changed
            m_GpioStat[gpio_num].millis = millis();
        }
        else {
            return -1;
        }
        
        return 0;
    }
    
protected:
    Valid& m_Valid;
    
    typedef struct {
        gpio_direction_t    gpio_direction;
        unsigned long       millis;
    } gpio_stat_t;
    
    gpio_stat_t m_GpioStat[MAX_GPIO + 1];
};

}
}

class gpio_valid_t
{
public:
    /**
     * 
     * @param gpio_num
     * @return 
     */
    bool pinIsValid(uint8_t gpio_num)
    {
        switch(gpio_num) {
            case 0:     return true;
            case 1:     return true;
            case 2:     return false;
            case 3:     return false;
            case 4:     return true;
            case 5:     return true;
            case 6:     return false;
            case 7:     return false;
            case 8:     return false;
            case 9:     return false;
            case 10:    return false;
            case 11:    return false;
            case 12:    return true;
            case 13:    return true;
            case 14:    return true;
            case 15:    return true;
            case 16:    return true;
            default:    return false;
        }
    }
};

esp_open_rtos::io::gpio_t<gpio_valid_t> gpio(gpio_valid_t);


// queue used between MyMqtt task and MqttPub task
esp_open_rtos::mqtt::mqtt_payload_recv_t::mqtt_queue_t  MqttQueue;

//MyMqtt                                                  mqtt_client1;
//MyMqtt                                                  mqtt_client2;
MqttPub                                                 mqtt_pub;
//Mdns                                                    mdns;


struct sdk_station_config       m_Config;

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
    printf("START\n");
    printf("*****\n");
    printf("SDK version : %s\n\n", sdk_system_get_sdk_version());
    
    // get our 1 second clock running
    esp_start_system_time();

    if(mqtt_pub.m_Mqtt_payload.init(4) != 0) {
        printf("main(): mqtt_pub.m_Mqtt_payload.init(4) != 0\n");
    }
#if defined(WITH_SMARTLINK)
    if(wifi_global.init(esp_open_rtos::wifi::wifi_t::mode_smartlink) != 0) {
        printf("main(): wifi_global.init() != 0\n");
    }
//#elif defined(WITH_SMARTWEB)
//    if(wifi_global.init() != 0) {
//        printf("main(): wifi_global.init() != 0\n");
//    }
#else
    if(wifi_global.init(WIFI_SSID, WIFI_PASS) != 0) {
        printf("main(): wifi_global.init(WIFI_SSID, WIFI_PASS) != 0\n");
    }
#endif
    else if(wifi_global.task_create("wifi") != 0) {
        printf("main(): wifi_global.task_create(wifi) != 0\n");
    }
    //else if(mdns.init(wifi_global) != 0) {
    //    printf("main(): mdns.init(wifi_global) != 0\n");
    //}
    //else if(mdns.task_create("mdns_task") != 0) {
    //    printf("main(): mdns.task_create(mdns_task) != 0\n");
    //}
    //else if(mqtt_client1.init(mqtt_pub.m_Mqtt_payload.mqtt_queue(), wifi_global, "test.mosquitto.org") != 0) {
    //    printf("main(): mqtt_client1.init(mqtt_pub.m_Mqtt_payload.mqtt_queue(), wifi_global, test.mosquitto.org) != 0\n");
    //}
    //else if(mqtt_client2.init(mqtt_pub.m_Mqtt_payload.mqtt_queue(), wifi_global, "192.168.1.82") != 0) {
        //printf("main(): \n");
    //}
    //else if(mqtt_client1.task_create("MQTT_task_1") != 0) {
    //    printf("main(): mqtt_client1.task_create(MQTT_task_1) != 0\n");
    //}
    ////else if(mqtt_client2.task_create("MQTT_task_2") != 0) {
    ////    printf("main(): mqtt_client2.task_create(MQTT_task_2) != 0\n");
    ////}
    //else if(mqtt_pub.init(mqtt_client1) != 0) {
    //    printf("main(): mqtt_pub.init(mqtt_client1) != 0\n");
    //}
    //else if(mqtt_pub.task_create("mqtt_pub_1") != 0) {
    //    printf("main(): mqtt_pub.task_create(mqtt_pub_1) != 0\n");
    //}
}
