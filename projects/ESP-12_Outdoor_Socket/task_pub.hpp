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
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#ifndef TASK_PUB_HPP
#define	TASK_PUB_HPP

#if defined(REMOTE_BUILD)
    #include <com/tholusi/esp-open-rtos/mqtt/mqtt_bluemix.hpp>
#else
    #include <mqtt/mqtt_bluemix.hpp>
#endif

#include "task_mqtt.hpp"
#include "io.hpp"

/******************************************************************************************************************
 * MqttPub
 *
 */
class MqttPub: public esp_open_rtos::thread::task_t, protected esp_open_rtos::mqtt::fabric_t, protected esp_open_rtos::mqtt::bluemix_t
{
public:
    /**
     * 
     */
    MqttPub()
    {}
    /**
     * 
     * @param mqtt_client
     * @return 
     */
    int init(MyMqtt& mqtt_client);
    
    esp_open_rtos::mqtt::mqtt_payload_recv_t m_Mqtt_payload;
    
private:
    /**
     * 
     */
    void task();
    /**
     * 
     * @param payload
     * @param payloadlen
     */
    void do_chronos(void* payload, size_t payloadlen);
    /**
     * 
     * @param port
     * @param payload
     * @param payloadlen
     */
    void do_gpio_out(int port, void* payload = NULL, size_t payloadlen = 0);
    
    MyMqtt*                             m_Client;
    esp_open_rtos::timer::countdown_t   m_PubCountdown;
    esp_open_rtos::timer::countdown_t   m_LedCountdown;
    
    // structures to receive incoming publish messages
    MQTTString                          m_TopicName;
    MQTT::Message                       m_Msg;

    // no copy and no = operator
    MqttPub(const MqttPub&);
    MqttPub &operator=(const MqttPub&);
};


#endif	/* TASK_PUB_HPP */

