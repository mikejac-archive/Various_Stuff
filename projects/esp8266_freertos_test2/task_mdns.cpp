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

#include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#include <com/tholusi/esp-open-rtos/date_time/system_time.h>
#include <org/bitbucket/geekman/tinysvcmdns/mdns.h>
#include <org/bitbucket/geekman/tinysvcmdns/mdnsd.h>
#include <stdio.h>
#include <string.h>

#include "task_mdns.hpp"

//#include "lwip/err.h"
#include "lwip/sockets.h"
//#include "lwip/sys.h"
//#include "lwip/netdb.h"
//#include "lwip/dns.h"

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * Mdns
 *
 */

int Mdns::init(esp_open_rtos::wifi::wifi_t& wifi)
{
    m_Wifi = &wifi;
    
    if(m_Wifi == 0) {
        return -1;
    }
    
    return 0;
}

const int xMDNS_PORT = 5353;
//struct sockaddr_in sock_w;
//struct sockaddr to;
//socklen_t len_w;
    
int create_send_socket()
{
    int sdw = lwip_socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sdw < 0) {
        DTXT("create_send_socket(): socket()\n");
        return sdw;
    }

    //memset(&sock_w, 0, sizeof(sock_w));
    
    //sock_w.sin_family      = AF_INET;
    //sock_w.sin_port        = htons(xMDNS_PORT);
    //sock_w.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //len_w = sizeof(sock_w);
    
    unsigned char ttl = 255;
    
    if(lwip_setsockopt(sdw, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        DTXT("create_send_socket(): setsockopt(IP_MULTICAST_TTL)\n");
        lwip_close(sdw);
        return -1;
    }
    
    return sdw;
}
/**
 * 
 */
void Mdns::task()
{
    DTXT("Mdns::task(): start\n");
    
    struct mdnsd* server = (struct mdnsd*) malloc(sizeof(struct mdnsd));
    
    if(server == NULL) {
        DTXT("Mdns::task(): server == NULL\n");
        sleep(10000);
        return;
    }
    
    memset(server, 0, sizeof(struct mdnsd));
    //server->sockfd = -1;
    
    //server->sockfd = create_recv_sock();
    
    //if(server->sockfd < 0) {
    //    DTXT("Mdns::task(): server->sockfd < 0\n");
    //    free(server);
    //    return;
    //}
    
    struct mdns_pkt* mdns_reply = (struct mdns_pkt*) malloc(sizeof(struct mdns_pkt));
    
    if(mdns_reply == NULL) {
        DTXT("Mdns::task(): mdns_reply == NULL\n");
        sleep(10000);
        free(server);
        return;
    }
    
    memset(mdns_reply, 0, sizeof(struct mdns_pkt));

    void *pkt_buffer = malloc(PACKET_SIZE);
    
    if(pkt_buffer == NULL) {
        DTXT("Mdns::task(): pkt_buffer == NULL\n");
        sleep(10000);
        free(mdns_reply);
        free(server);
        return;
    }
    
    // create host entries
    char hostname[] =  "target.local"; //"MyDeviceName.local";
    //char hostname[] =  "CC3200.local";

    //mdnsd_set_hostname(server, hostname, m_Wifi->ipaddr() /*inet_addr("192.168.0.29")*/);

    //struct rr_entry *a2_e = NULL;
    //a2_e = rr_create_a(create_nlabel(hostname), m_Wifi->ipaddr() /*inet_addr("192.168.0.31")*/);
    //mdnsd_add_rr(server, a2_e);
    
    const char *txt[] = {
        "dev=CC3200",
        "vendor=Texas-Instruments",
        NULL
    };
    
    /*struct mdns_service *svc =*/ //mdnsd_register_svc(server, "CC3000"  /*"MyDeviceName"*/, /*"_http._tcp.local"*/ "_device-info._udp.local", 1234, NULL, txt);
    //mdnsd_register_svc(server, "My Website 2", "_http._tcp.local", 9090, NULL, txt);
    
    int sdw = -1;
    
    while(true) {
        /******************************************************************************************************************
         * 
         *
         */
        if(m_Wifi->isConnected()) {
            DTXT("Mdns::task(): run\n");
            
            //if(server->sockfd < 0) {
            if(sdw < 0) {
                //DTXT("Mdns::task(): server->sockfd < 0\n");
                DTXT("Mdns::task(): sdw < 0\n");
                
                if((sdw = create_send_socket()) >= 0) {
                    mdnsd_set_hostname(server, hostname, m_Wifi->ipaddr());

                    struct rr_entry *a2_e = NULL;
                    a2_e = rr_create_a(create_nlabel(hostname), m_Wifi->ipaddr());
                    mdnsd_add_rr(server, a2_e);
                    
                    mdnsd_register_svc(server, "CC3200"  /*"MyDeviceName"*/, /*"_http._tcp.local"*/ "_device-info._udp.local", 1234, NULL, txt);
                }
                
                //server->sockfd = create_recv_sock(m_Wifi->ipaddr());
            }
            
            //if(server->sockfd >= 0) {
            if(sdw >= 0) {
                // send out announces
                if(server->announce) {
                    DTXT("Mdns::task(): server->announce\n");
                    
                    struct rr_entry *ann_e = NULL;
                    
                    ann_e = rr_list_first(&server->announce);
                    
                    if(!ann_e) {
                        DTXT("Mdns::task(): !ann_e\n");
                        break;
                    }
                    
                    //do {
                        char *namestr = nlabel_to_str(ann_e->name);
                        DTXT("Mdns::task(): sending announce for %s\n", namestr);
                        free(namestr);

                        announce_srv(server, mdns_reply, ann_e->name);

                        if(mdns_reply->num_ans_rr > 0) {
                            size_t replylen = mdns_encode_pkt(mdns_reply, (uint8_t*) pkt_buffer, PACKET_SIZE);

                            if(send_packet(sdw, pkt_buffer, replylen) < 0) {
                            //if(lwip_sendto(sdw, pkt_buffer, replylen, 0, (struct sockaddr*)&sock_w, len_w) < 0) {
                                DTXT("Mdns::task(): lwip_sendto\n");
                            }
                        }
                        
                        //ann_e = rr_list_next(&server->announce, ann_e);
                        
                    //} while(ann_e != NULL);
                    
                }
                //while(true) {
                    /*struct rr_entry *ann_e = NULL;

                    // extract from head of list
                    if(server->announce) {
                        //ann_e = rr_list_remove(&server->announce, server->announce->e);
                        ann_e = rr_list_first(&server->announce);
                    }

                    if(!ann_e) {
                        DTXT("Mdns::task(): !ann_e\n");
                        break;
                    }

                    char *namestr = nlabel_to_str(ann_e->name);
                    DTXT("Mdns::task(): sending announce for %s (%s)\n", namestr, (char*)(ann_e->name));
                    free(namestr);

                    announce_srv(server, mdns_reply, ann_e->name);

                    if(mdns_reply->num_ans_rr > 0) {
                        size_t replylen = mdns_encode_pkt(mdns_reply, (uint8_t*) pkt_buffer, PACKET_SIZE);

                        send_packet(server->sockfd, pkt_buffer, replylen);
                    }*/
                //}
            }

            sleep(5000);
        }
        else {
            sleep(2000);
        }
    }
}

