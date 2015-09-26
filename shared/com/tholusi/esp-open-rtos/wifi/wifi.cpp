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
    #include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
    //#include <com/tholusi/esp-open-rtos/ip/mdns.hpp>
    #include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#else
    #include <wifi/wifi.hpp>
    #include <cplusplus/cplusplus.hpp>
#endif

#include <string.h>

using namespace esp_open_rtos::wifi;

#define DTXT(...)       printf(__VA_ARGS__)

//
// dammit - but can't pass own data to the scan_callback function
//
esp_open_rtos::wifi::wifi_t wifi_global;

/******************************************************************************************************************
 * class wifi_t
 *
 */

#if defined(WITH_SMARTLINK)
/**
 * 
 * @param mode
 * @param ssid
 * @param password
 * @return 
 */
int wifi_t::init(wifi_t::mode_t mode, const char* ssid, const char* password)
{
    m_Mode          = mode;
    m_State         = none;
    m_WifiErrors    = 0;
    
    memset(&m_IpConfig, 0, sizeof(struct ip_info));
    
    uint8_t hwaddr[6];
    
    // get our MAC address and convert it to text for future use
    sdk_wifi_get_macaddr(STATION_IF, hwaddr);
    sprintf(m_Mac, MACSTR, MAC2STR(hwaddr));
    
    if(ssid != NULL) {
        strcpy((char*)(m_Config.ssid), ssid);
        
        if(password != NULL) {
            strcpy((char*)(m_Config.password), password);
        }
        else {
            m_Config.password[0] = '\0';
        }
    }
    else {
        m_Config.ssid[0]     = '\0';
        m_Config.password[0] = '\0';
    }
    
    return 0;    
}
#elif defined(WITH_SMARTWEB)
/**
 * 
 * @return 
 */
int wifi_t::init()
{
    //m_State         = wifi_smartweb;
    m_WifiErrors    = 0;
    
    memset(&m_IpConfig, 0, sizeof(struct ip_info));
    
    uint8_t hwaddr[6];
    
    // get our MAC address and convert it to text for future use
    sdk_wifi_get_macaddr(STATION_IF, hwaddr);
    sprintf(m_Mac, MACSTR, MAC2STR(hwaddr));
    
    sdk_wifi_set_opmode(SOFTAP_MODE);
    
    struct ip_info ap_ip;
    IP4_ADDR(&ap_ip.ip,      172, 16,  0, 1);
    IP4_ADDR(&ap_ip.gw,      0,   0,   0, 0);
    IP4_ADDR(&ap_ip.netmask, 255, 255, 0, 0);
    sdk_wifi_set_ip_info(1, &ap_ip);

    struct sdk_softap_config ap_config;
    
    memset(ap_config.ssid,     0, sizeof(ap_config.ssid));
    memset(ap_config.password, 0, sizeof(ap_config.password));

    strcpy((char*)ap_config.ssid, WIFI_AP_NAME);

    ap_config.ssid_len        = strlen(WIFI_AP_NAME);
    ap_config.authmode        = AUTH_OPEN;
    ap_config.ssid_hidden     = 0;
    ap_config.max_connection  = 3;
    ap_config.channel         = 7;
    ap_config.beacon_interval = 100;

    if(!sdk_wifi_softap_set_config(&ap_config)) {
        DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() failed\n");
        m_State = wifi_smartweb_fail;
    }
    else {
        DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() success\n");

        //ip_addr_t first_client_ip;
        //IP4_ADDR(&first_client_ip, 172, 16, 0, 2);
        //dhcpserver_start(&first_client_ip, 4);

        m_State = wifi_smartweb;
    }
    
    return 0;
}
#else
/**
 * 
 * @param ssid
 * @param password
 * @return 
 */
int wifi_t::init(const char* ssid, const char* password)
{
    m_State         = none;
    m_WifiErrors    = 0;
    
    memset(&m_IpConfig, 0, sizeof(struct ip_info));
    
    uint8_t hwaddr[6];
    
    // get our MAC address and convert it to text for future use
    sdk_wifi_get_macaddr(STATION_IF, hwaddr);
    sprintf(m_Mac, MACSTR, MAC2STR(hwaddr));
    
    if(ssid != NULL) {
        strcpy((char*)(m_Config.ssid), ssid);
        
        if(password != NULL) {
            strcpy((char*)(m_Config.password), password);
        }
        else {
            m_Config.password[0] = '\0';
        }
    }
    else {
        m_Config.ssid[0]     = '\0';
        m_Config.password[0] = '\0';
        
        return -1;
    }
    
    return 0;    
}
#endif
/**
 * 
 * @param pcName
 * @return 
 */
int wifi_t::task_create(const char * const pcName)
{
    unsigned short usStackDepth = 512; //256;
    
    return thread::task_t::task_create(pcName, usStackDepth, 2);
}
/**
 * 
 */
void wifi_t::task()
{
    DTXT("wifi_t::task(): start; MAC = %s\n", m_Mac);
    
#if defined(WITH_SMARTWEB)
    //bool smartweb_
#endif
    
    while(true) {
        switch(m_State) {
            case none:
                m_State = do_wifi_connect();
                break;
                
            case wifi_connect:
                m_State = do_wifi_connect();
                break;
                
            case wifi_connect_in_progress:
                sleep(1000);
                m_State = do_wifi_connect_in_progress();
                break;
                
            case wifi_connect_fail:
                sleep(1000);
                m_State = do_wifi_connect();                                    // start again
                break;
                
            case wifi_connect_done:
                m_State = do_wifi_connect_done();
                on_connect();                                                   // notify user
                break;
                
#if defined(WITH_SMARTLINK)
            case wifi_smartlink:
                m_State = do_wifi_smartlink();
                break;
                
            case wifi_smartlink_scan_in_progress:
                break;
                
            case wifi_smartlink_in_progress:
                m_State = do_wifi_smartlink_in_progress();
                break;
                
            case wifi_smartlink_done:
                //sleep(10000);
                m_State = do_wifi_smartlink_done();
                //sleep(10000);
                break;
                
            case wifi_smartlink_fail:
                m_State = do_wifi_smartlink();
                break;
#endif
                
#if defined(WITH_SMARTWEB)
            case wifi_smartweb:
                m_State = do_wifi_smartweb();
                break;

            case wifi_smartweb_run:
                //m_State = ready;
                m_State = smartweb_start_server(80);
                break;
                
            case wifi_smartweb_in_progress:
                m_State = smartweb_run_server();
                break;
                
            case wifi_smartweb_done:
                break;
                
            case wifi_smartweb_fail:
                DTXT("wifi_t::task(): wifi_smartweb_fail\n");
                m_State = ready;
                break;
#endif
                
            case wifi_scan:
                m_State = do_wifi_scan();
                DTXT("wifi_t::task(): do_wifi_scan\n");
                break;
                
            case wifi_scan_in_progress:
                //DTXT("wifi_t::task(): do_wifi_scan_in_progress\n");
                //sleep(500);
                //m_State = do_wifi_scan_in_progress();
                break;
                
            case wifi_scan_done:
                DTXT("wifi_t::task(): do_wifi_scan_done\n");
                m_State = do_wifi_check();
                break;

            case ready:
                DTXT("wifi_t::task(): ready\n");
#if defined(WITH_SMARTWEB)
                //m_State = smartweb_start_server(80);
#else
                sleep(60000);
                m_State = do_wifi_check();
#endif
                break;
                
            case dummy:
                break;
                
            default:
                DTXT("wifi_t::task(): default\n");
                break;
        }
    }
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_connect()
{
    DTXT("wifi_t::do_wifi_connect(): begin\n");
    
    state_t state = m_State;
    
#if defined(WITH_SMARTLINK)
    switch(m_Mode) {
        case mode_static:
            DTXT("wifi_t::do_wifi_connect(): (mode_static) ssid = %s\n", m_Config.ssid);

            // required to call wifi_set_opmode before station_set_config
            sdk_wifi_set_opmode(STATION_MODE);

            sdk_wifi_station_set_config(&m_Config);
            sdk_wifi_station_connect();

            state = wifi_connect_in_progress;
            break;

        case mode_best_effort:
            break;

        case mode_smartlink:
            if(m_Config.ssid[0] != '\0') {
                DTXT("wifi_t::do_wifi_connect(): (mode_smartlink) have SSID; ssid = %s\n", m_Config.ssid);

                // required to call wifi_set_opmode before station_set_config
                sdk_wifi_set_opmode(STATION_MODE);

                sdk_wifi_station_set_config(&m_Config);
                sdk_wifi_station_connect();

                state = wifi_connect_in_progress;
            }
            else {
                DTXT("wifi_t::do_wifi_connect(): (mode_smartlink) start smartlink\n");
                
                // start smartlink procedure
                state = wifi_smartlink;
            }
            break;

    }
#else
    if(m_Config.ssid[0] != '\0') {
        DTXT("wifi_t::do_wifi_connect(): have SSID; ssid = %s, password = %s\n", m_Config.ssid, m_Config.password);
        
        // required to call wifi_set_opmode before station_set_config
        sdk_wifi_set_opmode(STATION_MODE);
        
        sdk_wifi_station_set_config(&m_Config);
        sdk_wifi_station_connect();

        state = wifi_connect_in_progress;
    }
    else {
        DTXT("wifi_t::do_wifi_connect(): no SSID!\n");
    }
#endif
    
    m_WifiErrors = 0;
    
    DTXT("wifi_t::do_wifi_connect(): end\n");
    
    return state;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_connect_done()
{
    DTXT("wifi_t::do_wifi_connect_done(): begin\n");
    
    sdk_wifi_get_ip_info(STATION_IF, &m_IpConfig);
    
    DTXT("wifi_t::do_wifi_connect_done(): ip = %d.%d.%d.%d\n", ip4_addr1(&m_IpConfig.ip), ip4_addr2(&m_IpConfig.ip), ip4_addr3(&m_IpConfig.ip), ip4_addr4(&m_IpConfig.ip));

    DTXT("wifi_t::do_wifi_connect_done(): end\n");
    
    return ready;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_connect_in_progress()
{
    DTXT("wifi_t::do_wifi_connect_in_progress(): begin\n");
    
    state_t state;
    uint8_t wifi_status = sdk_wifi_station_get_connect_status();
    
    switch(wifi_status) {
        case STATION_IDLE:
            DTXT("wifi_t::do_wifi_connect_in_progress(): STATION_IDLE, m_WifiErrors = %d\n", m_WifiErrors);
            state = wifi_connect_in_progress;       // not really :-(
            ++m_WifiErrors;
            break;
            
        case STATION_GOT_IP:
            DTXT("wifi_t::do_wifi_connect_in_progress(): wifi connected\n");
            state        = wifi_connect_done;
            m_WifiErrors = 0;
            break;
            
        case STATION_WRONG_PASSWORD:
            DTXT("wifi_t::do_wifi_connect_in_progress(): STATION_WRONG_PASSWORD, m_WifiErrors = %d\n", m_WifiErrors);
            state = wifi_connect_in_progress;
            ++m_WifiErrors;
            break;

        case STATION_NO_AP_FOUND:
            DTXT("wifi_t::do_wifi_connect_in_progress(): STATION_NO_AP_FOUND, m_WifiErrors = %d\n", m_WifiErrors);
            state = wifi_connect_in_progress;
            ++m_WifiErrors;
            break;

        case STATION_CONNECT_FAIL:
            DTXT("wifi_t::do_wifi_connect_in_progress(): STATION_CONNECT_FAIL, m_WifiErrors = %d\n", m_WifiErrors);
            state = wifi_scan;
            //state = wifi_connect_in_progress;
            ++m_WifiErrors;
            break;
        
        default:
            DTXT("wifi_t::do_wifi_connect_in_progress(): wifi not connected; wifi_status = %d, m_WifiErrors = %d\n", wifi_status, m_WifiErrors);
            state = wifi_connect_in_progress;
            ++m_WifiErrors;
            break;
    }
    
    if(m_WifiErrors >= 60) {
        DTXT("wifi_t::do_wifi_connect_in_progress(): excessive error count; ssid = %s, password = %s\n", m_Config.ssid, m_Config.password);
        
#if defined(WITH_SMARTLINK)        
        if(m_Mode == mode_smartlink) {
            //state = wifi_smartlink;
            sdk_system_restart();
        }
        else {
            //sdk_system_restart();
            state = wifi_scan;
        }
#else
        state = wifi_scan;

#endif        
    }
    
    DTXT("wifi_t::do_wifi_connect_in_progress(): end\n");
    
    return state;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_scan()
{
    DTXT("wifi_t::do_wifi_scan(): begin\n");
    
    struct sdk_station_config config;
    
    bool ret = sdk_wifi_station_get_config(&config);
    
    if(ret) {
        DTXT("wifi_t::do_wifi_scan(): true; ssid = %s, password = %s\n", config.ssid, config.password);
    }
    else {
        DTXT("wifi_t::do_wifi_scan(): false; ssid = %s, password = %s\n", config.ssid, config.password);
    }
        
    sdk_wifi_station_scan(NULL, &_scan_done_callback);
    
    DTXT("wifi_t::do_wifi_scan(): end\n");
    
    return wifi_scan_in_progress;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_scan_in_progress()
{
    return wifi_scan_in_progress;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_check()
{
    DTXT("wifi_t::do_wifi_check(): begin\n");
    
    state_t state;
    uint8_t wifi_status = sdk_wifi_station_get_connect_status();
    
    if(wifi_status == STATION_GOT_IP) {
        DTXT("wifi_t::do_wifi_check(): wifi connected\n");
        state = ready;
    }
    else {
        DTXT("wifi_t::do_wifi_check(): wifi not connected; wifi_status = %d\n", wifi_status);
        state = none;
    }

    DTXT("wifi_t::do_wifi_check(): end\n");

    return state;
}
/**
 * 
 * @param c
 * @param status
 */
void wifi_t::scan_done_callback(sdk_scaninfo_t* c, sdk_scan_status_t status)
{
    DTXT("wifi_t::scan_done_callback(): begin\n");
    
    struct sdk_bss_info* inf;
    char                 b[20];
    
    switch(status) {
        case SCAN_OK:
            DTXT("wifi_t::scan_done_callback(): SCAN_OK\n");
            
            if(!c->pbss) {
                DTXT("wifi_t::scan_done_callback(): !c->pbss\n");
                break;
            }
            
            STAILQ_FOREACH(inf, c->pbss, next) {
                //sprintf(b, "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(inf->bssid));

                DTXT("%-20s  %3d  %4d  %5s\n", (char*)inf->ssid, inf->channel, inf->rssi, auth_to_string(b, inf->authmode));
                
                inf = (struct sdk_bss_info *) &inf->next;
            }
            break;
            
        case SCAN_FAIL:
            DTXT("wifi_t::scan_done_callback(): SCAN_FAIL\n");
            break;
            
        case SCAN_PENDING:
            DTXT("wifi_t::scan_done_callback(): SCAN_PENDING\n");
            break;
            
        case SCAN_BUSY:
            DTXT("wifi_t::scan_done_callback(): SCAN_BUSY\n");
            break;
            
        case SCAN_CANCEL:
            DTXT("wifi_t::scan_done_callback(): SCAN_CANCEL\n");
            break;
    }
    
    m_State = wifi_scan_done;
    
    DTXT("wifi_t::scan_done_callback(): end\n");
}
/**
 * 
 * @param arg
 * @param status
 */
void wifi_t::_scan_done_callback(void* arg, sdk_scan_status_t status)
{
    wifi_global.scan_done_callback((sdk_scaninfo_t*) arg, status);
}
/**
 * 
 * @param buffer
 * @param mode
 * @return 
 */
char* wifi_t::auth_to_string(char* buffer, uint8_t mode)
{
    switch(mode) {
        case AUTH_OPEN:
            strcpy(buffer, "open");
            break;
            
        case AUTH_WEP:
            strcpy(buffer, "wep");
            break;
            
        case AUTH_WPA_PSK:
            strcpy(buffer, "wpa_psk");
            break;
            
        case AUTH_WPA2_PSK:
            strcpy(buffer, "wpa2_psk");
            break;
            
        case AUTH_WPA_WPA2_PSK:
            strcpy(buffer, "wpa_wpa2_psk");
            break;
            
        default:
            strcpy(buffer, "(unknown)");
            break;            
    }
    
    return buffer;
}
