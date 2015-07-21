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

#ifndef COM_THOLUSI_ESP8266_WIFI_T_HPP
#define	COM_THOLUSI_ESP8266_WIFI_T_HPP

#include <com/tholusi/esp8266/task/task_t.hpp>
//#include <com/espressif/esp8266/include/ip_addr.h>
//#include <com/espressif/esp8266/include/user_interface.h>

namespace com {
namespace tholusi {
namespace esp8266 {

/******************************************************************************************************************
 * wifi_t
 *
 */
class wifi_t: public com::tholusi::esp8266::task0_t
{
    friend void ICACHE_FLASH_ATTR _scan_done_callback(void *arg, STATUS status);
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR wifi_t()
    {
    }
    /**
     * 
     */
    ICACHE_FLASH_ATTR ~wifi_t()
    {
    }
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR setup();
    /**
     * 
     * @param ssid
     * @param key
     */
    void ICACHE_FLASH_ATTR connect(const char* ssid, const char* key);
    /**
     * 
     */
    void ICACHE_FLASH_ATTR disconnect();
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR scan();
    /**
     * 
     * @return 
     */
    bool ICACHE_FLASH_ATTR isConnected()
    {
        return (m_Status == status_connect_done) || (m_SavedStatus == status_connect_done) ? true : false;
    }
    
protected:
    typedef enum {
        status_none = 0,
        status_connect,
        status_connect_in_progress,
        status_connect_done,
        status_disconnect,
        status_disconnect_in_progress,
        status_disconnect_done,
        status_scan,
        status_scan_in_progress,
        status_scan_done
    } status_t;

    /**
     * 
     * @param code
     * @param system
     * @param str
     * @param p1
     * @param p2
     */
    void ICACHE_FLASH_ATTR notification(int16_t code, const char* system, const char* str, const char* p1 = 0, const char* p2 = 0);

    status_t                m_Status;
    status_t                m_SavedStatus;
    status_t                m_Command;
    status_t                m_SavedCommand;
    
    struct ip_info          m_IpConfig;
    struct station_config   m_StationConf;
    struct scan_config      m_ScanConf;
    
    char                    m_Buffer[20];
    
private:
    /**
     * 
     * @param e
     */
    void ICACHE_FLASH_ATTR runTask(os_event_t* e);
    /**
     * 
     * @param c
     * @param status
     */
    void ICACHE_FLASH_ATTR scan_done_callback(scaninfo* c, STATUS status);
    /**
     * 
     * @param arg
     * @param status
     */
    static void ICACHE_FLASH_ATTR _scan_done_callback(void *arg, STATUS status);
    /**
     * 
     * @param buffer
     * @param mode
     * @return 
     */
    char* ICACHE_FLASH_ATTR auth_to_string(char* buffer, uint8 mode);
    
    uint8_t     m_WifiStat;
    
    // no copy and no = operator
    wifi_t(const wifi_t&);
    wifi_t &operator=(const wifi_t&);    
};

}
}
}

#endif	/* COM_THOLUSI_ESP8266_WIFI_T_HPP */

