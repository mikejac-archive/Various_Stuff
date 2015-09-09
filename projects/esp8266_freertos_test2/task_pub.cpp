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

#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#include <com/tholusi/esp-open-rtos/date_time/system_time.h>
#include <stdio.h>

#include "task_pub.hpp"

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * MqttPub
 *
 */

/**
 * 
 * @param mqtt_client
 * @return 
 */
int MqttPub::init(MyMqtt& mqtt_client)
{
    m_Client = &mqtt_client;

    if(m_Client == 0) {
        return -1;
    }
    else if(esp_open_rtos::mqtt::bluemix_t::bmixInit() != 0) {
        return -2;
    }
    else {
        return 0;
    }
}
/**
 * 
 */
void MqttPub::task()
{
    DTXT("MqttPub::task(): start\n");

    char* topic = new char[m_Client->maxTopicSize()];

    if(topic == 0) {
        DTXT("MqttPub::task(): error; could not allocate memeory for topic\n");
        return;
    }

    esp_open_rtos::mqtt::fabric_t::init("fabric", m_Client->mac(), "esp_open_rtos");        

    // create (static) topic
    fabricOnrampTopic("uptime", "seconds", topic);

    m_Countdown.countdown(1);

    const MQTT::MessageData* md;

    while(true) {
        /******************************************************************************************************************
         * publish data if it's time
         *
         */
        if(m_Countdown.expired()) {
            //DTXT("MqttPub::task(): countdown expired\n");

            // create payload in IBM Bluemix JSON format
            if(bmixEncoderBegin() == 0) {
                bmixAddString("_type",   "seconds");
                bmixAddLong(  "seconds", esp_uptime(0));
                bmixAddLong(  "clock",   esp_time(0));
                
                char* d = bmixEncoderEnd();
                
                if(d != 0) {
                    m_Client->publish(topic, d);
                }
                else {
                    DTXT("MqttPub::task(): d == 0\n");
                }
            }
            else {
                DTXT("MqttPub::task(): bmixEncoderBegin() != 0\n");                
            }
            
            m_Countdown.countdown(1);
        }

        /******************************************************************************************************************
         * receive data from MQTT task, if any
         *
         */
        if((md = m_Mqtt_payload.receive()) != 0) {
            //printf("MqttPub::task(): got message\n");

            int chronos_detect = 0;

            //
            // go thru each sub-topic
            //
            if(fabricTokenBegin(md) == 0) {
                //DTXT("MqttPub::task(): fabricTokenBegin(md) == 0\n");

                char* subtopic;
                int   idx;

                while(fabricTokenNext(&subtopic, idx) == 0) {
                    //DTXT("MqttPub::task(): idx = %d; subtopic = %s\n", idx, subtopic);

                    switch(idx) {
                        case 0:
                            if(strcmp(subtopic, "fabric") == 0) {
                                //DTXT("MqttPub::task(): 'fabric' message\n");
                            }
                            else if(strcmp(subtopic, "home") == 0) {
                                //DTXT("MqttPub::task(): 'home' message\n");
                            }
                            break;
                            
                        case 1:
                            if(strcmp(subtopic, m_Client->mac()) == 0) {        // does this message come from ourselves, i.e. the second word is our MAC-address
                                // yes it does - stop sub-topic iteration
                                //DTXT("MqttPub::task(): own message\n");

                                fabricTokenReset();                             // force-stop iteration of this topic
                            }
                            else if(strcmp(subtopic, "e03fe8f6-ad55-455f-9039-2a9fd7d9eec5") == 0) {
                                if(chronos_detect == 0) {
                                    ++chronos_detect;
                                }
                            }
                            break;

                        case 8:
                            if(strcmp(subtopic, "time") == 0) {
                                if(chronos_detect == 1) {
                                    ++chronos_detect;
                                }
                            }
                            break;

                        case 9:
                            if(strcmp(subtopic, "seconds") == 0) {
                                if(chronos_detect == 2) {
                                    ++chronos_detect;
                                }
                            }
                            break;

                        default:
                            break;
                    }
                }
            }
            else {
                // I think this happens when one TCP packet contains more than one message ...
                //DTXT("MqttPub::task(): fabricTokenBegin(md) != 0; %s\n", md->topicName.lenstring.data);
            }

            if(chronos_detect == 3) {
                DTXT("MqttPub::task(): chronos topic\n");
                do_chronos(md->message.payload, md->message.payloadlen);                    
            }
        }
    }
}
/**
 * 
 * @param payload
 * @param payloadlen
 */
void MqttPub::do_chronos(void* payload, size_t payloadlen)
{
    /******************************************************************************************************************
     * extract data from 'd' object
     *
     */
    if(bmixDecoderBegin(payload, payloadlen) != 0) {
        const char* _type;
        esp_time_t  seconds;

        if(bmixGetString("_type", &_type) == 0 && strcmp(_type, "clock") == 0 && bmixGetLong("seconds", (int64_t&)seconds) == 0) {
            // set current system time
            esp_stime(&seconds);
        }

        // get ready for next run
        bmixDecoderEnd();
    }
}

