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
    #include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
#else
    #include <cplusplus/cplusplus.hpp>
    #include <date_time/system_time.h>
    #include <wifi/wifi.hpp>
#endif

#include <stdio.h>

#include "task_pub.hpp"

#define DTXT(...)       printf(__VA_ARGS__)

extern MyGpio_t gpio;

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
    
    char* topic                = new char[m_Client->maxTopicSize()];
    char* output_changed_topic = new char[m_Client->maxTopicSize()];

    if(topic == 0 || output_changed_topic == 0) {
        DTXT("MqttPub::task(): error; could not allocate memeory for topic\n");
        return;
    }

    esp_open_rtos::mqtt::fabric_t::init("fabric", m_Client->mac(), "esp_open_rtos");        

    // create (static) topic
    fabricOnrampTopic("uptime", "seconds", topic);
    fabricOnrampTopic("read", "output", output_changed_topic);

    m_PubCountdown.countdown(1);
    m_LedCountdown.countdown_ms(200);

    while(true) {
        /******************************************************************************************************************
         * show wifi and MQTT status on blue LED on ESP-12E board - should we have a separate task running that could be
         * used for more advanced signalling via the blue LED?
         */
        if(m_LedCountdown.expired()) {
            gpio.toggle(GPIO_BLUE);

            if(wifi_global.isConnected() == true) {
                if(m_Client->isConnected() == true) {
                    m_LedCountdown.countdown_ms(3000);
                }
                else {
                    m_LedCountdown.countdown_ms(1000);
                }
            }
            else {
                m_LedCountdown.countdown_ms(200);
            }
        }
        
        /******************************************************************************************************************
         * check if any of the outputs has changed
         *
         */
        int           curr_val;
        int           prev_val;
        unsigned long prev_millis;
        
        //
        // U2
        //
        curr_val = gpio.read(GPIO_U2);
        gpio.prev_val(GPIO_U2, prev_val, prev_millis);
        
        if(curr_val != prev_val) {
            // create payload in IBM Bluemix JSON format
            if(bmixEncoderBegin() == 0) {
                bmixAddString("_type", "read_output");
                bmixAddInt(   "port",  1);
                bmixAddInt(   "val",   curr_val);
                bmixAddLong(  "clock", esp_time(0));                
                
                char* d = bmixEncoderEnd();
                
                if(d != 0) {
                    m_Client->publish(output_changed_topic, d);
                    //DTXT("MqttPub::task(): d = %s\n", d);
                }
                else {
                    DTXT("MqttPub::task(): d == 0\n");
                }
            }
            else {
                DTXT("MqttPub::task(): bmixEncoderBegin() != 0\n");                
            }
            
            gpio.save_val(GPIO_U2, curr_val);
        }
        
        //
        // U3
        //
        curr_val = gpio.read(GPIO_U3);
        gpio.prev_val(GPIO_U3, prev_val, prev_millis);
        
        if(curr_val != prev_val) {
            // create payload in IBM Bluemix JSON format
            if(bmixEncoderBegin() == 0) {
                bmixAddString("_type", "read_output");
                bmixAddInt(   "port",  2);
                bmixAddInt(   "val",   curr_val);
                bmixAddLong(  "clock", esp_time(0));
                
                char* d = bmixEncoderEnd();
                
                if(d != 0) {
                    m_Client->publish(output_changed_topic, d);
                    //DTXT("MqttPub::task(): d = %s\n", d);
                }
                else {
                    DTXT("MqttPub::task(): d == 0\n");
                }
            }
            else {
                DTXT("MqttPub::task(): bmixEncoderBegin() != 0\n");                
            }
            
            gpio.save_val(GPIO_U3, curr_val);
        }
        
        /******************************************************************************************************************
         * publish data if it's time
         *
         */
        /*if(m_PubCountdown.expired()) {
            //DTXT("MqttPub::task(): countdown expired\n");

            // create payload in IBM Bluemix JSON format
            if(bmixEncoderBegin() == 0) {
                bmixAddString("_type",  "seconds");
                bmixAddLong(  "uptime", esp_uptime(0));
                bmixAddLong(  "clock",  esp_time(0));
                
                char* d = bmixEncoderEnd();
                
                if(d != 0) {
                    m_Client->publish(topic, d);
                    //DTXT("MqttPub::task(): d = %s\n", d);
                }
                else {
                    DTXT("MqttPub::task(): d == 0\n");
                }
            }
            else {
                DTXT("MqttPub::task(): bmixEncoderBegin() != 0\n");                
            }
            
            m_PubCountdown.countdown(1);
        }*/

        /******************************************************************************************************************
         * receive data from MQTT task, if any
         *
         */
        if(m_Mqtt_payload.receive(m_TopicName, m_Msg) == 0) {
            //DTXT("MqttPub::task(): got message; len = %d, payloadlen = %d\n", m_TopicName.lenstring.len, m_Msg.payloadlen);
            
            int chronos_detect = 0;
            int out_detect     = 0;
            
            int  gpio_port     = -1;
            
            //
            // go thru each sub-topics
            //
            if(fabricTokenBegin(&m_TopicName) == 0) {
                //DTXT("MqttPub::task(): fabricTokenBegin(md) == 0\n");

                char* subtopic;
                int   idx;

                while(fabricTokenNext(&subtopic, idx) == 0) {
                    //DTXT("MqttPub::task(): idx = %d; subtopic = %s\n", idx, subtopic);

                    switch(idx) {
                        case 1:
                            if(strcmp(subtopic, m_Client->mac()) == 0) {        // does this message come from ourselves, i.e. the second word is our MAC-address
                                // yes it does - stop sub-topic iteration
                                fabricTokenReset();
                            }
                            else if(strcmp(subtopic, "e03fe8f6-ad55-455f-9039-2a9fd7d9eec5") == 0) {
                                if(chronos_detect == 0) {
                                    ++chronos_detect;
                                }
                            }
                            break;

                        case 7:
                            if(strcmp(subtopic, "chronos") == 0) {
                                if(chronos_detect == 1) {
                                    ++chronos_detect;
                                }
                            }
                            else if(strcmp(subtopic, "set") == 0) {
                                //DTXT("MqttPub::task(): set; out_detect = %d\n", out_detect);
                                if(out_detect == 0) {
                                    ++out_detect;
                                }
                            }
                            break;
                            
                        case 8:
                            if(strcmp(subtopic, "time") == 0) {
                                if(chronos_detect == 2) {
                                    ++chronos_detect;
                                }
                            }
                            else if(strcmp(subtopic, "output") == 0) {
                                //DTXT("MqttPub::task(): output; out_detect = %d\n", out_detect);
                                if(out_detect == 1) {
                                    ++out_detect;
                                }
                            }
                            break;

                        case 9:
                            //DTXT("MqttPub::task(): idx = 9; subtopic = '%s'\n", subtopic);
                            
                            if(strcmp(subtopic, "seconds") == 0) {
                                if(chronos_detect == 3) {
                                    ++chronos_detect;
                                }
                            }
                            else if(strcmp(subtopic, "1") == 0) {
                                //DTXT("MqttPub::task(): 1; out_detect = %d\n", out_detect);
                                if(out_detect == 2) {
                                    ++out_detect;
                                    gpio_port = GPIO_U2;
                                }
                            }
                            else if(strcmp(subtopic, "2") == 0) {
                                //DTXT("MqttPub::task(): 2; out_detect = %d\n", out_detect);
                                if(out_detect == 2) {
                                    ++out_detect;
                                    gpio_port = GPIO_U3;
                                }
                            }
                            break;

                        default:
                            break;
                    }
                }
            }
            else {
                //DTXT("MqttPub::task(): fabricTokenBegin(md) != 0; %s\n", md->topicName.lenstring.data);
            }

            //
            // act on incoming message
            //
            if(chronos_detect == 4) {
                DTXT("MqttPub::task(): chronos topic; id = %d, qos = %d\n", m_Msg.id, (int)m_Msg.qos);
                do_chronos(m_Msg.payload, m_Msg.payloadlen);                    
            }
            else if(out_detect == 3) {
                DTXT("MqttPub::task(): out topic; id = %d, qos = %d\n", m_Msg.id, (int)m_Msg.qos);
                do_gpio_out(gpio_port, m_Msg.payload, m_Msg.payloadlen);                    
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
/**
 * 
 * @param port
 * @param payload
 * @param payloadlen
 */
void MqttPub::do_gpio_out(int port, void* payload, size_t payloadlen)
{
    /******************************************************************************************************************
     * extract data from 'd' object
     *
     * {
     *   "d" : {
     *     "_type" : "set_output",
     *     "val"   : 1
     *   }
     * }
     * 
     */
    if(bmixDecoderBegin(payload, payloadlen) != 0) {
        const char* _type;
        int         val;

        if(bmixGetString("_type", &_type) == 0 && strcmp(_type, "set_output") == 0 && bmixGetInt("val", val) == 0) {
            DTXT("MqttPub::do_gpio_out(): port = %d, val = %d\n", port, val);
            
            // set output
            if(gpio.write(port, val == 0 ? false : true) != 0) {
                DTXT("MqttPub::do_gpio_out(): gpio.write() returns error\n");
            }
        }

        // get ready for next run
        bmixDecoderEnd();
    }
}