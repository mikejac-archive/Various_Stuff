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
    #include <com/tholusi/esp-open-rtos/net/mdns.hpp>
    #include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
#else
    #include <net/mdns.hpp>
    #include <cplusplus/cplusplus.hpp>
#endif

using namespace esp_open_rtos::ip;

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * class mdns_t
 *
 */

/*static unsigned int put8(uint8_t* data, int8_t val)
{
    *data = val;
    
    return sizeof(int8_t);
}*/

/*static unsigned int put16(uint8_t* data, int16_t val)
{
    uint16_t* p = (uint16_t*)data;
    
    *p = val;
    
    return sizeof(int16_t);
}*/

int mdns_t::init()
{
    return 0;
}

int mdns_t::open()
{
    DTXT("mdns_t::open(): begin\n");
    
    //uint8_t data[64];
    uint8_t data[] = {
        0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x07, 0x61, 0x70, 0x70,
        0x6c, 0x65, 0x74, 0x76, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x01, 0x80, 0x01, 0x00,
        0x00, 0x78, 0x00, 0x00, 0x04, 0x99, 0x6d, 0x07, 0x5a, 0xc0, 0x0c, 0x00, 0x1c, 0x80, 0x01, 0x00,
        0x00, 0x78, 0x00, 0x00, 0x10, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x23, 0x32,
        0xff, 0xfe, 0xb1, 0x21, 0x52, 0xc0, 0x0c, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00,
        0x08, 0xc0, 0x0c, 0x00, 0x04, 0x40, 0x00, 0x00, 0x08        
    };
    /*unsigned int offset = 0;
    
    offset += put16(&data[offset], 0);          // ID
    offset += put16(&data[offset], 0x8400);     // flags
    offset += put16(&data[offset], 0);          // QDCOUNT
    offset += put16(&data[offset], 1);          // ANCOUNT
    offset += put16(&data[offset], 0);          // NSCOUNT
    offset += put16(&data[offset], 2);          // ARCOUNT

    offset += put8(&data[offset], 7);           // string length
    offset += put8(&data[offset], 'a');
    offset += put8(&data[offset], 'p');
    offset += put8(&data[offset], 'p');
    offset += put8(&data[offset], 'l');
    offset += put8(&data[offset], 'e');
    offset += put8(&data[offset], 't');
    offset += put8(&data[offset], 'v');
    
    
    offset += put8(&data[offset], 5);           // string length
    offset += put8(&data[offset], 'l');
    offset += put8(&data[offset], 'o');
    offset += put8(&data[offset], 'c');
    offset += put8(&data[offset], 'a');
    offset += put8(&data[offset], 'l');
    
    int datalen = offset;*/
    int datalen = sizeof(data);

    DTXT("mdns_t::open(): datalen = %d\n", datalen);
    
    if((m_Sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        DTXT("mdns_t::open(): error; could not create socket\n");
        return -1;
    }
    
    struct sockaddr_in addr;

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(5353);
    addr.sin_addr.s_addr = inet_addr("224.0.0.251");
    
    int n = lwip_sendto(m_Sock, data, datalen, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    
    if(n == -1) {
        DTXT("mdns_t::open(): error; lwip_sendto() == -1\n");
    }
    else if(n != datalen) {
        DTXT("mdns_t::open(): error; lwip_sendto() n != datalen\n");
    }
    else {
        DTXT("mdns_t::open(): success\n");
    }
    
    DTXT("mdns_t::open(): end\n");
    
    return 0;
}

int mdns_t::close()
{
    if(m_Sock >= 0) {
        lwip_close(m_Sock);
    }
    
    return 0;
}

