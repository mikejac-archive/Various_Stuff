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

#ifndef TASK_MQTT_HPP
#define	TASK_MQTT_HPP

#if defined(REMOTE_BUILD)
    #include <com/tholusi/esp-open-rtos/mqtt/mqtt_client.hpp>
    #include <com/tholusi/esp-open-rtos/mqtt/mqtt_fabric.hpp>
#else
    #include <mqtt/mqtt_client.hpp>
    #include <mqtt/mqtt_fabric.hpp>
#endif

/******************************************************************************************************************
 * MyMqtt
 *
 */
class MyMqtt: public esp_open_rtos::mqtt::mqtt_client_queue_t, protected esp_open_rtos::mqtt::fabric_t
{
public:
    /**
     * 
     */
    MyMqtt()
    {}
    /**
     * 
     * @param wifi
     * @param broker
     * @param port
     * @param client_id
     * @return 
     */
    int init(esp_open_rtos::mqtt::mqtt_payload_recv_t::mqtt_queue_t& mqtt_queue, esp_open_rtos::wifi::wifi_t& wifi, const char* broker, int port = 1883, const char* client_id = NULL);
    
private:
    /**
     * 
     */
    void on_connect();
    /**
     * 
     */
    void on_disconnect();
    
    char m_CmdTopic[esp_open_rtos::mqtt::MAX_TOPIC_SIZE];
    
    // no copy and no = operator
    MyMqtt(const MyMqtt&);
    MyMqtt &operator=(const MyMqtt&);    
};

#endif	/* TASK_MQTT_HPP */

