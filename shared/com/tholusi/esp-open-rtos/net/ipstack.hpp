/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Benjamin Cabe - generic IPStack
 *    Michael Jacobsen (github.com/mikejac)
 *******************************************************************************/

#ifndef ESP_OPEN_RTOS_IPSTACK_HPP
#define	ESP_OPEN_RTOS_IPSTACK_HPP

#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

namespace esp_open_rtos {
namespace ip {

#define DTXT(...)   printf(__VA_ARGS__)

//
// these macros are are defined in "lwip/sockets.h" and messes with the ipstack_t names
//
#undef connect
#undef write
#undef read

/******************************************************************************************************************
 * class ipstack_t
 *
 */
class ipstack_t
{
public:
    /**
     * 
     */
    ipstack_t()
    {
        mysock = -1;
    }
    /**
     * 
     * @param hostname
     * @param port
     * @return 
     */
    int connect(const char * hostname, int port)
    {
        int                rc     = -1;
        struct sockaddr_in address;
        struct addrinfo *  result = NULL;
        struct addrinfo    hints  = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};

        //DTXT("ipstack_t::connect(): begin\n");
        
        if((rc = lwip_getaddrinfo(hostname, NULL, &hints, &result)) == 0) {
            //DTXT("ipstack_t::connect(): lwip_getaddrinfo() == 0\n");

            struct addrinfo * res = result;

            // prefer ipv4 addresses
            while(res) {
                if(res->ai_family == AF_INET) {
                    result = res;
                    break;
                }
                
                res = res->ai_next;
            }

            if(result->ai_family == AF_INET) {
                //DTXT("ipstack_t::connect(): result->ai_family == AF_INET\n");
                
                address.sin_port   = htons(port);
                address.sin_family = AF_INET;
                address.sin_addr   = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
            }
            else {
                rc = -1;
            }
            
            lwip_freeaddrinfo(result);
        }
        else {
            //DTXT("ipstack_t::connect(): lwip_getaddrinfo() != 0\n");
        }
        
        if(rc == 0) {
            mysock = lwip_socket(address.sin_family, SOCK_STREAM, 0);
            
            if(mysock >= 0) {
                //DTXT("ipstack_t::connect(): mysock >= 0\n");
                
                rc = lwip_connect(mysock, (struct sockaddr*)&address, sizeof(address));
                
                if(rc != 0) {
                    //DTXT("ipstack_t::connect(): connect failed\n");                
                    disconnect();
                }
            }
            else {
                //DTXT("ipstack_t::connect(): mysock < 0\n");                
            }
        }

        //DTXT("ipstack_t::connect(): end; rc = %d\n", rc);
        
        return rc;
    }
    /**
     * 
     * @param buffer
     * @param len
     * @param timeout_ms
     * @return 
     */
    int read(unsigned char * buffer, int len, int timeout_ms)
    {
        //DTXT("ipstack_t::read(): begin; mysock = %d\n", mysock);
        
        lwip_setsockopt(mysock, SOL_SOCKET, SO_RCVTIMEO, &timeout_ms, sizeof(int));

        int bytes = 0;
        
        while(bytes < len) {
            //DTXT("ipstack_t::read(): lwip_recv begin\n");
            
            int rc = lwip_recv(mysock, &buffer[bytes], (size_t)(len - bytes), 0);

            //DTXT("ipstack_t::read(): lwip_recv end\n");

            if(rc == -1) {
                if(Socket_error("read") != 0) {
                    bytes = -1;
                    break;
                }
            }
            else {
                bytes += rc;
            }
        }

        //DTXT("ipstack_t::read(): end; bytes = %d\n", bytes);
        
        return bytes;
    }
    /**
     * 
     * @param buffer
     * @param len
     * @param timeout_ms
     * @return 
     */
    int write(const unsigned char * buffer, int len, int timeout_ms)
    {
        lwip_setsockopt(mysock, SOL_SOCKET, SO_SNDTIMEO, &timeout_ms, sizeof(int));

        int rc = lwip_write(mysock, buffer, len);
        
        return rc;
    }
    /**
     * 
     * @return 
     */
    int disconnect()
    {
        int rc = lwip_close(mysock);
        
        mysock = -1;
        
        return rc;
    }
    /**
     * 
     * @return 
     */
    bool isConnected()
    {
        return (mysock >= 0) ? true : false;
    }
    
protected:
    /**
     * 
     * @param aString
     * @return 
     */
    int Socket_error(const char * aString)
    {
        int error;
        u32_t optlen = sizeof(error);
        
        lwip_getsockopt(mysock, SOL_SOCKET, SO_ERROR, &error, &optlen);
        
        //if (errno != EINTR && errno != EAGAIN && errno != EINPROGRESS && errno != EWOULDBLOCK)
        if(error != EAGAIN) {
            if(/*strcmp(aString, "shutdown") != 0 ||*/ (error != ENOTCONN && error != ECONNRESET)) {
                //DTXT("ipstack_t::Socket_error(): error '%s' in %s for socket %d; error = %d, errno = %d\r\n", strerror(errno), aString, mysock, error, errno);
            }
        }
        //else {
        //    error = 0;
        //}
        
        return error;
    }

private:
    int mysock;
};

} // namespace ip {
} // namespace esp_open_rtos {

#endif
