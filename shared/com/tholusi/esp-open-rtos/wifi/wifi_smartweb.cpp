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
#include <com/github/esp-open-rtos/dhcpserver/dhcpserver.h>
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
    
    //sdk_wifi_set_opmode(SOFTAP_MODE);
    
    //struct ip_info ap_ip;
    //IP4_ADDR(&ap_ip.ip,      172, 16,  0, 1);
    //IP4_ADDR(&ap_ip.gw,      0,   0,   0, 0);
    //IP4_ADDR(&ap_ip.netmask, 255, 255, 0, 0);
    //sdk_wifi_set_ip_info(1, &ap_ip);

    //struct sdk_softap_config ap_config;
    
    //if(sdk_wifi_softap_get_config(&ap_config)) {
    //    DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_get_config() success\n");
        
        //memset(ap_config.ssid,     0, sizeof(ap_config.ssid));
        //memset(ap_config.password, 0, sizeof(ap_config.password));

        //strcpy((char*)ap_config.ssid, WIFI_AP_NAME);

        //ap_config.ssid_len        = strlen(WIFI_AP_NAME);
        //ap_config.authmode        = AUTH_OPEN;
        //ap_config.ssid_hidden     = 0;
        //ap_config.max_connection  = 4;
        //ap_config.channel         = 7;
        //ap_config.beacon_interval = 100;
        
        //if(!sdk_wifi_softap_set_config(&ap_config)) {
        //    DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() failed\n");
        //    state = wifi_smartweb_fail;
        //}
        //else {
        //    DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_set_config() success\n");
            
            ip_addr_t first_client_ip;
            IP4_ADDR(&first_client_ip, 172, 16, 0, 2);
            dhcpserver_start(&first_client_ip, 4);
            
            state = wifi_smartweb_run;
        //}
    //}
    //else {
    //    DTXT("wifi_t::do_wifi_smartweb(): sdk_wifi_softap_get_config() failed\n");
    //    state = wifi_smartweb_fail;
    //}
    
    DTXT("wifi_t::do_wifi_smartweb(): end\n");
    
    return state;
}

wifi_t::state_t wifi_t::smartweb_start_server(int port)
{
    DTXT("wifi_t::smartweb_start_server(): begin\n");

    /*m_Sc     = -1;
    m_Sd     = -1;
    
    m_Buffer = (char*) malloc(BUFFER_SIZE);
    
    if(m_Buffer == 0) {
        DTXT("wifi_t::smartweb_start_server(): m_Buffer == 0\n");
        return wifi_smartweb_fail;
    }
    
    m_Sd = lwip_socket(AF_INET, SOCK_STREAM, 0);
    
    if(m_Sd < 0) {
        DTXT("wifi_t::smartweb_start_server(): lwip_socket()\n");
        free(m_Buffer);
        return wifi_smartweb_fail;
    }    
    
    int flags = lwip_fcntl(m_Sd, F_GETFL, 0);
    
    if(flags < 0) {
    	DTXT("wifi_t::smartweb_start_server(): lwip_fcntl() F_GETFL\n");
        free(m_Buffer);
        lwip_close(m_Sd);
    	return wifi_smartweb_fail;
    }
    
    flags |= O_NONBLOCK;
    
    if(lwip_fcntl(m_Sd, F_SETFL, flags) < 0) {
    	DTXT("wifi_t::smartweb_start_server(): lwip_fcntl() F_SETFL\n");
        free(m_Buffer);
        lwip_close(m_Sd);
    	return wifi_smartweb_fail;
    }
    
    // bind to an address
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_port        = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(lwip_bind(m_Sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    	DTXT("wifi_t::smartweb_start_server(): lwip_bind()\n");
        free(m_Buffer);
        lwip_close(m_Sd);
    	return wifi_smartweb_fail;
    }
    
    if(lwip_listen(m_Sd, BACKLOG) < 0) {
    	DTXT("wifi_t::smartweb_start_server(): lwip_listen()\n");
        free(m_Buffer);
        lwip_close(m_Sd);
    	return wifi_smartweb_fail;
    }*/
    
    DTXT("wifi_t::smartweb_start_server(): end\n");
    
    return wifi_smartweb_in_progress;
}


wifi_t::state_t wifi_t::smartweb_run_server()
{
    state_t state = wifi_smartweb_in_progress;
    
    //DTXT("wifi_t::smartweb_run_server(): begin\n");

    /*fd_set readset;

    FD_ZERO(&readset); 
    FD_SET(m_Sd, &readset);
    
    int maxfd = m_Sd;

    if(m_Sc >= 0) {
        FD_SET(m_Sc, &readset);
        maxfd = max(maxfd, m_Sc);
    }

    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    
    lwip_select(maxfd + 1, &readset, NULL, NULL, &timeout);

    // a new connection?
    if(FD_ISSET(m_Sd, &readset)) {
        DTXT("wifi_t::smartweb_run_server(): FD_ISSET(m_Sd, &readset)\n");
        
        struct sockaddr serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        
        socklen_t b = sizeof(serveraddr);
        
        if((m_Sc = lwip_accept(m_Sd, &serveraddr, &b)) < 0) {
            DTXT("wifi_t::smartweb_run_server(): lwip_accept()\n");
            state = wifi_smartweb_fail;
        }
    }

    // existing client connection
    if(m_Sc >= 0 && FD_ISSET(m_Sc, &readset)) {
        DTXT("wifi_t::smartweb_run_server(): m_Sc >= 0 && FD_ISSET(m_Sc, &readset)\n");
        
        ssize_t n = lwip_recv(m_Sc, m_Buffer, BUFFER_SIZE, 0);
        
        if(n < 0) {

        }
        else if(n == 0) {

        }
        else {
            DTXT("[%s]\n", m_Buffer);
        }
        
        lwip_close(m_Sc);
        m_Sc = -1;
    }*/
    
    //DTXT("wifi_t::smartweb_run_server(): end\n");
    
    return state;
}

wifi_t::state_t wifi_t::smartweb_stop_server()
{
    lwip_close(m_Sd);
    
    free(m_Buffer);
    
    return wifi_smartweb_done;
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