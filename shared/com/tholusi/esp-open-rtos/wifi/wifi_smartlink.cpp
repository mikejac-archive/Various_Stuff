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

#if defined(WITH_SMARTLINK)

#include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#include <string.h>

using namespace esp_open_rtos::wifi;

#define DTXT(...)       printf(__VA_ARGS__)


const uint16_t TAG_NONE = 0;
const uint16_t TAG_SSID = 1099;
const uint16_t TAG_PSWD = 1199;

const uint8_t  MARKER_1 = 3;
const uint8_t  MARKER_2 = 23;

const uint16_t OFFSET_L = 28;
const uint16_t OFFSET_C = 593;

const uint8_t  SIG_BEGIN = 0xFF;
const uint8_t  SIG_DONE  = 0xFE;

const int      CHANNLE_CHANGE_TIME = 5;

struct RxControl {
    signed              rssi:8;             // signal intensity of packet
    unsigned            rate:4;
    unsigned            is_group:1;
    unsigned:1;
    unsigned            sig_mode:2;         // 0:is 11n packet; 1:is not 11n packet;
    unsigned            legacy_length:12;   // if not 11n packet, shows length of packet.
    unsigned            damatch0:1;
    unsigned            damatch1:1;
    unsigned            bssidmatch0:1;
    unsigned            bssidmatch1:1;
    unsigned            MCS:7;              // if is 11n packet, shows the modulation and code used (range from 0 to 76)
    unsigned            CWB:1;              // if is 11n packet, shows if is HT40 packet or not
    unsigned            HT_length:16;       // if is 11n packet, shows length of packet.
    unsigned            Smoothing:1;
    unsigned            Not_Sounding:1;
    unsigned:1;
    unsigned            Aggregation:1;
    unsigned            STBC:2;
    unsigned            FEC_CODING:1;       // if is 11n packet, shows if is LDPC packet or not.
    unsigned            SGI:1;
    unsigned            rxend_state:8;
    unsigned            ampdu_cnt:8;
    unsigned            channel:4;          // which channel this packet in.
    unsigned:12;
};

struct LenSeq {
    uint16_t            len;                // length of packet
    uint16_t            seq;                // serial number of packet, the high 12bits are serial number, low 14 bits are Fragment number (usually be 0)
    uint8_t             addr3[6];           // the third address in packet
};

struct sniffer_buf {
    struct RxControl    rx_ctrl;
    uint8_t             buf[36];            // head of ieee802.11 packet
    uint16_t            cnt;                // number count of packet
    struct LenSeq       lenseq[1];          // length of packet
};

/*struct sniffer_buf2 {
    struct RxControl    rx_ctrl;
    uint8_t             buf[112];
    uint16_t            cnt;
    uint16_t            len;                // length of packet
};*/ 


void loghexdump(const void *pAddressIn, long lSize)
{
    char szBuf[100];
    long lIndent = 1;
    long lOutLen, lIndex, lIndex2, lOutLen2;
    long lRelPos;
    struct { char *pData; unsigned long lSize; } buf;
    unsigned char *pTmp,ucTmp;
    unsigned char *pAddress = (unsigned char *)pAddressIn;

    buf.pData   = (char *)pAddress;
    buf.lSize   = lSize;

    while (buf.lSize > 0) {
        pTmp     = (unsigned char *)buf.pData;
        lOutLen  = (int)buf.lSize;

        if (lOutLen > 16)
            lOutLen = 16;

        // create a 64-character formatted output line:
        sprintf(szBuf,  " >                            "
                        "                      "
                        "    %08X", pTmp-pAddress);

        lOutLen2 = lOutLen;

        for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
            lOutLen2;
            lOutLen2--, lIndex += 2, lIndex2++
        )

        {
            ucTmp = *pTmp++;

            sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
            
            if(ucTmp < 0x20 || ucTmp > 0x7E) {
                ucTmp = '.'; // nonprintable char
            }
            
            szBuf[lIndex2] = ucTmp;

            if (!(++lRelPos & 3))     // extra blank after 4 bytes
                {  lIndex++; szBuf[lIndex+2] = ' '; }
        }

        if (!(lRelPos & 3)) lIndex--;

        szBuf[lIndex  ]   = '<';
        szBuf[lIndex+1]   = ' ';

        printf("%s\n", szBuf);

        buf.pData   += lOutLen;
        buf.lSize   -= lOutLen;
    }
}
/******************************************************************************************************************
 * class wifi_t
 *
 */

/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_smartlink()
{
    state_t state;

    DTXT("wifi_t::do_wifi_smartlink(): begin\n");
    
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_auto_connect(0);

    m_Config.ssid[0]     = '\0';
    m_Config.password[0] = '\0';
    
    sdk_wifi_station_set_config(&m_Config);

    m_CurrentTag = TAG_NONE;
    m_SsidCnt    = SIG_BEGIN;
    m_PswdCnt    = SIG_BEGIN;

    // init the router info 
    SLIST_INIT(&m_RouterList);
  
    // scan wifi 
    sdk_wifi_station_scan(NULL, _smartlink_scan_done_callback);

    state = wifi_smartlink_scan_in_progress;
    
    DTXT("wifi_t::do_wifi_smartlink(): end\n");
    
    return state;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_smartlink_done()
{
    DTXT("wifi_t::do_wifi_smartlink_done(): begin\n");
    
    memset(m_Config.ssid,     0, SSID_LEN);
    memset(m_Config.password, 0, PSWD_LEN);
    
    memcpy(m_Config.ssid,     m_SsidBuf, m_SsidLen);
    memcpy(m_Config.password, m_PswdBuf, m_PswdLen);
    
    DTXT("wifi_t::do_wifi_smartlink_done(): end\n");
    
    return wifi_connect;
}
/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_smartlink_in_progress()
{
    state_t state = m_State;
    
    if(m_SsidCnt == SIG_DONE && m_PswdCnt == SIG_DONE) {
        sdk_wifi_promiscuous_enable(0);
        
        state = wifi_smartlink_done;
        
        DTXT("wifi_t::do_wifi_smartlink_in_progress(): done!\n");
    }
    else if(m_SmartCountdown.expired()) {
        //DTXT("wifi_t::do_wifi_smartlink_in_progress(): timer expired!\n");
        
        uint8_t i;
        
        if(m_CurrentTag == TAG_NONE) {
            // nothing seems to be happening, change channel
            for(i = m_CurrentChannel; i < 14; i++) {
                if((m_ChannelBits & (1 << i)) != 0) {            // is this channel active
                    m_CurrentChannel = i + 1;
                    
                    sdk_wifi_promiscuous_enable(0);
                    sdk_wifi_set_channel(m_CurrentChannel - 1);
                    sdk_wifi_set_promiscuous_rx_cb(_smartlink_promiscuous_rx_callback);
                    sdk_wifi_promiscuous_enable(1);

                    DTXT("wifi_t::do_wifi_smartlink_in_progress(): channel changed (a) %d\n", m_CurrentChannel - 1);

                    m_SmartCountdown.countdown(CHANNLE_CHANGE_TIME);
                    break;
                }
            }
            
            if(i == 14) {
                // we reached the end of channels, start from the beginning
                m_CurrentChannel = 1;

                for(i = m_CurrentChannel; i < 14; i++) {
                    if((m_ChannelBits & (1 << i)) != 0) {        // is this channel active
                        m_CurrentChannel = i + 1;
                        
                        sdk_wifi_promiscuous_enable(0);
                        sdk_wifi_set_channel(m_CurrentChannel - 1);
                        sdk_wifi_set_promiscuous_rx_cb(_smartlink_promiscuous_rx_callback);
                        sdk_wifi_promiscuous_enable(1);

                        DTXT("wifi_t::do_wifi_smartlink_in_progress(): channel changed (b) %d\n", m_CurrentChannel - 1);

                        m_SmartCountdown.countdown(CHANNLE_CHANGE_TIME);
                        break;
                    }
                }
                
                if(i == 14) {
                    DTXT("wifi_t::do_wifi_smartlink_in_progress(): no channels!\n");
                }
            }
        }
    }
    
    return state;
}
/**
 * 
 * @param c
 * @param status
 */
IRAM void wifi_t::smartlink_scan_done_callback(sdk_bss_info* bss, sdk_scan_status_t status)
{
    DTXT("wifi_t::smartlink_scan_done_callback(): begin\n");
    
    uint8_t ssid[SSID_LEN + 1];

    m_ChannelBits    = 0;
    m_CurrentChannel = 1;
    
    struct router_info *info = NULL;
    state_t state;

    // clear the router list
    while((info = SLIST_FIRST(&m_RouterList)) != NULL){
        SLIST_REMOVE_HEAD(&m_RouterList, next);
        free(info);
    }
    
    switch(status) {
        case SCAN_OK:
            DTXT("wifi_t::smartlink_scan_done_callback(): SCAN_OK\n");
            
            uint8_t i;

            while(bss != NULL) {
                memset(ssid, 0, SSID_LEN + 1);

                // get the bss info
                if(strlen((const char*)(bss->ssid)) <= SSID_LEN) {
                    memcpy(ssid, bss->ssid, strlen((const char*)(bss->ssid)));
                } 
                else {
                    memcpy(ssid, bss->ssid, SSID_LEN);
                }

                if(bss->channel != 0) {
                    struct router_info *info = NULL;

                    // indicate that this channel is active
                    m_ChannelBits |= 1 << (bss->channel);

                    info = (struct router_info *)malloc(sizeof(struct router_info));
                    memset(info, 0, sizeof(struct router_info));
                    
                    info->authmode = bss->authmode;
                    info->channel  = bss->channel;
                    
                    memcpy(info->bssid, bss->bssid, 6);
                    DTXT("wifi_t::smartlink_scan_done_callback(): " MACSTR " ssid = %s, channel = %d, authmode = %d, rssi = %d\n", MAC2STR(info->bssid), ssid, bss->channel, bss->authmode, bss->rssi);

                    SLIST_INSERT_HEAD(&m_RouterList, info, next);
                }
                else {
                    DTXT("wifi_t::smartlink_scan_done_callback(): bss->channel == 0\n");
                }
                
                bss = STAILQ_NEXT(bss, next);
            }

            state = wifi_smartlink_fail;            // just in case ...
            
            for(i = m_CurrentChannel; i < 14; i++) {
                if((m_ChannelBits & (1 << i)) != 0) {
                    m_CurrentChannel = i + 1;
                    
                    sdk_wifi_set_channel(m_CurrentChannel - 1);
                    
                    // start sniffer
                    sdk_wifi_set_promiscuous_rx_cb(_smartlink_promiscuous_rx_callback);
                    sdk_wifi_promiscuous_enable(1);
                    
                    state = wifi_smartlink_in_progress;
                    
                    DTXT("wifi_t::smartlink_scan_done_callback(): current channel = %d\n", m_CurrentChannel - 1);
                    
                    m_SmartCountdown.countdown(CHANNLE_CHANGE_TIME);
                    break;
                }
            }

            m_State = state;
            break;
            
        case SCAN_FAIL:
            DTXT("wifi_t::smartlink_scan_done_callback(): SCAN_FAIL\n");
            m_State = wifi_smartlink_fail;
            break;
            
        case SCAN_PENDING:
            DTXT("wifi_t::smartlink_scan_done_callback(): SCAN_PENDING\n");
            m_State = wifi_smartlink_fail;
            break;
            
        case SCAN_BUSY:
            DTXT("wifi_t::smartlink_scan_done_callback(): SCAN_BUSY\n");
            m_State = wifi_smartlink_fail;
            break;
            
        case SCAN_CANCEL:
            DTXT("wifi_t::smartlink_scan_done_callback(): SCAN_CANCEL\n");
            m_State = wifi_smartlink_fail;
            break;
    }
    
    DTXT("wifi_t::smartlink_scan_done_callback(): end\n");
}
/**
 * http://nonnoise.github.io/ESP-WROOM-02/_downloads/2C-ESP8266__SDK__Programming%20Guide__EN_v1.1.1.pdf
 * 
 * @param buf
 * @param len
 */
IRAM void wifi_t::smartlink_promiscuous_rx_callback(uint8_t* buf, uint16_t len)
{
    if(len == 12 || len == 128) {
        // not interested
        return;
    }
    
    struct router_info* info    = NULL;
    struct sniffer_buf* sniffer = NULL;

    sniffer = (struct sniffer_buf*)buf;
    buf    += sizeof(struct RxControl);
    
    len     = sniffer->lenseq[0].len;                       // length of (encrypted) payload
    
    // check the bssid and channel of the packet
    SLIST_FOREACH(info, &m_RouterList, next) {
        if(memcmp(info->bssid, buf + 4, 6) == 0) {          // check for AP's MAC address
            if(m_CurrentChannel - 1 != info->channel) {      // check channel
                //DTXT("wifi_t::smartlink_promiscuous_rx_callback(): not our channel; end\n");
                return;
            } 
            else {
                //DTXT("wifi_t::smartlink_promiscuous_rx_callback(): our channel\n");
                break;                                      // found it
            }
        }
    }

    if(info == NULL) {
        //DTXT("wifi_t::smartlink_promiscuous_rx_callback(): info == NULL\n");
        // packet didn't come from an AP we know about
        return;
    }

    // 0x08 and 0x88 is wifi Data frame
    if(buf[0] == 0x08 || buf[0] == 0x88) {
        if(info->rx_seq != (*(uint16_t *)(buf + 22)) >> 4) {    // check that this is not a duplicate packet
            info->rx_seq = (*(uint16_t *)(buf + 22)) >> 4;      // save for next time around

            if(info->encrytion_mode != 0) {
                //
                // adjust payload lenght depending on encryption used
                //
                if((buf[0] & 0x80) == 0) {
                    len -= 24;
                } 
                else {
                    len -= 26;
                }
                
                if(info->encrytion_mode == ENCRY_NONE) {
                    len -= 40;
                } 
                else if (info->encrytion_mode == ENCRY_WEP){
                    len = len - 40 - 4 - 4;
                } 
                else if (info->encrytion_mode == ENCRY_TKIP) {
                    len = len - 40 - 12 - 8;
                } 
                else if (info->encrytion_mode == ENCRY_CCMP) {
                    len = len - 40 - 8 - 8;
                }
                
                //
                // now analyze the packet length
                //
                smartlink_analyze(buf + 10, len);
            } 
            else {
                if(info->authmode == AUTH_OPEN) {
                    info->encrytion_mode = ENCRY_NONE;
                    //DTXT(MACSTR ", channel %d, mode %d\n", MAC2STR(info->bssid), info->channel, info->encrytion_mode);
                } 
                else if(info->authmode == AUTH_WEP) {
                    info->encrytion_mode = ENCRY_WEP;
                    //DTXT(MACSTR ", channel %d, mode %d\n", MAC2STR(info->bssid), info->channel, info->encrytion_mode);
                } 
                else {
                    // set the bssid encrytion_mode by the packet
                    if(info->iv_check == 0) {
                        if(buf[0] == 0x08) {
                            memcpy(info->iv, buf + 24, 8);
                        } 
                        else if (buf[0] == 0x88) {
                            memcpy(info->iv, buf + 26, 8);
                        }
                        
                        info->iv_check = 1;
                    } 
                    else {
                        uint8_t *local_iv = NULL;
                        
                        if(buf[0] == 0x08) {
                            local_iv = buf + 24;
                        } 
                        else if(buf[0] == 0x88) {
                            local_iv = buf + 26;
                        }
                        
                        if(info->iv[2] == local_iv[2] && local_iv[2] == 0) {
                            info->encrytion_mode = ENCRY_CCMP;
                            //DTXT(MACSTR ", channel %d, mode %d\n", MAC2STR(info->bssid), info->channel, info->encrytion_mode);
                        } 
                        else {
                            info->encrytion_mode = ENCRY_TKIP;
                            //DTXT(MACSTR ", channel %d, mode %d\n", MAC2STR(info->bssid), info->channel, info->encrytion_mode);
                        }
                    }
                }
            }
        }
    }
}
/**
 * http://depletionregion.blogspot.ch/2013/10/cc3000-smart-config-transmitting-ssid.html
 * 
 * @param buf
 * @param len
 */
IRAM void wifi_t::smartlink_analyze(uint8_t* buf, int len)
{
    if(m_CurrentTag == TAG_NONE) {
        m_CurrentMarker = 0;
        m_GotMarker     = false;
        
        if(len == TAG_SSID && m_SsidCnt != SIG_DONE) {
            //DTXT("TAG_SSID\n");
            m_CurrentTag = TAG_SSID;
            m_SsidLen    = 0;
            m_SsidCnt    = SIG_BEGIN;
        }
        else if(len == TAG_PSWD && m_PswdCnt != SIG_DONE) {
            //DTXT("TAG_PSWD\n");
            m_CurrentTag = TAG_PSWD;
            m_PswdLen    = 0;
            m_PswdCnt    = SIG_BEGIN;
        }
    }
    else if(m_CurrentTag == TAG_SSID) {
        if(len == MARKER_1 || len == MARKER_2) {
            if(m_GotMarker == true) {
                m_CurrentTag = TAG_NONE;
            }
            else {
                m_GotMarker = true;
            }
        }
        else {
            if(m_GotMarker == false) {
                m_CurrentTag = TAG_NONE;
            }
            else if(m_SsidCnt == SIG_BEGIN) {        // expects length of SSID at this point
                m_SsidLen   = (len - OFFSET_L) * 2;
                m_SsidCnt   = 0;
                m_GotMarker = false;
                
                //DTXT("TAG_SSID, m_SsidLen = %d\n", m_SsidLen / 2);
            }
            else if(m_SsidCnt < m_SsidLen) {
                m_GotMarker = false;
                
                //
                // decode
                //
                int idx = m_SsidCnt % 2;
                
                m_NibblesSsid[idx] = (len - OFFSET_C) & 0xF;
                
                // do error checking
                if(m_SsidCnt > 0) {
                    // get low nibble from previous packet length
                    uint8_t prevNibble = m_NibblesSsid[(idx == 0) ? 1 : 0];
                    
                    // get high nibble from this packet length
                    uint8_t hiNibble = ((len - OFFSET_C) & 0xF0) >> 4;
                    
                    // simple XOR
                    uint8_t x        = prevNibble ^ (m_SsidCnt % 16);
                    
                    if(hiNibble != x) {
                        m_CurrentTag = TAG_NONE;       // error, start again
                    }
                    
                    //DTXT("TAG_SSID, prevNibble = %02X, hiNibble = %02X, x = %02X\n", prevNibble, hiNibble, x);
                }
                
                if(idx == 1) {
                    m_SsidBuf[m_SsidCnt / 2] = m_NibblesSsid[1] | (m_NibblesSsid[0] << 4);
                }
                
                ++m_SsidCnt;
            }
            else if(m_SsidCnt == m_SsidLen) {
                DTXT("wifi_t::smartlink_analyze(): TAG_SSID, done; m_SsidLen = %d\n", m_SsidLen / 2);
                
                //for(int i = 0; i < m_SsidLen / 2; ++i) {
                //    DTXT("TAG_SSID, m_SsidBuf[%d] = %02X (%c)\n", i, m_SsidBuf[i], m_SsidBuf[i]);
                //}
                
                m_CurrentTag = TAG_NONE;
                m_SsidCnt    = SIG_DONE;        // mark that we're done
            }
            else {
                DTXT("wifi_t::smartlink_analyze(): TAG_SSID, invalid; m_SsidLen = %d\n", m_SsidLen);                

                m_CurrentTag = TAG_NONE;
            }
        }
    }
    else if(m_CurrentTag == TAG_PSWD) {
        if(len == MARKER_1 || len == MARKER_2) {
            if(m_GotMarker == true) {
                m_CurrentTag = TAG_NONE;
            }
            else {
                m_GotMarker = true;
            }
        }
        else {
            if(m_GotMarker == false) {
                m_CurrentTag = TAG_NONE;
            }
            else if(m_PswdCnt == SIG_BEGIN) {        // expects length of password at this point
                m_PswdLen   = (len - OFFSET_L) * 2;
                m_PswdCnt   = 0;
                m_GotMarker = false;
                
                //DTXT("TAG_PSWD, m_PswdLen = %d\n", m_PswdLen / 2);
            }
            else if(m_PswdCnt < m_PswdLen) {
                m_GotMarker = false;
                
                //
                // decode
                //
                int idx = m_PswdCnt % 2;
                
                m_NibblesPswd[idx] = (len - OFFSET_C) & 0xF;
                
                // do error checking
                if(m_PswdCnt > 0) {
                    // get low nibble from previous packet length
                    uint8_t prevNibble = m_NibblesPswd[(idx == 0) ? 1 : 0];
                    
                    // get high nibble from this packet length
                    uint8_t hiNibble = ((len - OFFSET_C) & 0xF0) >> 4;
                    
                    // simple XOR
                    uint8_t x        = prevNibble ^ (m_PswdCnt % 16);
                    
                    if(hiNibble != x) {
                        m_CurrentTag = TAG_NONE;       // error, start again
                    }
                    
                    //DTXT("TAG_PSWD, prevNibble = %02X, hiNibble = %02X, x = %02X\n", prevNibble, hiNibble, x);
                }
                
                if(idx == 1) {
                    m_PswdBuf[m_PswdCnt / 2] = m_NibblesPswd[1] | (m_NibblesPswd[0] << 4);
                }
                
                ++m_PswdCnt;
            }
            else if(m_PswdCnt == m_PswdLen) {
                DTXT("wifi_t::smartlink_analyze(): TAG_PSWD, done; m_PswdLen = %d\n", m_PswdLen / 2);
                
                //for(int i = 0; i < m_PswdLen / 2; ++i) {
                //    DTXT("TAG_PSWD, m_PswdBuf[%d] = %02X (%c)\n", i, m_PswdBuf[i], m_PswdBuf[i]);
                //}
                
                m_CurrentTag = TAG_NONE;
                m_PswdCnt    = SIG_DONE;        // mark that we're done
            }
            else {
                DTXT("wifi_t::smartlink_analyze(): TAG_PSWD, invalid; m_PswdLen = %d\n", m_PswdLen / 2);                

                m_CurrentTag = TAG_NONE;
            }
        }
    }
}
/**
 * 
 * @param arg
 * @param status
 */
void wifi_t::_smartlink_scan_done_callback(void* arg, sdk_scan_status_t status)
{
    wifi_global.smartlink_scan_done_callback((sdk_bss_info*) arg, status);
}
/**
 * 
 * @param buf
 * @param len
 */
void wifi_t::_smartlink_promiscuous_rx_callback(uint8_t* buf, uint16_t len)
{
    wifi_global.smartlink_promiscuous_rx_callback(buf, len);
}

#endif