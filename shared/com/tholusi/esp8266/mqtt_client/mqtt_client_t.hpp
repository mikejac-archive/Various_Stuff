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

#ifndef COM_THOLUSI_ESP8266_MQTT_CLIENT_T_HPP
#define	COM_THOLUSI_ESP8266_MQTT_CLIENT_T_HPP

#include <com/tholusi/esp8266/mqtt_client/mqtt_handler_t.hpp>
#include <com/tholusi/esp8266/errorcodes/errorcodes_t.hpp>
#include <com/tholusi/esp8266/task/task_t.hpp>
#include <com/tholusi/esp8266/wifi/wifi_t.hpp>

#define N_CONNECTIONS       4

namespace com {
namespace tholusi {
namespace esp8266 {

/******************************************************************************************************************
 * mqtt_client_t
 *
 */
class mqtt_client_t
{
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR mqtt_client_t()
    {
    }
    /**
     * 
     */
    ICACHE_FLASH_ATTR ~mqtt_client_t()
    {
    }
    /**
     * 
     * @param seconds
     * @return 
     */
    int ICACHE_FLASH_ATTR run(uint32_t seconds);
    /**
     * 
     * @param os
     * @param wifi
     * @return 
     */
    int ICACHE_FLASH_ATTR setup(com::tholusi::esp8266::esp8266_t* os, wifi_t* wifi);
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR getFreeSlot();
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR getMaxConnections()
    {
        return N_CONNECTIONS;
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
     * @param slot
     * @return 
     */
    int ICACHE_FLASH_ATTR disconnect(int slot);
    /**
     * 
     * @param slot
     * @return 
     */
    int ICACHE_FLASH_ATTR isUsed(int slot);
    /**
     * 
     * @param slot
     * @return 
     */
    int ICACHE_FLASH_ATTR isConnected(int slot);
    /**
     * 
     * @param slot
     * @return 
     */
    int ICACHE_FLASH_ATTR isReady(int slot);
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
    int ICACHE_FLASH_ATTR publish(int slot, const char* topic, const char* payload, int payload_len, int qos, bool retain);
    /**
     * 
     * @param slot
     * @param topic
     * @param qos
     * @return 
     */
    int ICACHE_FLASH_ATTR subscribe(int slot, const char* topic, int qos);
    /**
     * 
     * @param slot
     * @param topic
     * @return 
     */
    int ICACHE_FLASH_ATTR unsubscribe(int slot, const char* topic);
    
protected:
    mqtt_handler_t*                     m_MqttHandler;
    
    com::tholusi::esp8266::esp8266_t*   m_Os;
    wifi_t*                             m_Wifi;
    // no copy and no = operator
    mqtt_client_t(const mqtt_client_t&);
    mqtt_client_t &operator=(const mqtt_client_t&);    
};

}
}
}

#endif	/* COM_THOLUSI_ESP8266_MQTT_CLIENT_T_HPP */

