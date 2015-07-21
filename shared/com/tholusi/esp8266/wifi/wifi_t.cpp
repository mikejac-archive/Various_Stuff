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

#include <com/tholusi/esp8266/wifi/wifi_t.hpp>
#include <com/espressif/esp8266/include/osapi.h>
#include <com/tholusi/esp8266/prototypes/osapi.h>
#include <com/espressif/esp8266/include/espconn.h>
#include <com/espressif/esp8266/include/mem.h>
#include <com/tholusi/esp8266/errorcodes/errorcodes_t.hpp>
#include <com/tholusi/esp8266/debug/debug_t.hpp>

using namespace com::tholusi::esp8266;

//
// dammit - but can't pass own data to the scan_callback function
//
com::tholusi::esp8266::wifi_t wifi;

/******************************************************************************************************************
 * wifi_t
 *
 */

/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR wifi_t::setup()
{
    m_Status       = status_none;
    
    wifi_set_opmode(STATION_MODE);
    wifi_station_set_auto_connect(false);
    
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
void ICACHE_FLASH_ATTR wifi_t::notification(int16_t code, const char* system, const char* str, const char* p1, const char* p2)
{
    if(os()->jsonRpc()->lock() == 0) {
        DTXT("mqtt_handler_t::notification() system = %s\n", system);
        
        /******************************************************************************************************************
         * build JSON RPC notification object
         *
         */        
        if(os()->jsonRpc()->getJEncoder()->set("{ss{dssssdd}}",
                        "jsonrpc", "2.0",
                        "method",  "wifi",
                        "params",
                            "code", code,
                            "sys",  system,
                            "msg",  str,
                            "p1",   p1,
                            "p2",   p2,
                            "n1",   0,
                            "n2",   0) != 0) {
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
 * @param e
 */
void ICACHE_FLASH_ATTR wifi_t::runTask(os_event_t* e)
{
    switch(m_Command) {
        case status_connect:
            if(m_Status != status_connect_in_progress) {
                wifi_station_set_auto_connect(true);
                wifi_station_connect();

                m_Status = status_connect_in_progress;
            }
            break;
            
        case status_disconnect:
            wifi_station_disconnect();
            
            m_Command = status_none;
            m_Status  = status_disconnect_done;

            notification(EWIFI_DISCONNECTED, "disconnect", "ok");
            break;
            
        case status_scan:
            break;
            
        default:
            break;
    }
    
    m_WifiStat = wifi_station_get_connect_status();
    
    switch(m_Status) {
        case status_connect_in_progress:
            switch(m_WifiStat) {
                case STATION_GOT_IP:
                    wifi_get_ip_info(STATION_IF, &m_IpConfig);

                    if(m_IpConfig.ip.addr != 0) {
                        m_Command = status_none;
                        m_Status  = status_connect_done;
                        
                        os_sprintf(m_Buffer, "%d.%d.%d.%d", ip4_addr1(&m_IpConfig.ip), ip4_addr2(&m_IpConfig.ip), ip4_addr3(&m_IpConfig.ip), ip4_addr4(&m_IpConfig.ip));
                        notification(EWIFI_CONNECTED, "connect", "ok", m_Buffer);
                    }
                    break;
                    
                case STATION_CONNECTING:
                    break;
                    
                case STATION_WRONG_PASSWORD:
                    m_Command = status_connect;
                    m_Status  = status_connect;

                    notification(EWIFI_WRONG_PASSWORD, "connect", "wrong_password");
                    break;
                    
                case STATION_NO_AP_FOUND:
                    m_Command = status_connect;
                    m_Status  = status_connect;
                    
                    notification(EWIFI_NO_AP_FOUND, "connect", "no_ap_found");
                    break;
                    
                case STATION_CONNECT_FAIL:
                    m_Command = status_connect;
                    m_Status  = status_connect;

                    notification(EWIFI_CONNECT_FAIL, "connect", "connect_fail");
                    break;
                    
                default:
                    DTXT("wifi_t::runTask(): default 1; m_WifiStat = %d\n", m_WifiStat);
                    break;
            }
            break;
            
        case status_connect_done:
            switch(m_WifiStat) {
                case STATION_GOT_IP:
                    break;
                    
                case STATION_CONNECTING:
                    os_printf("wifi_t::runTask(): STATION_CONNECTING\n");
                    break;
                    
                case STATION_WRONG_PASSWORD:
                    m_Command = status_connect;
                    m_Status  = status_connect;

                    notification(EWIFI_WRONG_PASSWORD, "connect", "wrong_password");
                    break;
                    
                case STATION_NO_AP_FOUND:
                    m_Command = status_connect;
                    m_Status  = status_connect;
                    
                    notification(EWIFI_NO_AP_FOUND, "connect", "no_ap_found");
                    break;
                    
                case STATION_CONNECT_FAIL:
                    m_Command = status_connect;
                    m_Status  = status_connect;

                    notification(EWIFI_CONNECT_FAIL, "connect", "connect_fail");
                    break;
                    
                default:
                    os_printf("wifi_t::runTask(): default 2; m_WifiStat = %d\n", m_WifiStat);
                    break;
            }
            break;
            
        case status_scan_done:
            m_Command = m_SavedCommand;
            m_Status  = m_SavedStatus;
            break;
            
        case status_scan_in_progress:
            break;
            
        default:
            break;
    }
}
/**
 * 
 * @param ssid
 * @param key
 */
void ICACHE_FLASH_ATTR wifi_t::connect(const char* ssid, const char* key)
{
    os_memset(&m_StationConf, 0, sizeof(struct station_config));
    
    os_strcpy((char*) m_StationConf.ssid,     ssid);
    os_strcpy((char*) m_StationConf.password, key);
    
    wifi_station_set_config(&m_StationConf);
    
    m_Command = status_connect;
    m_Status  = status_connect;
}
/**
 * 
 */
void ICACHE_FLASH_ATTR wifi_t::disconnect()
{
    m_Command = status_disconnect;    
}
/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR wifi_t::scan()
{
    os_memset(&m_ScanConf, 0, sizeof(struct scan_config));
    
    wifi_station_scan(NULL, &_scan_done_callback);
    
    m_SavedStatus  = m_Status;
    m_SavedCommand = m_Command;
    
    m_Command      = status_scan;
    m_Status       = status_scan_in_progress;
    
    return EOK;
}
/**
 * 
 * @param c
 * @param status
 */
void ICACHE_FLASH_ATTR wifi_t::scan_done_callback(scaninfo* c, STATUS status)
{
    DTXT("wifi_t::scan_done_callback()\n");
    
    switch(status) {
        case OK:
            {
            struct bss_info* inf;
            char             b[20];
            
            if(!c->pbss) {
                return;
            }

            if(os()->jsonRpc()->lock() == 0) {
                DTXT("mqtt_handler_t::notification() system = %s\n", system);

                /******************************************************************************************************************
                 * build JSON RPC notification object
                 *
                 */
                os()->jsonRpc()->getJEncoder()->beginObject();
                    os()->jsonRpc()->getJEncoder()->setName("jsonrpc"); os()->jsonRpc()->getJEncoder()->setString("2.0");
                    os()->jsonRpc()->getJEncoder()->setName("method");  os()->jsonRpc()->getJEncoder()->setString("wifi");
                    os()->jsonRpc()->getJEncoder()->setName("params");
                    os()->jsonRpc()->getJEncoder()->beginObject();
                        os()->jsonRpc()->getJEncoder()->setName("code"); os()->jsonRpc()->getJEncoder()->setInt(EWIFI_LIST);
                        os()->jsonRpc()->getJEncoder()->setName("sys");  os()->jsonRpc()->getJEncoder()->setString("list");
                        os()->jsonRpc()->getJEncoder()->setName("list");
                        os()->jsonRpc()->getJEncoder()->beginArray();
                            STAILQ_FOREACH(inf, c->pbss, next) {
                                os_sprintf(b, "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(inf->bssid));

                                os()->jsonRpc()->getJEncoder()->beginObject();
                                    os()->jsonRpc()->getJEncoder()->setName("bssid");   os()->jsonRpc()->getJEncoder()->setString(b);
                                    os()->jsonRpc()->getJEncoder()->setName("ssid");    os()->jsonRpc()->getJEncoder()->setString((char*)inf->ssid);
                                    os()->jsonRpc()->getJEncoder()->setName("channel"); os()->jsonRpc()->getJEncoder()->setInt(inf->channel);
                                    os()->jsonRpc()->getJEncoder()->setName("rssi");    os()->jsonRpc()->getJEncoder()->setInt(inf->rssi);
                                    os()->jsonRpc()->getJEncoder()->setName("auth");    os()->jsonRpc()->getJEncoder()->setString(auth_to_string(b, inf->authmode));                            
                                os()->jsonRpc()->getJEncoder()->endObject();

                                inf = (struct bss_info *) &inf->next;
                            }
                        os()->jsonRpc()->getJEncoder()->endArray();
                    os()->jsonRpc()->getJEncoder()->endObject();
                os()->jsonRpc()->getJEncoder()->endObject();
                
                if(os()->jsonRpc()->getJErr()->isError()) {
                    os_printf(JSONRPC_ERR2);            
                }
                else if(os()->jsonRpc()->getJEncoder()->commit()) {            // this will activate _BufPrint_flush() which will then output the JSON data
                    os_printf(JSONRPC_ERR2);
                }

                os()->jsonRpc()->unlock();
            }
            }
            break;
            
        case FAIL:
        case PENDING:
        case BUSY:
        case CANCEL:
            DTXT("wifi_t::scan_done_callback() status = %d\n", status);
            notification(EWIFI_SCAN_FAILED, "scan", "fail");
            break;
    }
    
    m_Status = status_scan_done;
}
/**
 * 
 * @param arg
 * @param status
 */
void ICACHE_FLASH_ATTR wifi_t::_scan_done_callback(void *arg, STATUS status)
{
    wifi.scan_done_callback((scaninfo*) arg, status);
}
/**
 * 
 * @param buffer
 * @param mode
 * @return 
 */
char* ICACHE_FLASH_ATTR wifi_t::auth_to_string(char* buffer, uint8 mode)
{
    switch(mode) {
        case AUTH_OPEN:
            os_strcpy(buffer, "open");
            break;
            
        case AUTH_WEP:
            os_strcpy(buffer, "wep");
            break;
            
        case AUTH_WPA_PSK:
            os_strcpy(buffer, "wpa_psk");
            break;
            
        case AUTH_WPA2_PSK:
            os_strcpy(buffer, "wpa2_psk");
            break;
            
        case AUTH_WPA_WPA2_PSK:
            os_strcpy(buffer, "wpa_wpa2_psk");
            break;
            
        default:
            os_strcpy(buffer, "(unknown)");
            break;            
    }
    
    return buffer;
}
