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

#ifndef ESP_OPEN_RTOS_WIFI_HPP
#define	ESP_OPEN_RTOS_WIFI_HPP

#include <com/tholusi/esp-open-rtos/thread/task.hpp>
#include <com/tholusi/esp-open-rtos/timer/countdown.hpp>
#include <stdlib.h>

#include "espressif/esp_common.h"

namespace esp_open_rtos {
namespace wifi {

const int SSID_LEN = 32;
const int PSWD_LEN = 64;

/******************************************************************************************************************
 * class wifi_t
 *
 */
class wifi_t: public thread::task_t
{
    friend void _scan_done_callback(void *arg, sdk_scan_status_t status);

public:
#if defined(WITH_SMARTLINK)
    typedef enum {
        mode_static,
        mode_best_effort,
        mode_smartlink
    } mode_t;
#endif
    
    /**
     * 
     */
    wifi_t()
    {}
#if defined(WITH_SMARTLINK)
    /**
     * 
     * @param mode
     * @param ssid
     * @param password
     * @return 
     */
    int init(mode_t mode, const char* ssid = NULL, const char* password = NULL);
//#elif defined(WITH_SMARTWEB)
    /**
     * 
     * @return 
     */
    //int init();
#else
    /**
     * 
     * @param ssid
     * @param password
     * @return 
     */
    int init(const char* ssid, const char* password = NULL);
#endif
    /**
     * 
     * @param pcName
     * @return 
     */
    int task_create(const char* const pcName);
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
     * @return 
     */
    const char* mac()
    {
        return m_Mac;
    }
    /**
     * 
     * @return 
     */
    uint32_t ipaddr()
    {
        return m_IpConfig.ip.addr;

    }
    
protected:
    typedef enum {
        none = 0,
        // wifi        
        wifi_connect,
        wifi_connect_in_progress,
        wifi_connect_done,
        wifi_connect_fail,
        wifi_disconnect,
        wifi_disconnect_in_progress,
        wifi_disconnect_done,
        wifi_scan,
        wifi_scan_in_progress,
        wifi_scan_done,
#if defined(WITH_SMARTLINK)        
        wifi_smartlink,
        wifi_smartlink_scan_in_progress,
        wifi_smartlink_in_progress,
        wifi_smartlink_done,
        wifi_smartlink_fail,
#endif
#if defined(WITH_SMARTWEB)
        wifi_smartweb,
        //wifi_smartlink_scan_in_progress,
        //wifi_smartlink_in_progress,
        wifi_smartweb_done,
        wifi_smartweb_fail,
#endif
        dummy,
        // other
        ready
    } state_t;

    volatile state_t m_State;
    
#if defined(WITH_SMARTLINK)    
    volatile mode_t  m_Mode;
#endif
    
    struct ip_info   m_IpConfig;
    
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
     * @return 
     */
    state_t do_wifi_connect();
    /**
     * 
     * @return 
     */
    state_t do_wifi_connect_in_progress();
    /**
     * 
     * @return 
     */
    state_t do_wifi_connect_done();
    /**
     * 
     * @return 
     */
    state_t do_wifi_scan();
    /**
     * 
     * @return 
     */
    state_t do_wifi_scan_in_progress();
    /**
     * 
     * @return 
     */
    state_t do_wifi_check();
#if defined(WITH_SMARTLINK)
    /**
     * 
     * @return 
     */
    state_t do_wifi_smartlink();
    /**
     * 
     * @return 
     */
    state_t do_wifi_smartlink_in_progress();
    /**
     * 
     * @return 
     */
    state_t do_wifi_smartlink_done();
    /**
     * 
     * @param bss
     * @param status
     */
    void smartlink_scan_done_callback(sdk_bss_info* bss, sdk_scan_status_t status);
    /**
     * 
     * @param buf
     * @param len
     */
    IRAM void smartlink_promiscuous_rx_callback(uint8_t *buf, uint16_t len);
    /**
     * 
     * @param buf
     * @param len
     */
    IRAM void smartlink_analyze(uint8_t* buf, int len);
#endif
#if defined(WITH_SMARTWEB)
    /**
     * 
     * @return 
     */
    state_t do_wifi_smartweb();
    /**
     * 
     * @param port
     * @return 
     */
    int smartweb_run(int port);
    /**
     * 
     * @param buffer
     * @param len
     * @return 
     */
    int smartweb_token_begin(char* buffer, int len);
    /**
     * 
     * @param token
     * @param len
     * @return 
     */
    int smartweb_token_next(char** token, int& len);

#endif
    /**
     * 
     * @param arg
     * @param status
     */
    void scan_done_callback(sdk_scaninfo_t* c, sdk_scan_status_t status);
    /**
     * 
     * @param buffer
     * @param mode
     * @return 
     */
    char* auth_to_string(char* buffer, uint8_t mode);
    /**
     * 
     * @param arg
     * @param status
     */
    static void _scan_done_callback(void* arg, sdk_scan_status_t status);
#if defined(WITH_SMARTLINK)
    /**
     * 
     * @param arg
     * @param status
     */
    static void _smartlink_scan_done_callback(void* arg, sdk_scan_status_t status);
    /**
     * 
     * @param buf
     * @param len
     */
    IRAM static void _smartlink_promiscuous_rx_callback(uint8_t *buf, uint16_t len);
#endif

    struct sdk_station_config       m_Config;
    
    char                            m_Mac[20];
    int                             m_WifiErrors;
    
#if defined(WITH_SMARTLINK)
    typedef enum _encrytion_mode {
        ENCRY_NONE           = 1,
        ENCRY_WEP,
        ENCRY_TKIP,
        ENCRY_CCMP
    } ENCYTPTION_MODE;

    struct router_info {
        SLIST_ENTRY(router_info)    next;

        uint8_t                     bssid[6];
        uint8_t                     channel;
        uint8_t                     authmode;

        uint16_t                    rx_seq;
        uint8_t                     encrytion_mode;
        uint8_t                     iv[8];
        uint8_t                     iv_check;
    };
    
    uint16_t                                                m_CurrentTag;
    uint8_t                                                 m_CurrentMarker;
    uint8_t                                                 m_SsidLen;
    uint8_t                                                 m_PswdLen;
    
    uint8_t                                                 m_SsidCnt;
    uint8_t                                                 m_PswdCnt;
        
    uint8_t                                                 m_SsidBuf[SSID_LEN];
    uint8_t                                                 m_PswdBuf[PSWD_LEN];

    bool                                                    m_GotMarker;
    uint8_t                                                 m_NibblesSsid[2];
    uint8_t                                                 m_NibblesPswd[2];
    

    volatile SLIST_HEAD(router_info_head, router_info)      m_RouterList;
    volatile uint8_t                                        m_CurrentChannel;
    volatile uint16_t                                       m_ChannelBits;

    timer::countdown_t                                      m_SmartCountdown;
#endif
    
#if defined(WITH_SMARTWEB)
    char* m_P;
    char* m_Pp;
    int   m_I;
    int   m_N;
#endif
    
    // no copy and no = operator
    wifi_t(const wifi_t&);
    wifi_t &operator=(const wifi_t&);    
};

} // namespace wifi {
} // namespace esp_open_rtos {

extern esp_open_rtos::wifi::wifi_t wifi_global;

#endif	/* ESP_OPEN_RTOS_WIFI_HPP */

