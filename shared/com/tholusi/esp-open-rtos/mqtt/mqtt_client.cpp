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

#include <com/tholusi/esp-open-rtos/mqtt/mqtt_client.hpp>
#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>

#include "espressif/esp_common.h"
#include "espressif/sdk_private.h"

using namespace esp_open_rtos::mqtt;

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * class mqtt_client_t
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
int mqtt_client_t::init(esp_open_rtos::wifi::wifi_t& wifi, const char* broker, int port, const char* client_id)
{
    m_State         = none;
    m_Port          = port;
    m_Wifi          = &wifi;
    
    if(m_Publish_mutex.mutex_create() != 0) {
        return -1;
    }
    
    strncpy(m_Hostname, broker, MAX_HOSTNAME_SIZE);
    m_Hostname[MAX_HOSTNAME_SIZE - 1] = '\0';
    
    if(client_id == NULL) {
        strcpy(m_ClientId, "esp_");
        strcat(m_ClientId, wifi.mac());
    }
    else {
        strncpy(m_ClientId, client_id, MAX_CLIENT_ID_SIZE);
    }
    
    m_ClientId[MAX_CLIENT_ID_SIZE] = '\0';
    
#if defined(MQTT_WITH_USERNAME_PASSWORD)
    m_Username[0] = '\0';
    m_Password[0] = '\0';
#endif
    
#if defined(MQTT_WITH_LWT)    
    m_LwtTopic[0]   = '\0';
    m_LwtMessage[0] = '\0';
#endif
    
    return 0;
}
/**
 * 
 * @param pcName
 * @return 
 */
int mqtt_client_t::task_create(const char * const pcName)
{
    unsigned short usStackDepth = sizeof(ip::ipstack_t) + sizeof(MQTT_t) + sizeof(MQTT::Message) + 256;
    
    return thread::task_t::task_create(pcName, usStackDepth, 2);
}
/**
 * 
 * @param topic
 * @param payload
 * @param payload_len
 * @param qos
 * @param retain
 * @return 
 */
int mqtt_client_t::publish(const char* topic, const char* payload, int payload_len, MQTT::QoS qos, bool retain)
{
    int rc;
    
    //DTXT("mqtt_client_t::publish(): begin\n");
    
    if(payload_len <= 0) {
        DTXT("mqtt_client_t::publish(): error; payload_len = %d\n", payload_len);
        rc = -1;
    }
    else if(m_State != ready) {
        DTXT("mqtt_client_t::publish(): warning; m_State != ready\n");
        rc = -2;
    }
    else {
        if(m_Publish_mutex.try_lock(0) == 0) {
            //DTXT("mqtt_client_t::publish(): got mutex\n");

            if(m_Publish_message.payloadlen == 0) {
                //DTXT("mqtt_client_t::publish(): m_Publish_message.payloadlen == 0\n");

                strncpy(m_Publish_topic, topic, MAX_TOPIC_SIZE);

                m_Publish_message.qos        = qos;
                m_Publish_message.retained   = retain;
                m_Publish_message.dup        = false;
                m_Publish_message.payload    = (void*)payload;
                m_Publish_message.payloadlen = payload_len;
            }
            else {
                DTXT("mqtt_client_t::publish(): m_Publish_message.payloadlen != 0\n");            
            }

            m_Publish_mutex.unlock();

            rc = 0;
        }
        else {
            DTXT("mqtt_client_t::publish(): error; failed to get mutex\n");
            rc = -3;
        }
    }
    
    //DTXT("mqtt_client_t::publish(): end; rc = %d\n", rc);

    return rc;
}
/**
 * 
 * @param username
 * @param password
 * @return 
 */
#if defined(MQTT_WITH_USERNAME_PASSWORD)
int mqtt_client_t::setUserPassword(const char* username, const char* password)
{
    strncpy(m_Username, username, MAX_USERNAME_SIZE - 1);
    
    m_Username[MAX_USERNAME_SIZE - 1] = '\0';
    
    if(password != 0) {
        strncpy(m_Password, password, MAX_PASSWORD_SIZE - 1);
        
        m_Password[MAX_PASSWORD_SIZE - 1] = '\0';
    }
    
    return 0;
}
#endif
/**
 * 
 * @param will_topic
 * @param will_message
 * @param will_qos
 * @param will_retain
 * @return 
 */
#if defined(MQTT_WITH_LWT)    
int mqtt_client_t::setLwt(  const char*    will_topic,
                            const char*    will_message,
                            enum MQTT::QoS will_qos,
                            bool           will_retain)
{
    
    strncpy(m_LwtTopic,   will_topic,   MAX_TOPIC_SIZE - 1);
    strncpy(m_LwtMessage, will_message, MAX_PAYLOAD_SIZE - 1);
    //strncpy(m_LwtMessage, will_message, MAX_PACKET_SIZE - MAX_TOPIC_SIZE - 1);
    
    m_LwtTopic[MAX_TOPIC_SIZE - 1]                     = '\0';
    m_LwtMessage[MAX_PAYLOAD_SIZE - 1] = '\0';
    //m_LwtMessage[MAX_PACKET_SIZE - MAX_TOPIC_SIZE - 1] = '\0';
    
    m_LwtQos      = will_qos;
    m_LwtRetained = will_retain;
    
    return 0;
}
#endif
/**
 * 
 * @param topic
 * @param qos
 * @return 
 */
int mqtt_client_t::subscribe(const char* topic, MQTT::QoS qos)
{
    DTXT("mqtt_client_t::subscribe(): begin\n");
    
    int rc = m_Client->subscribe(topic, qos);   
    
    DTXT("mqtt_client_t::subscribe(): end; rc = %d\n", rc);

    return rc;
}
/**
 * 
 * @param topic
 * @return 
 */
int mqtt_client_t::unsubscribe(const char* topic)
{
    return 0;
}
/**
 * 
 */
void mqtt_client_t::task()
{
    DTXT("mqtt_client_t::task(): start; m_ClientId = %s\n", m_ClientId);
    
    ip::ipstack_t ipstack;
    MQTT_t client = MQTT_t(ipstack);

    m_Client  = &client;
    m_Ipstack = &ipstack;
    
    client.setDefaultMessageHandler(mqtt_client_t::_on_message, this);
    
    while(true) {
        switch(m_State) {
            case none:
                if(m_Wifi->isConnected() == true) {
                    m_State = do_tcp_connect();
                }
                else {
                    sleep(1000);
                }
                break;
                
            case tcp_connect_fail:
                sleep(1000);
                m_State = none; //do_tcp_connect();
                break;
                
            case tcp_connect_done:
                m_State = do_mqtt_connect();
                break;

            case tcp_disconnect_done:
                sleep(1000);
                m_State = none; //do_tcp_connect();
                break;
                    
            case mqtt_connect_fail:
                m_State = do_tcp_disconnect();
                break;
                
            case mqtt_connect_done:
                m_State = ready;
                on_connect();                                                   // notify user
                break;
            
            case ready:
                client.yield(CLIENT_YIELD);                                     // MQTT housekeeping
                
                if(client.isConnected() == false) {
                    // force TCP disconnect, and start the connect procedure over
                    m_State = mqtt_connect_fail;
                    on_disconnect();                                            // notify user
                }
                break;
                
            default:
                break;
        }
        
        if(m_State == ready && m_Publish_mutex.try_lock(0) == 0) {
            if(m_Publish_message.payloadlen != 0) {
                //DTXT("mqtt_client_t::task(): m_Publish_message.payloadlen != 0\n");
                
                int rc = client.publish(m_Publish_topic, m_Publish_message);

                if(rc != MQTT::SUCCESS) {
                    DTXT("mqtt_client_t::task(): error: message not published\n");
                    
                    if(client.isConnected() == false) {
                        // force TCP disconnect, and start the connect procedure over
                        m_State = mqtt_connect_fail;
                        on_disconnect();                                        // notify user
                    }
                }
                
                m_Publish_message.payloadlen = 0;
            }            
            
            m_Publish_mutex.unlock();
        }
    }
}
/**
 * 
 * @return 
 */
mqtt_client_t::state_t mqtt_client_t::do_tcp_connect()
{
    state_t state;
    
    if(m_Ipstack->isConnected() == false) {
        DTXT("mqtt_client_t::do_tcp_connect(): connecting to %s:%d\n", m_Hostname, m_Port);

        int rc = m_Ipstack->connect(m_Hostname, m_Port);

        if(rc != 0) {
            DTXT("mqtt_client_t::do_tcp_connect(): error; rc = %d\n", rc);
            state = tcp_connect_fail;
        }
        else {
            DTXT("mqtt_client_t::do_tcp_connect(): TCP connected\n");
            state = tcp_connect_done;            
        }
    }
    else {
        DTXT("mqtt_client_t::do_tcp_connect(): error; already connected, disconnecting\n");
        
        m_Ipstack->disconnect();
        
        state = tcp_connect_fail;
    }
    
    return state;
}
/**
 * 
 * @return 
 */
mqtt_client_t::state_t mqtt_client_t::do_tcp_disconnect()
{
    DTXT("mqtt_client_t::do_tcp_disconnect(): disconnecting\n");
        
    m_Ipstack->disconnect();
        
    return tcp_disconnect_done;
}
/**
 * 
 * @return 
 */
mqtt_client_t::state_t mqtt_client_t::do_mqtt_connect()
{
    state_t state;
    
    DTXT("mqtt_client_t::do_mqtt_connect(): MQTT connecting\n");

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion            = 3;
    data.clientID.cstring       = m_ClientId;
    
#if defined(MQTT_WITH_USERNAME_PASSWORD)
    // username and password setup
    if(m_Username[0] != '\0') {
        data.username.cstring = m_Username;
        
        if(m_Password[0] != '\0') {
            data.password.cstring = m_Password;
        }
    }

#endif
    
#if defined(MQTT_WITH_LWT)    
    // last will and testament setup
    if(m_LwtTopic[0] != '\0' && m_LwtMessage[0] != '\0') {
        MQTTPacket_willOptions will = MQTTPacket_willOptions_initializer;
        will.qos                    = m_LwtQos;
        will.retained               = m_LwtRetained;
        will.topicName.cstring      = m_LwtTopic;
        will.message.cstring        = m_LwtMessage;
        
        data.will     = will;
        data.willFlag = 1;
    }
#endif
    
    int rc = m_Client->connect(data);

    if(rc != 0) {
        DTXT("mqtt_client_t::do_mqtt_connect(): error; rc = %d\n", rc);
        state = mqtt_connect_fail;
    }
    else {
        DTXT("mqtt_client_t::do_mqtt_connect(): MQTT connected\n");
        state = mqtt_connect_done;
    }
    
    return state;
}
/**
 * 
 * @param md
 * @param userptr
 */
void mqtt_client_t::_on_message(MQTT::MessageData& md, const void * userptr)
{
    if(userptr != 0) {
        ((mqtt_client_t*)(userptr))->on_message(md);
    }
    else {
        
    }
}
/**
 * 
 * @param topicFilter
 * @param topicName
 * @return 
 */
bool mqtt_client_t::isTopicMatched(const char* topicFilter, MQTTString& topicName)
{
    char* curf     = (char*) topicFilter;
    char* curn     = topicName.lenstring.data;
    char* curn_end = curn + topicName.lenstring.len;

    while(*curf && curn < curn_end) {
        if(*curn == '/' && *curf != '/') {
            break;
        }
        
        if(*curf != '+' && *curf != '#' && *curf != *curn) {
            break;
        }
        
        if(*curf == '+') {
            // skip until we meet the next separator, or end of string
            char* nextpos = curn + 1;
            
            while(nextpos < curn_end && *nextpos != '/') {
                nextpos = ++curn + 1;
            }
        }
        else if(*curf == '#') {
            curn = curn_end - 1;    // skip until end of string
        }
        
        curf++;
        curn++;
    };

    return (curn == curn_end) && (*curf == '\0');
}

