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
#include <com/tholusi/esp-open-rtos/mqtt/mqtt_fabric.hpp>
#include <stdio.h>
#include <assert.h>

using namespace esp_open_rtos::mqtt;

#define DTXT(...)       printf(__VA_ARGS__)

const char* FABRIC_CMD_STATUS   = "status";

/******************************************************************************************************************
 * fabric_t
 *
 */

/**
 * 
 * @param root_topic
 * @param nodename
 * @param platform_id
 * @return 
 */
int fabric_t::init(const char* root_topic, const char* nodename, const char* platform_id)
{
    strncpy(m_RootTopic,  root_topic,  ROOT_TOPIC_SIZE);
    strncpy(m_NodeName,   nodename,    NODENAME_SIZE);
    strncpy(m_PlatformId, platform_id, PLATFORM_ID_SIZE);
    
    m_RootTopic[ROOT_TOPIC_SIZE - 1]   = '\0';
    m_NodeName[NODENAME_SIZE - 1]      = '\0';
    m_PlatformId[PLATFORM_ID_SIZE - 1] = '\0';
    
    strcpy(m_ActorId, "actor_id");
    strcpy(m_ActorPlatformId, "actor_platform_id");
    strcpy(m_TaskId, "task_id");
    
    return 0;
}
/**
 * 
 * @param fabric_status
 * @param seconds
 * @param topic
 * @param message
 * @return 
 */
int fabric_t::fabricStatusMessage(fabric_status_t fabric_status, int seconds, char* topic, char* message)
{
    int ret = 0;

    strcpy(topic, m_RootTopic);
    strcat(topic, "/");
    strcat(topic, m_NodeName);
    strcat(topic, "/$commands/$clients/");
    strcat(topic, m_ActorId);
    strcat(topic, "/");
    strcat(topic, m_PlatformId);
    strcat(topic, "/");
    strcat(topic, FABRIC_CMD_STATUS);

    DTXT("fabric_t::statusMessage(): topic = %s, strlen = %d\n", topic, strlen(topic));

    assert(strlen(topic) < MAX_TOPIC_SIZE);
    
    switch(fabric_status) {
        case fabric_online:
            sprintf(message, "\"d\":{\"_type\":\"system_status\",\"status\":\"online\",\"uptime\":%d}", seconds);
            break;

        case fabric_offline:
            sprintf(message, "\"d\":{\"_type\":\"system_status\",\"status\":\"offline\",\"uptime\":%d}", seconds);
            break;

        case fabric_disconnected:
            strcpy(message, "\"d\":{\"_type\":\"system_status\",\"status\":\"disconnected\",\"uptime\":null}");
            break;

        default:
            message[0] = '\0';
            ret        = -1;
    }
    
    DTXT("fabric_t::statusMessage(): message = %s, strlen = %d\n", message, strlen(message));

    assert(strlen(message) < MAX_PAYLOAD_SIZE);
    
    return ret;
}
/**
 * 
 * @param service_id
 * @param feed_id
 * @param topic
 * @return 
 */
int fabric_t::fabricOnrampTopic(const char* service_id, const char* feed_id, char* topic)
{
    int ret = 0;

    strcpy(topic, m_RootTopic);
    strcat(topic, "/");
    strcat(topic, m_NodeName);
    strcat(topic, "/$feeds/$onramp/");
    strcat(topic, m_PlatformId);
    strcat(topic, "/");
    strcat(topic, service_id);
    strcat(topic, "/");
    strcat(topic, feed_id);

    DTXT("fabric_t::message(): topic = %s, strlen = %d\n", topic, strlen(topic));

    assert(strlen(topic) < MAX_TOPIC_SIZE);
    
    return ret;    
}
/**
 * 
 * @param service_id
 * @param feed_id
 * @param topic
 * @return 
 */
int fabric_t::fabricOfframpTopic(const char* service_id, const char* feed_id, char* topic)
{
    int ret = 0;

    strcpy(topic, m_RootTopic);
    strcat(topic, "/");
    strcat(topic, m_NodeName);
    strcat(topic, "/$feeds/$offramp/");
    strcat(topic, m_ActorId);
    strcat(topic, "/");
    strcat(topic, m_ActorPlatformId);
    strcat(topic, "/");
    strcat(topic, m_TaskId);
    strcat(topic, "/");
    strcat(topic, m_PlatformId);
    strcat(topic, "/");
    strcat(topic, service_id);
    strcat(topic, "/");
    strcat(topic, feed_id);

    DTXT("fabric_t::message(): topic = %s, strlen = %d\n", topic, strlen(topic));

    assert(strlen(topic) < MAX_TOPIC_SIZE);
    
    return ret;    
}
/**
 * 
 * @param md
 * @return 
 */
int fabric_t::fabricTokenBegin(const MQTTString* topicName)
{
    m_TopicName = (MQTTString*) topicName;
    
    if(m_TopicName == 0) {
        DTXT("fabric_t::fabricTokenBegin(): m_TopicName == 0\n");
        return -1;
    }
    else if(m_TopicName->lenstring.len <= 0) {
        DTXT("fabric_t::fabricTokenBegin(): len  <= 0\n");
        m_TopicName = 0;
        return -1;
    }
    
    m_P     = m_TopicName->lenstring.data;
    m_Pp    = m_TopicName->lenstring.data;
    m_Words = 0;
    m_I     = 0;
    
    return 0;
}
/**
 * 
 * @param subtopic
 * @param index
 * @return 
 */
int fabric_t::fabricTokenNext(char** subtopic, int& index)
{
    if(m_TopicName == 0) {
        //DTXT("fabric_t::fabricTokenNext(): m_TopicName == 0\n");
        return -1;
    }
    
    while(m_I < m_TopicName->lenstring.len) {
        if(*m_P == '/') {
            *m_P = '\0';

            *subtopic = m_Pp;
            index     = m_Words;
            
            ++m_Words;
            
            m_Pp = m_P + 1;
            
            ++m_P;
            ++m_I;
            
            return 0;
        }

        ++m_P;
        ++m_I;
    }

    int len = m_P - m_Pp;
    
    //DTXT("fabric_t::fabricTokenNext(): len = %d\n", len);
    
    if(len > 0) {
        //
        // note: 'm_Pp' is not zero-terminated at this point
        //
        strncpy(m_LastSubtopic, m_Pp, len);
        m_LastSubtopic[len] = '\0';
        
        *subtopic = m_LastSubtopic;
        index     = m_Words;
        
        // no more in this topic-string
        m_TopicName = 0;
        
        return 0;
    }
    
    DTXT("fabric_t::fabricTokenNext(): len <= 0\n");
    
    return -1;
}
/**
 * 
 * @return 
 */
int fabric_t::fabricTokenReset()
{
    // no more in this topic-string
    m_TopicName = 0;

    return 0;
}