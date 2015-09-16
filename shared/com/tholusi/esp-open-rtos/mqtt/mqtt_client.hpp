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

#ifndef ESP_OPEN_RTOS_MQTT_CLIENT_HPP
#define	ESP_OPEN_RTOS_MQTT_CLIENT_HPP

// #define MQTT_WITH_USERNAME_PASSWORD

#include <org/eclipse/paho/mqtt/MQTTClient/src/MQTTClient.h>
#include <com/tholusi/esp-open-rtos/ip/ipstack.hpp>
#include "com/tholusi/esp-open-rtos/timer/countdown.hpp"
#include <com/tholusi/esp-open-rtos/mqtt/mqtt_config.hpp>
#include <com/tholusi/esp-open-rtos/thread/task.hpp>
#include <com/tholusi/esp-open-rtos/thread/mutex.hpp>
#include <com/tholusi/esp-open-rtos/thread/queue.hpp>
#include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
#include <stdlib.h>
#include <assert.h>

#define DTXT(...)       printf(__VA_ARGS__)

//void loghexdump(const void *pAddressIn, long lSize);

namespace esp_open_rtos {
namespace mqtt {

typedef MQTT::Client<ip::ipstack_t, timer::countdown_t, MAX_PACKET_SIZE> MQTT_t;

/******************************************************************************************************************
 * class mqtt_client_t
 *
 */
class mqtt_client_t: public thread::task_t
{
public:
    /**
     * 
     */
    mqtt_client_t()
    {}
    /**
     * 
     * @param wifi
     * @param broker
     * @param port
     * @param client_id
     * @return 
     */
    int init(esp_open_rtos::wifi::wifi_t& wifi, const char* broker, int port = 1883, const char* client_id = NULL);
    /**
     * 
     * @param pcName
     * @return 
     */
    int task_create(const char* const pcName);
    /**
     * 
     * @param topic
     * @param payload
     * @param payload_len
     * @param qos
     * @param retain
     * @return 
     */
    int publish(const char* topic, const char* payload, int payload_len, enum MQTT::QoS qos = MQTT::QOS0, bool retain = false);
    /**
     * 
     * @param topic
     * @param payload
     * @param qos
     * @param retain
     * @return 
     */
    int publish(const char* topic, const char* payload, enum MQTT::QoS qos = MQTT::QOS0, bool retain = false)
    {
        return publish(topic, payload, strlen(payload) + 1, qos, retain);
    }
    /**
     * 
     * @return 
     */
    const char* mac()
    {
        return m_Wifi->mac();
    }
    /**
     * 
     * @return 
     */
    int maxTopicSize()
    {
        return MAX_TOPIC_SIZE - 1;
    }
    /**
     * 
     * @return 
     */
    int maxMessageSize()
    {
        return MAX_PAYLOAD_SIZE;
    }
    /**
     * 
     * @return 
     */
    inline bool isConnected()
    {
        return (m_State == ready) ? true : false;
    }
    /**
     * 
     * @param topicFilter
     * @param topicName
     * @return 
     */
    static bool isTopicMatched(const char* topicFilter, MQTTString& topicName);
   
protected:
#if defined(MQTT_WITH_USERNAME_PASSWORD)
    /**
     * 
     * @param username
     * @param password
     * @return 
     */
    int setUserPassword(const char* username, const char* password = NULL);
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
    int setLwt( const char*    will_topic,
                const char*    will_message,
                enum MQTT::QoS will_qos      = MQTT::QOS0,
                bool           will_retain   = false);
#endif
    /**
     * 
     * @param topic
     * @param qos
     * @return 
     */
    int subscribe(const char* topic, enum MQTT::QoS qos = MQTT::QOS0);
    /**
     * 
     * @param topic
     * @return 
     */
    int unsubscribe(const char* topic);
    
    typedef enum {
        none = 0,
        // tcp
        tcp_connect,
        tcp_connect_in_progress,
        tcp_connect_done,
        tcp_connect_fail,
        tcp_disconnect,
        tcp_disconnect_in_progress,
        tcp_disconnect_done,
        tcp_disconnect_fail,
        // mqtt
        mqtt_connect,
        mqtt_connect_in_progress,
        mqtt_connect_done,
        mqtt_connect_fail,
        // other
        ready
    } state_t;

    state_t                         m_State;
    
    esp_open_rtos::wifi::wifi_t*    m_Wifi;
    
    char                            m_Hostname[MAX_HOSTNAME_SIZE + 1];
    int                             m_Port;
    char                            m_ClientId[MAX_CLIENT_ID_SIZE + 1];
    
#if defined(MQTT_WITH_USERNAME_PASSWORD)
    char                            m_Username[MAX_USERNAME_SIZE];
    char                            m_Password[MAX_PASSWORD_SIZE];
#endif
    
#if defined(MQTT_WITH_LWT)    
    char                            m_LwtTopic[MAX_TOPIC_SIZE];
    char                            m_LwtMessage[MAX_PAYLOAD_SIZE];
    //char                            m_LwtMessage[MAX_PACKET_SIZE - MAX_TOPIC_SIZE];
    unsigned char                   m_LwtRetained;
    enum MQTT::QoS                  m_LwtQos;
#endif
    
private:
    /**
     * 
     */
    void task(void);
    /**
     * 
     */
    virtual void on_connect()
    {}
    /**
     * 
     */
    virtual void on_disconnect()
    {}
    /**
     * 
     * @param topicName
     * @param message
     */
    //virtual void on_message(MQTTString& topicName, MQTT::Message& message)
    virtual void on_message(void* data)
    {}
    /**
     * 
     * @param topicName
     * @param message
     * @param userptr
     */
    static void _on_message(void* data, const void* userptr);
    //static void _on_message(MQTTString& topicName, MQTT::Message& message, const void* userptr);
    /**
     * 
     * @return 
     */
    state_t do_tcp_connect();
    /**
     * 
     * @return 
     */
    state_t do_tcp_disconnect();
    /**
     * 
     * @return 
     */
    state_t do_mqtt_connect();
  
    thread::mutex_t                 m_Publish_mutex;
    MQTT::Message                   m_Publish_message;
    char                            m_Publish_topic[MAX_TOPIC_SIZE];
    
    MQTT_t*                         m_Client;
    ip::ipstack_t*                  m_Ipstack;
    
    // no copy and no = operator
    mqtt_client_t(const mqtt_client_t&);
    mqtt_client_t &operator=(const mqtt_client_t&);    
};

/******************************************************************************************************************
 * class mqtt_payload_recv_t
 *
 */
class mqtt_payload_recv_t
{
public:
    typedef esp_open_rtos::thread::queue_buf_t<MAX_PACKET_SIZE> mqtt_queue_t;
    /**
     * 
     */
    mqtt_payload_recv_t() : m_Buffer(0)
    {
    }
    /**
     * 
     */
    ~mqtt_payload_recv_t()
    {
        if(m_Buffer != 0) {
            delete[] m_Buffer;
        }
    }
    /**
     * 
     * @param uxQueueLength
     * @return 
     */
    int init(unsigned portBASE_TYPE uxQueueLength = 2)
    {
        m_Buffer = new char[esp_open_rtos::mqtt::MAX_PACKET_SIZE];
        
        if(m_Buffer == 0) {
            return -2;
        }
        
        return m_Mqtt_queue.queue_create(uxQueueLength);
    }
    /**
     * 
     * @param ms
     * @return 
     */
    inline int receive(MQTTString& topicName, MQTT::Message& msg, unsigned long ms = 0) 
    {
        if(m_Mqtt_queue.receive(m_Buffer, ms) == 0) {
            //loghexdump(m_Buffer, 200);
            
            topicName = MQTTString_initializer;
            
            MQTTDeserialize_publish((unsigned char*)&msg.dup, 
                                    (int*)&msg.qos, 
                                    (unsigned char*)&msg.retained, 
                                    (unsigned short*)&msg.id, 
                                    &topicName,
                                    (unsigned char**)&msg.payload, 
                                    (int*)&msg.payloadlen, 
                                    (unsigned char*)m_Buffer, 
                                    MAX_PACKET_SIZE);
            
            return 0;
        }
        else {
            return -1;
        }
    }
    /**
     * 
     * @param topicFilter
     * @return 
     */
    bool isTopicMatched(const char* topicFilter)
    {
        return mqtt_client_t::isTopicMatched(topicFilter, ((MQTT::MessageData*)m_Buffer)->topicName);
    }
    /**
     * 
     * @return 
     */
    mqtt_queue_t& mqtt_queue()
    {
        return m_Mqtt_queue;
    }
    
protected:
    mqtt_queue_t    m_Mqtt_queue;
    char*           m_Buffer;
    
private:
    // no copy and no = operator
    mqtt_payload_recv_t(const mqtt_payload_recv_t&);
    mqtt_payload_recv_t &operator=(const mqtt_payload_recv_t&);    
};

/******************************************************************************************************************
 * class mqtt_client_queue_t
 *
 */
class mqtt_client_queue_t: public mqtt_client_t
{
public:
    /**
     * 
     */
    mqtt_client_queue_t()
    {}
    /**
     * 
     * @param mqtt_queue
     * @param wifi
     * @param broker
     * @param port
     * @param client_id
     * @return 
     */
    int init(mqtt_payload_recv_t::mqtt_queue_t& mqtt_queue, esp_open_rtos::wifi::wifi_t& wifi, const char* broker, int port = 1883, const char* client_id = NULL)
    {
        m_Mqtt_queue = mqtt_queue;
        
        return mqtt_client_t::init(wifi, broker, port, client_id);
    }

private:
    /**
     * 
     * @param md
     */
    void on_message(void* data)
    {
        //DTXT("mqtt_client_queue_t::(): begin\n");
        //loghexdump(data, 200);
        
        // send the message to other task so that we can concentrate on doing MQTT communication
        assert(m_Mqtt_queue.post(data) == 0);
        
        //DTXT("mqtt_client_queue_t::(): end\n");
    }
    
    mqtt_payload_recv_t::mqtt_queue_t m_Mqtt_queue;
};

} // namespace mqtt {
} // namespace esp_open_rtos {

#endif	/* ESP_OPEN_RTOS_MQTT_CLIENT_HPP */

