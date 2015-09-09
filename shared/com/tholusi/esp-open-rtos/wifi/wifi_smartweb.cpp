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

#if defined(WITH_SMARTWEB)

#include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>
#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

using namespace esp_open_rtos::wifi;

#define DTXT(...)       printf(__VA_ARGS__)

const int BUFFER_SIZE = 512;
const int BACKLOG     = 1;

#define HEADER200   "HTTP/1.1 200 OK\r\n"
#define HEADER404   "HTTP/1.1 404 Not Found\r\n"

#define HEADER_TYPE "Content-Type: text/html\r\n"
#define HEADER_LEN  "Content-Length: %u\r\n\r\n"

#define RESPONSE_BEGIN  "<html>\r\n"                                    \
                            "<head>\r\n"                                \
                                "<title>ESP8266 Configuration</title>"  \
                            "</head>\r\n"                               \
                            "<body>\r\n"
                                                
#define RESPONSE_END        "</body>\r\n"                               \
                        "</html>\r\n"

#define RESPONSE_FORM   "<p>Set data for network to connect to</p>"                 \
                        "<form action=\"set.exe\" method=\"post\">"                 \
                            "SSID: <input type=\"text\" name=\"ssid\"><br>"         \
                            "Shared Key: <input type=\"text\" name=\"key\"><br>"    \
                            "<input type=\"submit\" value=\"Submit\">"              \
                        "</form>"

#define RESPONSE_DONE   "<p>Done!</p>"






#define WIFI_AP_NAME    "ESP_0001"

/*enum {
    EVENT_STAMODE_CONNECTED = 0,
    EVENT_STAMODE_DISCONNECTED,
    EVENT_STAMODE_AUTHMODE_CHANGE,
    EVENT_STAMODE_GOT_IP,
    EVENT_SOFTAPMODE_STACONNECTED,
    EVENT_SOFTAPMODE_STADISCONNECTED,
    EVENT_MAX
};
typedef struct {
        uint8_t ssid[32];
        uint8_t ssid_len;
        uint8_t bssid[6];
        uint8_t channel;
} Event_StaMode_Connected_t;

typedef struct {
        uint8_t ssid[32];
        uint8_t ssid_len;
        uint8_t bssid[6];
        uint8_t reason;
} Event_StaMode_Disconnected_t;

typedef struct {
        uint8_t old_mode;
        uint8_t new_mode;
} Event_StaMode_AuthMode_Change_t;

typedef struct {
        struct ip_addr ip;
        struct ip_addr mask;
        struct ip_addr gw;
} Event_StaMode_Got_IP_t;

typedef struct {
        uint8_t mac[6];
        uint8_t aid;
} Event_SoftAPMode_StaConnected_t;

typedef struct {
        uint8_t mac[6];
        uint8_t aid;
} Event_SoftAPMode_StaDisconnected_t;

typedef union {
        Event_StaMode_Connected_t                       connected;
        Event_StaMode_Disconnected_t            disconnected;
        Event_StaMode_AuthMode_Change_t         auth_change;
        Event_StaMode_Got_IP_t                          got_ip;
        Event_SoftAPMode_StaConnected_t         sta_connected;
        Event_SoftAPMode_StaDisconnected_t      sta_disconnected;
} Event_Info_u;
typedef struct _esp_event {
    uint32_t event;
    Event_Info_u event_info;
} System_Event_t;

typedef void (* wifi_event_handler_cb_t)(System_Event_t *event);

void sdk_wifi_set_event_handler_cb(wifi_event_handler_cb_t cb);*/

/*static char* m_P;
static char* m_Pp;
static int   m_I;
static int   m_N;*/

/**
 * 
 * @param buffer
 * @param len
 * @return 
 */
//int smartweb_token_begin(char* buffer, int len)
//{
    /*m_P     = buffer;
    m_Pp    = buffer;
    m_N     = len;
    m_I     = 0;*/
    
//    return 0;
//}
/**
 * 
 * @param token
 * @param len
 * @return 
 */
//int XXX__smartweb_token_next(char** token, int& len)
//{
    /*while(m_I < m_N) {
        if(*m_P == ' ' || *m_P == '\r') {
            int ret = *m_P;
            
            *m_P = '\0';

            *token = m_Pp;
            len    = m_P - m_Pp;

            m_Pp = m_P + 1;

            ++m_P;
            ++m_I;

            return ret;
        }
        else if(*m_P == '\n') {
            m_Pp = m_P + 1;
        }

        ++m_P;
        ++m_I;
    }*/
    
//    return -1;
//}

/******************************************************************************************************************
 * class wifi_t
 *
 */

/**
 * 
 * @return 
 */
wifi_t::state_t wifi_t::do_wifi_smartweb()
{
    DTXT("wifi_t::do_wifi_smartweb(): begin\n");
    
    state_t state;
    
    sdk_wifi_set_opmode(SOFTAP_MODE);
    //sdk_wifi_softap_dhcps_stop();

    struct sdk_softap_config apconfig;
    
    if(sdk_wifi_softap_get_config(&apconfig)) {
        DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_get_config() success\n");
        
        memset(apconfig.ssid,     0, sizeof(apconfig.ssid));
        memset(apconfig.password, 0, sizeof(apconfig.password));

        strcpy((char*)apconfig.ssid, WIFI_AP_NAME);

        apconfig.authmode       = AUTH_OPEN;
        apconfig.ssid_hidden    = 0;
        apconfig.max_connection = 4;
        apconfig.channel        = 7;

        if(!sdk_wifi_softap_set_config(&apconfig)) {
            DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() failed\n");
            state = wifi_smartweb_fail;
        }
        else {
            DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() success\n");
            state = wifi_smartweb_done;;
        }
    }
    else {
        DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_get_config() failed\n");
        state = wifi_smartweb_fail;
    }
    
    //sdk_wifi_set_event_handler_cb(0);
    /*wifi_set_event_handler_cb(wifi_event_cb);

     LOCAL struct ip_info info;
     IP4_ADDR(&info.ip, 192, 168, 22, 1);
     IP4_ADDR(&info.gw, 192, 168, 22, 1);
     IP4_ADDR(&info.netmask, 255, 255, 255, 0);
     wifi_set_ip_info(SOFTAP_IF, &info);

     struct dhcps_lease dhcp_lease;
     IP4_ADDR(&dhcp_lease.start_ip, 192, 168, 22, 2);
     IP4_ADDR(&dhcp_lease.end_ip, 192, 168, 22, 5);
     wifi_softap_set_dhcps_lease(&dhcp_lease);

     wifi_softap_dhcps_start();
     ets_uart_printf("SOFTAP Status:%d\r\n",wifi_softap_dhcps_status());*/
    
    DTXT("wifi_t::do_wifi_smartweb(): end\n");
    
    return state;
}
/**
 * 
 * @param port
 * @return 
 */
int wifi_t::smartweb_run(int port)
{
    DTXT("wifi_t::smartweb_run(): begin\n");

    int sd = lwip_socket(AF_INET, SOCK_STREAM, 0);
    
    if(sd < 0) {
        DTXT("wifi_t::smartweb_run(): lwip_socket()\n");
        return -1;
    }    
    
    char* buffer = (char*) malloc(BUFFER_SIZE);
    
    if(buffer == 0) {
        DTXT("wifi_t::smartweb_run(): buffer == 0\n");
        close(sd);
        return -1;
    }
    
    // bind to an address
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_port        = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(lwip_bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    	DTXT("wifi_t::smartweb_run(): lwip_bind()\n");
        free(buffer);
        close(sd);
    	return -1;
    }
    
    if(lwip_listen(sd, BACKLOG) < 0) {
    	DTXT("wifi_t::smartweb_run(): lwip_listen()\n");
        free(buffer);
        close(sd);
    	return -1;
    }
    
    bool run = true;

    while(run) {
        socklen_t b = sizeof(serveraddr);
        int c;

        DTXT("wifi_t::smartweb_run(): lwip_accept() start\n");

        if((c = lwip_accept(sd, (struct sockaddr *)&serveraddr, &b)) < 0) {
            DTXT("wifi_t::smartweb_run(): lwip_accept()\n");
            free(buffer);
            close(sd);
            return -1;
        }

        DTXT("wifi_t::smartweb_run(): lwip_accept() done\n");

        ssize_t n = recv(c, buffer, BUFFER_SIZE, 0);

        DTXT("wifi_t::smartweb_run(): n = %d\n", n);

        if(n < 0) {

        }
        else if(n == 0) {

        }
        else {
            //DTXT("[%s]\n\n\n\n", buffer);

            int   get_detect_     = 0;
            int   post_detect_    = 0;
            
            char* content        = NULL;
            char* http_version   = NULL;
            int   content_length = 0;
            
            if(smartweb_token_begin(buffer, n) == 0) {
                int   len   = 0;
                int   ret   = -1;
                char* token = NULL;

                do {
                    ret = smartweb_token_next(&token, len);
                    
                    DTXT("token [%s] len %d\n", token, len);

                    /*if(get_detect_ == 0) {
                        if(len == 3) {
                            if(strcmp(token, "GET") == 0) {
                                ++get_detect_;
                            }
                        }
                    }
                    
                    if(get_detect_ == 1) {
                        if(len == 1) {
                            if(token[0] == '/') {
                                //++get_detect_;
                            }
                        }
                    }
                    
                    if(get_detect_ == 2) {
                        if(len == 8) {
                            http_version = token;
                            ++get_detect_;
                        }
                    }
                    
                    if(post_detect_ == 0) {
                        if(len == 4) {
                            if(strcmp(token, "POST") == 0) {
                                ++post_detect_;
                            }
                        }
                    }
                    
                    if(post_detect_ == 1) {
                        if(len == 8) {
                            if(strcmp(token, "/set.exe") == 0) {
                                ++post_detect_;
                            }
                        }
                    }
                    
                    if(post_detect_ == 2) {
                        if(len == 15) {
                            if(strcmp(token, "Content-Length:") == 0) {
                                ++post_detect_;
                            }
                        }
                    }
                    
                    if(post_detect_ == 3) {
                        //content_length = atoi(token);
                        //++post_detect_;
                    }*/

                    /*if((get_detect_ == 0) && (len == 3) && (strcmp(token, "GET") == 0)) {
                        ++get_detect_;
                    }
                    else if((get_detect_ == 1) && (len == 1) && (token[0] == '/')) {
                        //++get_detect_;
                    }
                    else if((get_detect_ == 2) && (len == 8)) {
                        http_version = token;
                        ++get_detect_;
                    }
                    else if((post_detect_ == 0) && (len == 4) && (strcmp(token, "POST") == 0)) {
                        ++post_detect_;
                    }
                    else if((post_detect_ == 1) && (len == 8) && (strcmp(token, "/set.exe") == 0)) {
                        ++post_detect_;
                    }
                    else if((post_detect_ == 2) && (len == 15) && (strcmp(token, "Content-Length:")) == 0) {
                        ++post_detect_;
                    }
                    else if(post_detect_ == 3) {
                        //content_length = atoi(token);
                        //++post_detect;
                    }*/
                    
                    //DTXT("token [%s] len %d\n", token, len);
                } while(ret != -1 && get_detect_ != 3);
                
                content = token;
            }

            if(get_detect_ == 3) {
                DTXT("wifi_t::smartweb_run(): http_version = [%s]\n", http_version);

                strcpy(buffer, HEADER200);
                strcat(buffer, HEADER_TYPE);

                sprintf(buffer + strlen(buffer), HEADER_LEN, sizeof(RESPONSE_BEGIN) + sizeof(RESPONSE_FORM) + sizeof(RESPONSE_END));

                lwip_send(c, buffer,         strlen(buffer),         0);
                lwip_send(c, RESPONSE_BEGIN, sizeof(RESPONSE_BEGIN), 0);
                lwip_send(c, RESPONSE_FORM,  sizeof(RESPONSE_FORM),  0);
                lwip_send(c, RESPONSE_END,   sizeof(RESPONSE_END),   0);
            }
            else if(post_detect_ == 4) {
                content += 2;       // get to the payload
                
                DTXT("wifi_t::smartweb_run(): content_length = %d\n", content_length);
                
                char ssid[64];
                char pswd[32];
                
                char* p = content;
                
                while(p - content < content_length) {
                    if(strncmp(p, "ssid", 4) == 0) {
                        p += 4;
                        
                        if(*p == '=') {
                            ++p;
                            
                            for(int i = 0; p - content < content_length; ++i, ++p) {
                                if(*p == '&') {
                                    ssid[i] = '\0';
                                    break;
                                }
                                
                                ssid[i] = *p;
                            }
                        }
                    }
                    else if(strncmp(p, "key", 3) == 0) {
                        p += 3;
                        
                        if(*p == '=') {
                            ++p;
                            
                            for(int i = 0; p - content < content_length; ++i, ++p) {
                                if(*p == '&') {
                                    pswd[i] = '\0';
                                    break;
                                }
                                
                                pswd[i] = *p;
                            }
                        }
                    }
                    else {
                        ++p;
                    }
                }
                
                DTXT("wifi_t::smartweb_run(): ssid = [%s], pswd = [%s]\n", ssid, pswd);
                
                strcpy(buffer, HEADER200);
                strcat(buffer, HEADER_TYPE);

                sprintf(buffer + strlen(buffer), HEADER_LEN, sizeof(RESPONSE_BEGIN) + sizeof(RESPONSE_DONE) + sizeof(RESPONSE_END));

                lwip_send(c, buffer,         strlen(buffer),         0);
                lwip_send(c, RESPONSE_BEGIN, sizeof(RESPONSE_BEGIN), 0);
                lwip_send(c, RESPONSE_DONE,  sizeof(RESPONSE_DONE),  0);
                lwip_send(c, RESPONSE_END,   sizeof(RESPONSE_END),   0);
                
                run = false;
            }
            else {
                strcpy(buffer, HEADER404);
                strcat(buffer, HEADER_TYPE);

                sprintf(buffer + strlen(buffer), HEADER_LEN, 0);

                DTXT("%s", buffer);

                lwip_send(c, buffer, strlen(buffer), 0);
            }
        }

        lwip_close(c);
    }
    
    lwip_close(sd);
    
    free(buffer);
    
    DTXT("wifi_t::smartweb_run(): end\n");
    
    return 0;
}

/*static char* m_P;
static char* m_Pp;
static int   m_I;
static int   m_N;*/

/**
 * 
 * @param buffer
 * @param len
 * @return 
 */
int wifi_t::smartweb_token_begin(char* buffer, int len)
{
    m_P     = buffer;
    m_Pp    = buffer;
    m_N     = len;
    m_I     = 0;
    
    return 0;
}
/**
 * 
 * @param token
 * @param len
 * @return 
 */
int wifi_t::smartweb_token_next(char** token, int& len)
{
    while(m_I < m_N) {
        if(*m_P == ' ' || *m_P == '\r') {
            int ret = *m_P;
            
            *m_P = '\0';

            *token = m_Pp;
            len    = m_P - m_Pp;

            m_Pp = m_P + 1;

            ++m_P;
            ++m_I;

            return ret;
        }
        else if(*m_P == '\n') {
            m_Pp = m_P + 1;
        }

        ++m_P;
        ++m_I;
    }
    
    return -1;
}










/*void wifi_event_cb(System_Event_t *evt) {
   //static int serverinit=0;
   switch (evt->event) {
      case EVENT_SOFTAPMODE_STACONNECTED:
         //ets_uart_printf("station: " MACSTR " join, AID = %d\n",
         //MAC2STR(evt->event_info.sta_connected.mac),
         //evt->event_info.sta_connected.aid);
         //Start Web Server
         //if(!serverinit) {
         //   user_webserver_init(SERVER_PORT);
         //   serverinit=1;
         //}
         //Start periodic loop
         //os_timer_disarm(&loop_timer);
         //os_timer_setfn(&loop_timer, (os_timer_func_t *)loop_cb, (void *)0);
         //os_timer_arm(&loop_timer, DELAY, 10);
         break;
      case EVENT_SOFTAPMODE_STADISCONNECTED:
         //ets_uart_printf("station: " MACSTR " leave, AID = %d\n",
         //MAC2STR(evt->event_info.sta_disconnected.mac),
         //evt->event_info.sta_disconnected.aid);
         //Stop Web Server
         //???;
         //Stop periodic loop
         //os_timer_disarm(&loop_timer);
         break;
      default:
         break;
   }
}*/
#endif