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

#include <com/tholusi/esp8266/mqtt_client/mqtt_client_t.hpp>
#include <com/github/taunpmt/esp_mqtt/utils/utils.h>
#include <com/tholusi/esp8266/prototypes/osapi.h>
#include <com/tholusi/esp8266/debug/debug_t.hpp>

using namespace com::tholusi::esp8266;

/******************************************************************************************************************
 * mqtt_client
 *
 */

/**
 * 
 * @param seconds
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::run(uint32_t seconds)
{
    bool wifi = m_Wifi->isConnected();
    
    // go thru all connection slots
    for(int i = 0; i < N_CONNECTIONS; ++i) {
        if(wifi) {                                                              // we have WiFi connection
            if(m_MqttHandler[i].m_MqttConnectInfo.client_id[0] != 0) {          // found an configured broker
                m_MqttHandler[i].run(seconds);
            }
        }
        else {                                                                  // no WiFi connection
            if(m_MqttHandler[i].m_MqttConnectInfo.client_id[0] != 0) {          // found an configured broker
                m_MqttHandler[i].reset(seconds);
            }            
        }
    }
    
    return EOK;
}
/**
 * 
 * @param os
 * @param wifi
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::setup(com::tholusi::esp8266::esp8266_t* os, wifi_t* wifi)
{
    if(os == 0) {
        return -1;
    }
    
    if(wifi == 0) {
        return -2;
    }
    
    m_Os          = os;
    m_Wifi        = wifi;
    m_MqttHandler = (mqtt_handler_t*) os_zalloc(N_CONNECTIONS * sizeof(mqtt_handler_t));

    if(m_MqttHandler == 0) {
        os_printf(JSONRPC_ERR5, "m_MqttHandler");
        return -2;
    }

    for(int i = 0; i < N_CONNECTIONS; ++i) {
        if(m_MqttHandler[i].init(m_Os) != 0) {
            os_printf(JSONRPC_ERR7, i);
        }
    }
    
    return EOK;
}
/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::getFreeSlot()
{
    int i;
    
    // find a free slot
    for(i = 0; i < N_CONNECTIONS; ++i) {
        if(m_MqttHandler[i].m_MqttConnectInfo.client_id[0] == 0) {
            return i;       // this is the slot id
        }
    }
    
    return EAVAILABLE_SLOT;
}
/**
 * 
 * @param slot
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::isUsed(int slot)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] != 0) {
        return 1;
    }
    
    return 0;
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
int ICACHE_FLASH_ATTR mqtt_client_t::connect(   int         slotId,
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
    return m_MqttHandler[slotId].connect(slotId, broker, port, client_id, username, password, keepalive, will_topic, will_message, will_qos, will_retain, clean_session, secure);
}
/**
 * 
 * @param slot
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::disconnect(int slot)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used
    }
    
    return m_MqttHandler[slot].disconnect();
}
/**
 * 
 * @param slot
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::isConnected(int slot)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used
    }
    
    return m_MqttHandler[slot].isConnected() ? 1 : 0;
}
/**
 * 
 * @param slot
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::isReady(int slot)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used
    }
    
    if(m_MqttHandler[slot].isConnected() == false) {
        return ENOT_CONNECTED;
    }
    
    return m_MqttHandler[slot].isReady() == true ? 1 : 0;
}
/**
 * 
 * @param slot
 * @param topic
 * @param payload
 * @param payload_len
 * @param qos
 * @param retain
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::publish(int slot, const char* topic, const char* payload, int payload_len, int qos, bool retain)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used        
    }
    
    return m_MqttHandler[slot].publish(topic, payload, payload_len, qos, retain);
}
/**
 * 
 * @param slot
 * @param topic
 * @param qos
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::subscribe(int slot, const char* topic, int qos)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used        
    }
    
    return m_MqttHandler[slot].subscribe(topic, qos);
}
/**
 * 
 * @param slot
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR mqtt_client_t::unsubscribe(int slot, const char* topic)
{
    if(m_MqttHandler[slot].m_MqttConnectInfo.client_id[0] == 0) {
        return EID;      // not used        
    }
    
    return m_MqttHandler[slot].unsubscribe(topic);
}

