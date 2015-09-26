/* 
 * The MIT License (MIT)
 * 
 * ESP8266 FreeRTOS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#if defined(REMOTE_BUILD)
    #include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
    #include <com/tholusi/esp-open-rtos/date_time/system_time.h>
#else
    #include <cplusplus/cplusplus.hpp>
    #include <date_time/system_time.h>
#endif

#include <stdio.h>

#include "task_mqtt.hpp"

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * MyMQTT
 *
 */

/**
 * 
 * @param wifi
 * @param broker
 * @param port
 * @param client_id
 * @return 
 */
int MyMqtt::init(esp_open_rtos::mqtt::mqtt_payload_recv_t::mqtt_queue_t& mqtt_queue, esp_open_rtos::wifi::wifi_t& wifi, const char* broker, int port, const char* client_id)
{
    int ret = 0;
    
    if(esp_open_rtos::mqtt::mqtt_client_queue_t::init(mqtt_queue, wifi, broker, port, client_id) != 0) {
        ret = -1;
    }    
    else if(esp_open_rtos::mqtt::fabric_t::init("fabric", wifi.mac(), "esp_open_rtos") != 0) {
        ret = -2;
    }
    else {
        // create LWT topic and message
        char* will_topic   = new char[maxTopicSize()];
        char* will_message = new char[maxMessageSize()];

        if(will_topic == 0 || will_message == 0) {
            DTXT("MyMqtt::init(): error; could not allocate memory\n");
            ret = -3;
        }
        else {
            DTXT("MyMqtt::init(): success - begin\n");
            
            fabricStatusMessage(fabric_disconnected, 0, will_topic, will_message);

            // set the LWT - used when MQTT is connecting
            setLwt(will_topic, will_message, MQTT::QOS2, false);

            delete[] will_topic;
            delete[] will_message;
            
            DTXT("MyMqtt::init(): success - end\n");
        }
    }
    
    return ret;
}
/**
 * 
 */
void MyMqtt::on_connect()
{
    DTXT("MyMqtt::on_connect(): begin; maxTopicSize() = %d, maxMessageSize() = %d\n", maxTopicSize(), maxMessageSize());
    
    char* topic = new char[maxTopicSize()];
    char* msg   = new char[maxMessageSize()];
    
    if(topic == 0 || msg == 0) {
        DTXT("MyMqtt::on_connect(): error; could not allocate memory\n");
    }
    else {
        // publish the fact that we're alive
        fabricStatusMessage(esp_open_rtos::mqtt::fabric_t::fabric_online, esp_uptime(0), topic, msg);
        
        publish(topic, msg, MQTT::QOS0, true);
        
        delete[] msg;
        delete[] topic;
    }
    
    /*
        // commands/updates/status
        std::string topic1 = getRootTopic() + "/+/$commands/$clients/#";

        std::string topic2 = getRootTopic() + "/+/$feeds/$onramp/+/metric/#";                 // metrics
        std::string topic3 = getRootTopic() + "/+/$feeds/$onramp/+/configuration/os_info";    // os info
        std::string topic4 = getRootTopic() + "/+/$feeds/$onramp/+/configuration/platform";   // platform info

        subscribe(NULL, topic1.c_str(), qos());
        subscribe(NULL, topic2.c_str(), qos());
        subscribe(NULL, topic3.c_str(), qos());
        subscribe(NULL, topic4.c_str(), qos());    
    */
    
/*    
    fabric/e03fe8f6-ad55-455f-9039-2a9fd7d9eec5/$feeds/$offramp/actor_id/actor_platform_id/task_id/chronos/time/seconds 
     {
        "d" : {
           "_type" : "clock",
           "seconds" : 1440670960
        }
     }
*/
    // commands/updates/status
    strcpy(m_CmdTopic, fabricRootTopic());
    strcat(m_CmdTopic, "/+/$commands/$clients/#");

    subscribe(m_CmdTopic);
    
    subscribe("fabric/e03fe8f6-ad55-455f-9039-2a9fd7d9eec5/$feeds/$offramp/actor_id/actor_platform_id/task_id/chronos/time/seconds");
    
    // mosquitto_pub -m "{\"d\":{\"_type\":\"set_output\",\"val\":1}}" -t "fabric/e03fe8f6-ad55-455f-9039-2a9fd7d9eec5/\$feeds/\$offramp/actor_id/actor_platform_id/task_id/set/output/1"
    subscribe("fabric/+/$feeds/$offramp/actor_id/actor_platform_id/task_id/+/+/+");
    
    DTXT("MyMqtt::on_connect(): end\n");
}
/**
 * 
 */
void MyMqtt::on_disconnect()
{
    DTXT("MyMqtt::on_disconnect(): begin\n");
    DTXT("MyMqtt::on_disconnect(): end\n");
}

