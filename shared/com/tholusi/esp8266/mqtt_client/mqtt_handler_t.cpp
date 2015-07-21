/* 
 * MQTT ESP8266 Slave Firmware
 * Copyright (C) 2015  Michael Jacobsen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * ENOT_CONNECTED
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

#include <com/tholusi/esp8266/mqtt_client/mqtt_handler_t.hpp>
#include <com/github/taunpmt/esp_mqtt/utils/utils.h>
#include <com/tholusi/esp8266/debug/debug_t.hpp>
#include <com/tholusi/esp8266/cplusplus/includes.hpp>
#include <com/tholusi/esp8266/prototypes/osapi.h>

#undef  DTXT
#define DTXT(...)   os_printf(__VA_ARGS__)

/******************************************************************************************************************
 * ssl variables
 *
 */
unsigned char* default_certificate;
unsigned int   default_certificate_len = 0;
unsigned char* default_private_key;
unsigned int   default_private_key_len = 0;

using namespace com::tholusi::esp8266;

/******************************************************************************************************************
 * mqtt_handler_t
 *
 */

/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_handler_t::init(esp8266_t* os, size_t in_buffer_size, size_t out_buffer_size)
{
    m_Os                = os;
    m_SlotId            = -1;
    
    m_EspConn.proto.tcp = (esp_tcp*) os_zalloc(sizeof(esp_tcp));
    m_EspConn.reverse   = this;
    
    if(m_EspConn.proto.tcp == 0) {
        return EMEMORY;
    }
    
    in_buffer         = (uint8_t*) os_zalloc(in_buffer_size);
    in_buffer_length  = in_buffer_size;
    out_buffer        = (uint8_t*) os_zalloc(out_buffer_size);
    out_buffer_length = out_buffer_size;
    
    if(in_buffer == 0) {
        return EMEMORY;
    }
    
    if(out_buffer == 0) {
        return EMEMORY;
    }
    
    espconn_regist_connectcb(&m_EspConn, _connect_callback);
    espconn_regist_reconcb(  &m_EspConn, _reconnect_callback);
    
    m_Reconnects = 0;
    m_DnsErrors  = 0;
    
    return EOK;
}
/**
 * 
 * @param code
 * @param system
 * @param str
 * @param p1
 * @param p2
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::notification(int16_t code, const char* system, const char* str, const char* p1, const char* p2, int n1, int n2)
{
    if(os()->jsonRpc()->lock() == 0) {
        DTXT("mqtt_handler_t::notification() system = %s\n", system);
        
        /******************************************************************************************************************
         * build JSON RPC notification object
         *
         */        
        if(os()->jsonRpc()->getJEncoder()->set("{ss{dddssssdd}}",
                        "jsonrpc", "2.0",
                        "method",  "mqtt",
                        "params",
                            "seconds",  m_Seconds,
                            "id",       m_SlotId,
                            "code",     code,
                            "sys",      system,
                            "msg",      str,
                            "p1",       p1,
                            "p2",       p2,
                            "n1",       n1,
                            "n2",       n2) != 0) {
            os_printf(JSONRPC_ERR1);
        }

        if(os()->jsonRpc()->getJErr()->isError()) {
            os_printf(JSONRPC_ERR2);            
        }
        else if(os()->jsonRpc()->getJEncoder()->commit()) {            // this will activate _BufPrint_flush() which will then output the JSON data
            os_printf(JSONRPC_ERR2);
        }
        
        os()->jsonRpc()->unlock();
    }
}
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
int ICACHE_FLASH_ATTR mqtt_handler_t::connect(  int         slotId,
                                                const char* broker,
                                                uint16_t    port,
                                                const char* client_id,
                                                const char* username,
                                                const char* password,
                                                uint32_t    keepalive,
                                                const char* will_topic,
                                                const char* will_message,
                                                int         will_qos,
                                                bool        will_retain,
                                                bool        clean_session,
                                                bool        secure)

{
    if(client_id == NULL || client_id[0] == '\0') {
        return -1;
    }
    else {
        os_strcpy(m_MqttConnectInfo.client_id, client_id);
    }

    if(broker == NULL || broker[0] == '\0') {
        return -1;
    }
    else {
        os_strcpy(m_Broker, broker);
    }

    if(username == NULL || username[0] == '\0') {
        m_MqttConnectInfo.username[0] = '\0';
    }
    else {
        os_strcpy(m_MqttConnectInfo.username, username);
    }
    
    if(password == NULL || password[0] == '\0') {
        m_MqttConnectInfo.password[0] = '\0';
    }
    else {
        os_strcpy(m_MqttConnectInfo.password, password);
    }
    
    m_MqttConnectInfo.keepalive         = keepalive;
    m_PingInterval                      = keepalive * 1000;
    m_Port                              = port;
    m_Secure                            = secure;
    
    m_ActionTimeout                     = 5000;
    m_SlotId                            = slotId;
        
    m_EspConn.proto.tcp->remote_port    = m_Port;

    m_MqttConnectInfo.will_qos          = will_qos;
    m_MqttConnectInfo.will_retain       = will_retain;
    m_MqttConnectInfo.clean_session     = clean_session;

    if(m_MqttConnectInfo.will_topic != 0) {
        os_free(m_MqttConnectInfo.will_topic);
        m_MqttConnectInfo.will_topic = 0;
    }
    
    if(m_MqttConnectInfo.will_message != 0) {
        os_free(m_MqttConnectInfo.will_message);
        m_MqttConnectInfo.will_message = 0;
    }
    
    if(will_topic != 0 && will_topic[0] != '\0') {
        m_MqttConnectInfo.will_topic = (char*) os_malloc(os_strlen(will_topic) + 1);
        
        if(m_MqttConnectInfo.will_topic != 0) {
            os_strcpy(m_MqttConnectInfo.will_topic, will_topic);
        }
    }
    
    if(will_message != 0 && will_message[0] != '\0') {
        m_MqttConnectInfo.will_message = (char*) os_malloc(os_strlen(will_message) + 1);
        
        if(m_MqttConnectInfo.will_message != 0) {
            os_strcpy(m_MqttConnectInfo.will_message, will_message);
        }
    }
    
    m_EspConn.type                      = ESPCONN_TCP;
    m_EspConn.state                     = ESPCONN_NONE;
    m_EspConn.proto.tcp->local_port     = espconn_port();

    if(UTILS_StrToIP(m_Broker, m_EspConn.proto.tcp->remote_ip)) {
        DTXT("%lu mqtt_handler_t::connect(): connect to ip %s:%d\n", os()->millis(), m_Broker, m_Port);

        m_Status  = status_tcp_connect_ip;
        m_Command = status_tcp_connect_ip;
    }
    else {
        DTXT("%lu mqtt_handler_t::connect(): connect to FQDN %s:%d\n", os()->millis(), m_Broker, m_Port);

        dns();
        
        m_Command = status_tcp_connect_dns;
    }

    m_LastRun     = os()->millis();
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_handler_t::disconnect()
{
    if(m_Status != status_mqtt_data) {
        DTXT("%lu mqtt_handler_t::disconnect(): m_Status != status_mqtt_data\n", os()->millis());
        return ENOT_CONNECTED;
    }

    if(outbound_message != NULL) {
        DTXT("%lu mqtt_handler_t::disconnect(): outbound_message != NULL\n", os()->millis());
        return EOUTBOUNDBUSY;        
    }
    
    outbound_message = mqtt_msg_disconnect(&mqtt_connection);
    pending_msg_type = MQTT_MSG_TYPE_DISCONNECT;
    
    DTXT("%lu mqtt_handler_t::disconnect(): pending_msg_id = %u\n", os()->millis(), pending_msg_id);
    
    m_Command     = status_disconnect_in_progress;
    m_Status      = status_disconnect_in_progress;
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @param seconds
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_handler_t::reset(uint32_t seconds)
{
    m_Seconds                           = seconds;
    m_EspConn.type                      = ESPCONN_TCP;
    m_EspConn.state                     = ESPCONN_NONE;
    m_EspConn.proto.tcp->local_port     = espconn_port();
    
    if(UTILS_StrToIP(m_Broker, m_EspConn.proto.tcp->remote_ip)) {
        DTXT("%lu mqtt_handler_t::connect(): connect to ip %s:%d\n", os()->millis(), m_Broker, m_Port);

        m_Status  = status_tcp_connect_ip;
        m_Command = status_tcp_connect_ip;
    }
    else {
        dns();
        
        m_Command = status_tcp_connect_dns;
    }
    
    m_LastRun     = os()->millis();
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @return 
 */
bool ICACHE_FLASH_ATTR mqtt_handler_t::isConnected()
{
    switch(m_Status) {
        case status_mqtt_data:
        case status_publish_in_progress_qos_0:
        case status_publish_in_progress_qos_1_2:
        case status_publish_done:
        case status_subscribe_in_progress:
        case status_subscribe_done:
            return true;
            
        default:
            return false;
    }
}
/**
 * 
 * @return 
 */
bool ICACHE_FLASH_ATTR mqtt_handler_t::isActionTimeout()
{
    if(os()->millis() - m_ActionStart >= m_ActionTimeout) {
        return true;
    }

    return false;
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
int ICACHE_FLASH_ATTR mqtt_handler_t::publish(const char* topic, const char* payload, int payload_len, int qos, bool retain)
{
    if(topic == 0 || payload == 0 || topic[0] == '\0') {
        DTXT("%lu mqtt_handler_t::publish(): no topic or payload\n", os()->millis());
        return EMISSING_PARAMETER;        
    }
    
    if(m_Status != status_mqtt_data) {
        if(m_Status == status_ping_in_progress) {
            DTXT("%lu mqtt_handler_t::publish(): m_Status == status_ping_in_progress\n", os()->millis());
            return EPING_IN_PROGRESS;
        }
        else {
            DTXT("%lu mqtt_handler_t::publish(): m_Status != status_mqtt_data\n", os()->millis());
            return ENOT_CONNECTED;
        }
    }

    if(outbound_message != NULL) {
        os_printf("%lu mqtt_handler_t::publish(): outbound_message != NULL\n", os()->millis());
        return EOUTBOUNDBUSY;        
    }
    
    if(qos == 1 || qos == 2) {
        m_Status = status_publish_in_progress_qos_1_2;
    }
    else {
        qos = 0;
        m_Status = status_publish_in_progress_qos_0;
    }
    
    outbound_message = mqtt_msg_publish(&mqtt_connection, topic, payload, payload_len, qos, retain == true ? 1 : 0, &pending_msg_id);
    pending_msg_type = MQTT_MSG_TYPE_PUBLISH;    

    DTXT("%lu mqtt_handler_t::publish(): pending_msg_id = %u, qos = %d\n", os()->millis(), pending_msg_id, qos);
    
    m_Command     = status_publish;
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @param topic
 * @param qos
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_handler_t::subscribe(const char* topic, int qos)
{
    if(topic == 0) {
        DTXT("%lu mqtt_handler_t::subscribe(): no topic\n", os()->millis());
        return EMISSING_PARAMETER;        
    }
    
    if(m_Status != status_mqtt_data) {
        DTXT("%lu mqtt_handler_t::subscribe(): m_Status != status_mqtt_data\n", os()->millis());
        return ENOT_CONNECTED;
    }

    if(outbound_message != NULL) {
        DTXT("%lu mqtt_handler_t::subscribe(): outbound_message != NULL\n", os()->millis());
        return EOUTBOUNDBUSY;        
    }
    
    outbound_message = mqtt_msg_subscribe(&mqtt_connection, topic, qos, &pending_msg_id);
    pending_msg_type = MQTT_MSG_TYPE_SUBSCRIBE;

    DTXT("%lu mqtt_handler_t::subscribe(): pending_msg_id = %u, qos = %d\n", os()->millis(), pending_msg_id, qos);
    
    m_Status = status_subscribe_in_progress;
    
    m_Command     = status_subscribe;
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_handler_t::unsubscribe(const char* topic)
{
    if(topic == 0) {
        DTXT("%lu mqtt_handler_t::unsubscribe(): no topic\n", os()->millis());
        return EMISSING_PARAMETER;        
    }
    
    if(m_Status != status_mqtt_data) {
        DTXT("%lu mqtt_handler_t::unsubscribe(): m_Status != status_mqtt_data\n", os()->millis());
        return ENOT_CONNECTED;
    }

    if(outbound_message != NULL) {
        DTXT("%lu mqtt_handler_t::unsubscribe(): outbound_message != NULL\n", os()->millis());
        return EOUTBOUNDBUSY;        
    }
    
    outbound_message = mqtt_msg_unsubscribe(&mqtt_connection, topic, &pending_msg_id);
    pending_msg_type = MQTT_MSG_TYPE_UNSUBACK;

    DTXT("%lu mqtt_handler_t::unsubscribe(): pending_msg_id = %u\n", os()->millis(), pending_msg_id);
    
    m_Status      = status_unsubscribe_in_progress;
    m_Command     = status_unsubscribe;
    m_ActionStart = os()->millis();
    
    return EOK;
}
/**
 * 
 * @param seconds
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::run(uint32_t seconds)
{
    m_Seconds = seconds;

    /*if(m_PingStart != 0 && m_PingEnd == 0) {
        if(os()->millis() - m_PingStart >= 1000) {
            m_PingStart = 0;
            ++m_PingFails;

            notification(EPING_TIMEOUT, "ping", "error: timeout", m_Broker, 0, 0, m_PingFails);
        }
    }*/
        
    if(outbound_message == NULL) {
        // is it time to do a ping
        if(m_Status == status_mqtt_data && (os()->millis() - m_LastRun >= m_PingInterval)) {
            DTXT("%lu mqtt_handler_t::run(): ping time\n", os()->millis());

            outbound_message = mqtt_msg_pingreq(&mqtt_connection);

            m_Status         = status_ping_in_progress;
            m_Command        = status_ping;
            m_ActionStart    = m_LastRun = os()->millis();

            //m_LastRun        = m_PingStart = os()->millis();
            //m_PingEnd        = 0;
        }
    }
    
    switch(m_Command) {
        case status_tcp_connect_ip:
        case status_tcp_connect_dns:
            if(m_Status != status_mqtt_data) {
                if(isActionTimeout()) {
                    notification(ETIMEOUT, "connect", "error: timeout", m_Broker);
                
                    m_ActionStart = os()->millis();     // start again
                }
            }
            break;

        case status_ping:
            if(m_Status != status_ping_done) {
                if(isActionTimeout()) {
                    notification(EPING_TIMEOUT, "ping", "error: timeout", m_Broker, 0, 0, m_PingFails);

                    ++m_PingFails;
                    
                    m_Status  = status_mqtt_data;
                    m_Command = status_mqtt_data;
                }
            }
            else {
                m_Status  = status_mqtt_data;
                m_Command = status_mqtt_data;
            }
            break;
            
        case status_publish:
            if(m_Status != status_publish_done) {
                if(isActionTimeout()) {
                    notification(ETIMEOUT, "pub", "error: timeout");

                    m_Status  = status_mqtt_data;
                    m_Command = status_mqtt_data;
                }
            }
            else {
                notification(EPUB_DONE, "pub", "done");

                m_Status  = status_mqtt_data;
                m_Command = status_mqtt_data;
            }
            break;

        case status_subscribe:
            if(m_Status != status_subscribe_done) {
                if(isActionTimeout()) {
                    notification(ETIMEOUT, "sub", "error: timeout");

                    m_Status  = status_mqtt_data;
                    m_Command = status_mqtt_data;
                }
            }
            else {
                notification(ESUB_DONE, "sub", "done");

                m_Status  = status_mqtt_data;
                m_Command = status_mqtt_data;
            }
            break;
            
        case status_unsubscribe:
            if(m_Status != status_unsubscribe_done) {
                if(isActionTimeout()) {
                    notification(ETIMEOUT, "unsub", "error: timeout");

                    m_Status  = status_mqtt_data;
                    m_Command = status_mqtt_data;
                }
            }
            else {
                notification(EUSUB_DONE, "unsub", "done");

                m_Status  = status_mqtt_data;
                m_Command = status_mqtt_data;
            }
            break;
            
        default:
            break;
    }
    
    switch(m_Status) {
        case status_tcp_connect_ip:
            os_printf("%lu mqtt_handler_t::run(): status_tcp_connect_ip\n", os()->millis());        

            if(m_Secure) {
                //espconn_secure_connect(&m_EspConn);
            }
            else {
                espconn_connect(&m_EspConn);
            }

            //m_LastRun = os()->millis();
            m_ActionStart = os()->millis();
            m_Status      = status_tcp_connect_in_progress;        
            break;
            
        case status_tcp_connect_dns:
            DTXT("%lu mqtt_handler_t::run(): status_tcp_connect_dns\n", os()->millis());        
            break;
            
        case status_dns_error:
            DTXT("%lu mqtt_handler_t::run(): status_dns_error\n", os()->millis());

            m_Status                       = status_none;
            m_SlotId                       = -1;
            m_MqttConnectInfo.client_id[0] = '\0';              // mark this one as free            

            //dns();                          // try again
            
            notification(EDNS, "connect", "error: dns", m_Broker);            
            break;
            
        case status_tcp_reconnect:
            os_printf("%lu mqtt_handler_t::run(): status_tcp_reconnect\n", os()->millis());

            m_EspConn.type                  = ESPCONN_TCP;
            m_EspConn.state                 = ESPCONN_NONE;
            m_EspConn.proto.tcp->local_port = espconn_port();

            m_ActionStart                   = os()->millis();
            m_Status                        = status_tcp_connect_ip;        
            
            notification(ERECONNECT, "connect", "error: reconnect", m_Broker, 0, m_Reconnects, m_PingFails);

            m_PingFails                     = 0;
            break;
            
        case status_tcp_disconnected:
            DTXT("%lu mqtt_handler_t::run(): status_tcp_disconnected\n", os()->millis());

            m_EspConn.type                  = ESPCONN_TCP;
            m_EspConn.state                 = ESPCONN_NONE;
            m_EspConn.proto.tcp->local_port = espconn_port();

            m_ActionStart                   = os()->millis();
            m_Status                        = status_tcp_connect_ip;        

            notification(EDISCONNECTED, "connect", "error: disconnected", m_Broker);
            break;
            
        case status_tcp_connect_in_progress:
            DTXT("%lu mqtt_handler_t::run(): status_tcp_connect_in_progress\n", os()->millis());
            break;
            
        case status_tcp_connect_done:
            DTXT("%lu mqtt_handler_t::run(): status_tcp_connect_done\n", os()->millis());
            break;
            
        case status_disconnect_done:
            DTXT("%lu mqtt_handler_t::run(): status_disconnect_done\n", os()->millis());        

            if(m_Secure) {
                //espconn_secure_disconnect(&m_EspConn);
            }
            else {
                espconn_disconnect(&m_EspConn);
            }
            
            notification(EDISCONNECT_DONE, "disconnect", "done");
            
            m_Status                       = status_none;
            m_SlotId                       = -1;
            m_MqttConnectInfo.client_id[0] = '\0';              // mark this one as free            
            return;
            
        case status_unacceptable_protocol_version:
        case status_identifier_rejected:
        case status_server_unavailable:
        case status_bad_username_or_password:
        case status_not_authorized:
        case status_reserved:
            m_Status                       = status_none;
            m_SlotId                       = -1;
            m_MqttConnectInfo.client_id[0] = '\0';              // mark this one as free
            return;
            
        default:
            break;
    }
        
    if(outbound_message != NULL){
        DTXT("%lu mqtt_handler_t::run(): outbound_message->length = %u\n", os()->millis(), outbound_message->length);
        
        if(m_Secure) {
            //espconn_secure_sent(&m_EspConn, outbound_message->data, outbound_message->length);
        }
        else {
            espconn_sent(&m_EspConn, outbound_message->data, outbound_message->length);
        }
        
        outbound_message = NULL;
    }
    /*else {
        if(m_PingStart != 0 && m_PingEnd == 0) {
            if(os()->millis() - m_PingStart >= 1000) {
                m_PingStart = 0;
                ++m_PingFails;
                
                notification(EPING_TIMEOUT, "ping", "error: timeout", m_Broker, 0, 0, m_PingFails);
            }
        }
        
        // is it time to do a ping
        if(m_Status == status_mqtt_data && (os()->millis() - m_LastRun >= m_PingInterval)) {
            DTXT("%lu mqtt_handler_t::run(): ping time\n", os()->millis());
            
            outbound_message = mqtt_msg_pingreq(&mqtt_connection);
            
            if(m_Secure) {
                //espconn_secure_sent(&m_EspConn, outbound_message->data, outbound_message->length);
            }
            else {
                espconn_sent(&m_EspConn, outbound_message->data, outbound_message->length);
            }

            outbound_message = NULL;
            m_LastRun        = m_PingStart = os()->millis();
            m_PingEnd        = 0;
        }
    }*/
}
/**
 * 
 * @param arg
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_connect_callback(void* arg)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    os_printf("%lu mqtt_handler_t::_connect_callback()\n", me->os()->millis());
    
    espconn_regist_disconcb(&me->m_EspConn, me->_disconnect_callback);
    espconn_regist_recvcb(  &me->m_EspConn, me->_recv_callback);
    espconn_regist_sentcb(  &me->m_EspConn, me->_sent_callback);
    
    // prepare to connect on MQTT level
    mqtt_msg_init(&me->mqtt_connection, me->out_buffer, me->out_buffer_length);
    
    me->outbound_message = mqtt_msg_connect(&me->mqtt_connection, &me->m_MqttConnectInfo);
    me->m_Status         = status_sending_mqtt_connect;    
}
/**
 * 
 * @param arg
 * @param err
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_reconnect_callback(void* arg, sint8 err)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    os_printf("%lu mqtt_handler_t::_reconnect_callback()\n", me->os()->millis());
    
    espconn_regist_disconcb(&me->m_EspConn, me->_disconnect_callback);
    espconn_regist_recvcb(  &me->m_EspConn, me->_recv_callback);
    espconn_regist_sentcb(  &me->m_EspConn, me->_sent_callback);
    
    me->m_Status = status_tcp_reconnect;
    
    me->m_Reconnects++;
}
/**
 * 
 * @param arg
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_disconnect_callback(void* arg)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    DTXT("%lu mqtt_handler_t::_disconnect_callback()\n", me->os()->millis());
    
    switch(me->m_Status) {
        case status_disconnect_in_progress:
        case status_disconnect_done:
        case status_unacceptable_protocol_version:
        case status_identifier_rejected:
        case status_server_unavailable:
        case status_bad_username_or_password:
        case status_not_authorized:
        case status_reserved:
            break;
            
        default:
            me->m_Status = status_tcp_disconnected;
            break;
    }
}
/**
 * 
 * @param arg
 * @param pdata
 * @param len
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_recv_callback(void* arg, char* pdata, unsigned short len)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    DTXT("%lu mqtt_handler_t::_recv_callback()\n", me->os()->millis());
    
    me->recv_callback(pdata, len);
}
/**
 * 
 * @param pdata
 * @param len
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::recv_callback(char* pdata, unsigned short len)
{
    if(len > in_buffer_length) {
        DTXT("%lu mqtt_handler_t::recv_callback() error: len > in_buffer_length; len = %d, in_buffer_length = %d\n", os()->millis(), len, in_buffer_length);
        notification(EPACKET_SIZE, "data", "error: packet len > in_buffer_length", m_Broker, 0, len, in_buffer_length);
    }
    else if(len > 0) {
        uint8_t  msg_type;
	uint8_t  msg_qos;
	uint16_t msg_id;

        // copy to mqtt in buffer
        os_memcpy(in_buffer, pdata, len);
        
        switch(m_Status) {
            case status_sending_mqtt_connect:
                if(mqtt_get_type(in_buffer) != MQTT_MSG_TYPE_CONNACK){
                    notification(EINVALID_PACKET, "connect", "error: invalid packet", m_Broker);

                    if(m_Secure) {
                        //espconn_secure_disconnect(&m_EspConn);
                    }
                    else {
                        espconn_disconnect(&m_EspConn);
                    }
                }
                else {
                    //
                    // note: when the connection return code is not 0 (ok), the server disconnects us
                    //
                    switch(mqtt_get_conn_return(in_buffer)) {
                        case 0:
                            DTXT("%lu mqtt_handler_t::recv_callback(): connected to %s:%d\n", os()->millis(), m_Broker, m_Port);
                            notification(ECONNECT_DONE, "connect", "ok", m_Broker);
                            m_Status  = status_mqtt_data;
                            m_Command = status_mqtt_data;
                            break;
                            
                        case 1:
                            notification(EPROTOCOL_VERSION, "connect", "error: unacceptable_protocol_version", m_Broker);
                            m_Status = status_unacceptable_protocol_version;
                            
                        case 2:
                            notification(EIDENTIFIER_REJECTED, "connect", "error: identifier_rejected", m_Broker);
                            m_Status = status_identifier_rejected;
                            break;
                            
                        case 3:
                            notification(ESERVER_UNAVAILABLE, "connect", "error: server_unavailable", m_Broker);
                            m_Status = status_server_unavailable;
                            break;
                        
                        case 4:
                            notification(EBAD_USERNAME_PASSWORD, "connect", "error: bad_username_or_password", m_Broker);
                            m_Status = status_bad_username_or_password;
                            break;
                                    
                        case 5:
                            notification(ENOT_AUTHORIZED, "connect", "error: not_authorized", m_Broker);
                            m_Status = status_not_authorized;
                            break;
                            
                        default:
                            notification(ERESERVED, "connect", "error: reserved", m_Broker);
                            m_Status = status_reserved;
                            break;
                    }
                }
                break;
                
            case status_mqtt_data:
            case status_ping_in_progress:
            case status_publish_in_progress_qos_1_2:
            case status_subscribe_in_progress:
            case status_unsubscribe_in_progress:
                message_length_read = len;
                message_length      = mqtt_get_total_length(in_buffer, message_length_read);
                
                msg_type            = mqtt_get_type(in_buffer);
                msg_qos             = mqtt_get_qos(in_buffer);
                msg_id              = mqtt_get_id(in_buffer, in_buffer_length);
                
                switch(msg_type) {
                    case MQTT_MSG_TYPE_PINGRESP:                        
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PINGRESP (freeHeap: %lu, m_Reconnects = %u)\n", os()->millis(), os()->freeHeap(), m_Reconnects);        
                        //m_PingEnd   = os()->millis();
                        m_Status    = status_ping_done;
                        m_PingFails = 0;
                        break;
                        
                    case MQTT_MSG_TYPE_PINGREQ:
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PINGREQ\n", os()->millis());
                        outbound_message = mqtt_msg_pingresp(&mqtt_connection);
                        break;

                    case MQTT_MSG_TYPE_PUBREC:                                                                              // QoS = 2
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBREC (msg_id = %u, pending_msg_id = %u)\n", os()->millis(), msg_id, pending_msg_id);
                        
                        if(pending_msg_type == MQTT_MSG_TYPE_PUBLISH && pending_msg_id == msg_id) {
                            outbound_message = mqtt_msg_pubrel(&mqtt_connection, msg_id);
                            pending_msg_type = MQTT_MSG_TYPE_PUBCOMP;
                        }
                        else {
                            m_Status = status_mqtt_data;
                        }
                        break;
                        
                    case MQTT_MSG_TYPE_PUBREL:                                                                              // QoS = 2
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBREL (msg_id = %u, pending_msg_id = %u)\n", os()->millis(), msg_id, pending_msg_id);
                        outbound_message = mqtt_msg_pubcomp(&mqtt_connection, msg_id);
                        break;
                        
                    case MQTT_MSG_TYPE_PUBCOMP:                                                                             // QoS = 2
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBCOMP (msg_id = %u, pending_msg_id = %u, pending_msg_type = %d)\n", os()->millis(), msg_id, pending_msg_id, pending_msg_type);
                        
                        if(pending_msg_type == MQTT_MSG_TYPE_PUBCOMP && pending_msg_id == msg_id) {
                            DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBCOMP QoS 2 publish success\n", os()->millis());
                            
                            m_Status = status_publish_done;
                        }
                        else {
                            m_Status = status_mqtt_data;
                        }
                        break;
                        
                    case MQTT_MSG_TYPE_PUBACK:                                                                              // QoS = 1
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBACK\n", os()->millis());
                        
                        if(pending_msg_type == MQTT_MSG_TYPE_PUBLISH && pending_msg_id == msg_id){
                            DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBACK publish success (QoS = %d)\n", os()->millis(), msg_qos);
                            
                            m_Status = status_publish_done;
                        }
                        else {
                            m_Status = status_mqtt_data;
                        }
                        break;
                        
                    case MQTT_MSG_TYPE_SUBACK:
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_SUBACK\n", os()->millis());
                        
                        if(pending_msg_type == MQTT_MSG_TYPE_SUBSCRIBE && pending_msg_id == msg_id){
                            DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_SUBSCRIBE subscribe success (QoS = %d)\n", os()->millis(), msg_qos);
                            
                            m_Status = status_subscribe_done;
                        }
                        else {
                            m_Status = status_mqtt_data;
                        }
                        break;
                        
                    case MQTT_MSG_TYPE_UNSUBACK:
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_UNSUBACK\n", os()->millis());
                        
                        if(pending_msg_type == MQTT_MSG_TYPE_UNSUBACK && pending_msg_id == msg_id){
                            DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_UNSUBACK unsubscribe success\n", os()->millis());
                            
                            m_Status = status_unsubscribe_done;
                        }
                        else {
                            m_Status = status_mqtt_data;
                        }
                        break;
                        
                    case MQTT_MSG_TYPE_PUBLISH:
                        {
                        DTXT("%lu mqtt_handler_t::recv_callback(): MQTT_MSG_TYPE_PUBLISH\n", os()->millis());
                        
                        if(msg_qos == 1) {
                            outbound_message = mqtt_msg_puback(&mqtt_connection, msg_id);
                        }
                        else if(msg_qos == 2) {
                            outbound_message = mqtt_msg_pubrec(&mqtt_connection, msg_id);
                        }

                        uint16_t    topic_length = len;
                        const char* topic        = mqtt_get_publish_topic(in_buffer, &topic_length);

                        uint16_t    data_length  = len;
                        const char* data         = mqtt_get_publish_data(in_buffer, &data_length);

                        os_memmove((char*)data + 1, (char*)data, data_length);
                        data += 1;
                        
                        ((char*)topic)[topic_length] = '\0';
                        ((char*)data) [data_length]  = '\0';

                        notification(EPUB_DATA, "pub", 0, topic, data);

                        DTXT("%lu mqtt_handler_t::recv_callback(): topic = %s\n", os()->millis(), topic);
                        DTXT("%lu mqtt_handler_t::recv_callback(): data  = %s\n", os()->millis(), data);
                        }
                        break;
                        
                    default:
                        DTXT("%lu mqtt_handler_t::recv_callback(): msg_type = %02X\n", os()->millis(), msg_type);        
                        break;
                }
                break;
              
            default:
                DTXT("%lu mqtt_handler_t::recv_callback(): default\n", os()->millis());
                break;
        }
    }
    else {
        DTXT("%lu mqtt_handler_t::recv_callback() error: len == 0\n", os()->millis());        
    }
}
/**
 * 
 * @param arg
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_sent_callback(void* arg)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    DTXT("%lu mqtt_handler_t::_sent_callback() begin\n", me->os()->millis());
    
    me->sent_callback();
    
    DTXT("%lu mqtt_handler_t::_sent_callback() end\n", me->os()->millis());
}
/**
 * 
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::sent_callback()
{
    DTXT("%lu mqtt_handler_t::sent_callback() begin; freeHeap: %lu\n", os()->millis(), os()->freeHeap());
    
    if(m_Status == status_publish_in_progress_qos_0) {
        DTXT("%lu mqtt_handler_t::sent_callback(): m_Status == status_publish_in_progress_qos_0\n", os()->millis());
        m_Status = status_publish_done;
    }
    else if(m_Status == status_disconnect_in_progress) {
        DTXT("%lu mqtt_handler_t::sent_callback(): m_Status == status_disconnect_in_progress\n", os()->millis());
        m_Status = status_disconnect_done;
    }
    else {
        DTXT("%lu mqtt_handler_t::sent_callback(): m_Command = %d, m_Status = %d\n", os()->millis(), m_Command, m_Status);        
    }
    
    DTXT("%lu mqtt_handler_t::sent_callback() end; freeHeap: %lu\n", os()->millis(), os()->freeHeap());
}
/**
 * 
 * @param name
 * @param ipaddr
 * @param arg
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::_dns_found_callback(const char* name, ip_addr_t* ipaddr, void* arg)
{
    mqtt_handler_t* me = (mqtt_handler_t*)((struct espconn*) arg)->reverse;
    DTXT("%lu mqtt_handler_t::_dns_found_callback()\n", me->os()->millis());
    
    me->dns_found_callback(name, ipaddr);    
}
/**
 * 
 * @param name
 * @param ipaddr
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::dns_found_callback(const char* name, ip_addr_t* ipaddr)
{
    if(ipaddr == 0) {
        DTXT("%lu mqtt_handler_t::dns_found_callback(): ipaddr == 0\n", os()->millis());
        
        ++m_DnsErrors;
        
        m_LastRun     = os()->millis();
        m_ActionStart = os()->millis();
        m_Status      = status_dns_error;
    }
    else {
        DTXT("%lu mqtt_handler_t::dns_found_callback(): ipaddr = %d.%d.%d.%d\n", os()->millis(), *((uint8 *) &ipaddr->addr), *((uint8 *) &ipaddr->addr + 1), *((uint8 *) &ipaddr->addr + 2), *((uint8 *) &ipaddr->addr + 3));
        
        os_memcpy(m_EspConn.proto.tcp->remote_ip, &ipaddr->addr, 4);
        m_Status = status_tcp_connect_ip;
    }
}
/**
 * 
 */
void ICACHE_FLASH_ATTR mqtt_handler_t::dns()
{
    ip_addr_t ipaddr;

    switch(espconn_gethostbyname(&m_EspConn, m_Broker, &ipaddr, _dns_found_callback)) {
        case ESPCONN_OK:                // if hostname is a valid IP address string or the host name is already in the local names table.
            DTXT("mqtt_handler_t::dns(): ESPCONN_OK\n");
            
            os_memcpy(m_EspConn.proto.tcp->remote_ip, &ipaddr.addr, 4);
            m_Status = status_tcp_connect_ip;
            break;

        case ESPCONN_INPROGRESS:        // enqueue a request to be sent to the DNS server for resolution if no errors are present.
            DTXT("mqtt_handler_t::dns(): ESPCONN_INPROGRESS\n");
            m_Status  = status_tcp_connect_dns;
            break;

        case ESPCONN_ARG:               // dns client not initialized or invalid hostname
            DTXT("mqtt_handler_t::dns(): ESPCONN_ARG\n");
            m_Status = status_dns_error;
            break;

        default:
            DTXT("mqtt_handler_t::dns(): (unknown)\n");
            m_Status = status_dns_error;
            break;
    }
}