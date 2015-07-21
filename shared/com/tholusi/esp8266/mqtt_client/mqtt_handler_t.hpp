/* 
 * MQTT ESP8266 Slave Firmware
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

#ifndef COM_THOLUSI_ESP8266_MQTT_HANDLER_T_HPP
#define	COM_THOLUSI_ESP8266_MQTT_HANDLER_T_HPP

#include <com/tholusi/esp8266/task/task_t.hpp>
#include <com/github/esar/contiki-mqtt/mqtt_msg.h>
#include <com/tholusi/esp8266/errorcodes/errorcodes_t.hpp>
#include <com/tholusi/esp8266/cplusplus/includes.hpp>

#define MQTT_BUF_SIZE       1024
#define MQTT_BROKER_SIZE    64

#define MQTT_NO_RETAIN      false
#define MQTT_RETAIN         true

namespace com {
namespace tholusi {
namespace esp8266 {

/******************************************************************************************************************
 * mqtt_handler_t
 *
 */
class mqtt_handler_t
{
    friend class mqtt_client_t;
    friend void ICACHE_FLASH_ATTR _connect_callback(void *arg);
    friend void ICACHE_FLASH_ATTR _reconnect_callback(void *arg, sint8 err);
    friend void ICACHE_FLASH_ATTR _disconnect_callback(void *arg);
    friend void ICACHE_FLASH_ATTR _recv_callback(void *arg, char *pdata, unsigned short len);
    friend void ICACHE_FLASH_ATTR _sent_callback(void *arg);  
    friend void ICACHE_FLASH_ATTR _dns_found_callback(const char *name, ip_addr_t *ipaddr, void *arg);
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR mqtt_handler_t()
    {
    }
    /**
     * 
     */
    ICACHE_FLASH_ATTR ~mqtt_handler_t()
    {
    }

protected:
    typedef enum
    {
        status_none = 0,                        // 0
        status_tcp_connect_ip,                  // 1
        status_tcp_connect_dns,                 // 2
        status_tcp_connect_in_progress,         // 3
        status_tcp_connect_done,                // 4
        status_tcp_reconnect,                   // 5
        status_tcp_disconnected,                // 6
        status_sending_mqtt_connect,            // 7
        status_disconnect_in_progress,          // 8    command only
        status_disconnect_done,                 // 9
        status_mqtt_data,                       // 10
        status_ping,                            // 11
        status_ping_in_progress,                // 12
        status_ping_done,                       // 13
        status_publish,                         // 14   command only
        status_publish_in_progress_qos_0,       // 15
        status_publish_in_progress_qos_1_2,     // 16
        status_publish_done,                    // 17
        status_subscribe,                       // 18   command only
        status_subscribe_in_progress,           // 19
        status_subscribe_done,                  // 20
        status_unsubscribe,                     // 21   command only
        status_unsubscribe_in_progress,         // 22
        status_unsubscribe_done,                // 23
        status_dns_error,                       // 24
        status_unacceptable_protocol_version,   // 25   CONNACK 1
        status_identifier_rejected,             // 26   CONNACK 2
        status_server_unavailable,              // 27   CONNACK 3
        status_bad_username_or_password,        // 28   CONNACK 4
        status_not_authorized,                  // 29   CONNACK 5
        status_reserved                         // 30   CONACK reserved
    } status_t;
    /**
     * 
     * @param code
     * @param system
     * @param str
     * @param p1
     * @param p2
     * @param n1
     * @param n2
     */
    void ICACHE_FLASH_ATTR notification(int16_t code, const char* system, const char* str, const char* p1 = 0, const char* p2 = 0, int n1 = 0, int n2 = 0);
    /**
     * 
     * @param os
     * @param in_buffer_size
     * @param out_buffer_size
     * @return 
     */
    int ICACHE_FLASH_ATTR init(esp8266_t* os, size_t in_buffer_size = MQTT_BUF_SIZE, size_t out_buffer_size = MQTT_BUF_SIZE);
    /**
     * 
     * @param slotId
     * @param broker
     * @param port
     * @param client_id
     * @param username
     * @param password
     * @param keepalive
     * @param will_topic
     * @param will_message
     * @param will_qos
     * @param will_retain
     * @param clean_session
     * @param secure
     * @return 
     */
    int ICACHE_FLASH_ATTR connect(  int         slotId,
                                    const char* broker,
                                    uint16_t    port,
                                    const char* client_id,
                                    const char* username,
                                    const char* password,
                                    uint32_t    keepalive,
                                    const char* will_topic    = 0,
                                    const char* will_message  = 0,
                                    int         will_qos      = 0,
                                    bool        will_retain   = false,
                                    bool        clean_session = false,
                                    bool        secure        = false);
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR disconnect();
    /**
     * 
     * @param seconds
     * @return 
     */
    int ICACHE_FLASH_ATTR reset(uint32_t seconds);
    /**
     * 
     * @param topic
     * @param payload
     * @param payload_len
     * @param qos
     * @param retain
     * @return 
     */
    int ICACHE_FLASH_ATTR publish(const char* topic, const char* payload, int payload_len, int qos, bool retain);
    /**
     * 
     * @param topic
     * @param qos
     * @return 
     */
    int ICACHE_FLASH_ATTR subscribe(const char* topic, int qos);
    /**
     * 
     * @param topic
     * @return 
     */
    int ICACHE_FLASH_ATTR unsubscribe(const char* topic);
    /**
     * 
     * @return 
     */
    bool ICACHE_FLASH_ATTR isConnected();
    /**
     * 
     * @return 
     */
    bool ICACHE_FLASH_ATTR isReady()
    {
        return m_Status == status_mqtt_data ? true : false;
    }
    /**
     * 
     * @return 
     */
    bool ICACHE_FLASH_ATTR isActionTimeout();
    /**
     * 
     * @return 
     */
    inline ICACHE_FLASH_ATTR esp8266_t* os()
    {
        return m_Os;
    }
    
    uint8_t*                in_buffer;
    uint8_t*                out_buffer;
    int                     in_buffer_length;
    int                     out_buffer_length;
    uint16_t                message_length;
    uint16_t                message_length_read;
    mqtt_message_t*         outbound_message;
    mqtt_connection_t       mqtt_connection;
    uint16_t                pending_msg_id;
    int                     pending_msg_type;

    char                    m_Broker[MQTT_BROKER_SIZE];    
    uint16_t                m_Port;
    bool                    m_Secure;
    struct espconn          m_EspConn;
    
    volatile status_t       m_Status;
    volatile status_t       m_Command;
    mqtt_connect_info_t     m_MqttConnectInfo;
    
    esp8266_t*              m_Os;
    
    int                     m_SlotId;
    
    uint32_t                m_Seconds;
    uint32_t                m_LastRun;
    uint32_t                m_PingInterval;    
    uint32_t                m_PingStart, m_PingEnd;
    int                     m_PingFails;
    
    uint16_t                m_Reconnects;
    
    uint32_t                m_ActionStart;
    uint32_t                m_ActionTimeout;
    
    uint32_t                m_DnsErrors;
    
private:
    /**
     * 
     * @param seconds
     */
    void ICACHE_FLASH_ATTR run(uint32_t seconds);
    /**
     * 
     */
    void ICACHE_FLASH_ATTR dns();    
    /**
     * 
     * @param pdata
     * @param len
     */
    void ICACHE_FLASH_ATTR recv_callback(char* pdata, unsigned short len);
    /**
     * 
     */
    void ICACHE_FLASH_ATTR sent_callback();
    /**
     * 
     * @param name
     * @param ipaddr
     */
    void ICACHE_FLASH_ATTR dns_found_callback(const char* name, ip_addr_t* ipaddr);
    /**
     * 
     * @param arg
     */
    static void ICACHE_FLASH_ATTR _connect_callback(void* arg);
    /**
     * 
     * @param arg
     * @param err
     */
    static void ICACHE_FLASH_ATTR _reconnect_callback(void* arg, sint8 err);
    /**
     * 
     * @param arg
     */
    static void ICACHE_FLASH_ATTR _disconnect_callback(void* arg);
    /**
     * 
     * @param arg
     * @param pdata
     * @param len
     */
    static void ICACHE_FLASH_ATTR _recv_callback(void* arg, char* pdata, unsigned short len);
    /**
     * 
     * @param arg
     */
    static void ICACHE_FLASH_ATTR _sent_callback(void* arg);    
    /**
     * 
     * @param name
     * @param ipaddr
     * @param arg
     */
    static void ICACHE_FLASH_ATTR _dns_found_callback(const char* name, ip_addr_t* ipaddr, void* arg);
    
    // no copy and no = operator
    mqtt_handler_t(const mqtt_handler_t&);
    mqtt_handler_t &operator=(const mqtt_handler_t&);    
};

}
}
}

#endif	/* COM_THOLUSI_ESP8266_MQTT_HANDLER_T_HPP */

