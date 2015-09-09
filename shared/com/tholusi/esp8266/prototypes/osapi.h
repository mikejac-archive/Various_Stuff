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

#ifndef COM_THOLUSI_ESP8266_OSAPI_H
#define	COM_THOLUSI_ESP8266_OSAPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <com/espressif/esp8266/include/osapi.h>
#include <com/espressif/esp8266/include/c_types.h>
#include <com/espressif/esp8266/include/ets_sys.h>
//#include <com/espressif/esp8266/include/ip_addr.h>
    
#define min_t(type, x, y) ({                     \
         type __min1 = (x);                      \
         type __min2 = (y);                      \
         __min1 < __min2 ? __min1: __min2; })
 
#define max_t(type, x, y) ({                     \
         type __max1 = (x);                      \
         type __max2 = (y);                      \
         __max1 > __max2 ? __max1: __max2; })
    
//#define sprintf     ets_sprintf
#define memset      ets_memset
#define memcpy      ets_memcpy
#define strncpy     ets_strncpy
#define strlen      ets_strlen
#define strncmp     ets_strncmp
    
//int ets_printf ( const char * format, ... );
//int ets_sprintf ( char * str, const char * format, ... );
//int sprintf ( char * str, const char * format, ... );

//int os_printf_plus ( const char * format, ... );

void * ets_memset ( void * ptr, int value, size_t num );
void * memset ( void * ptr, int value, size_t num );

void * ets_memcpy ( void * destination, const void * source, size_t num );
void * memcpy ( void * destination, const void * source, size_t num );

void * ets_memmove ( void * destination, const void * source, size_t num );
void * memmove ( void * destination, const void * source, size_t num );

int ets_memcmp ( const void * ptr1, const void * ptr2, size_t num );
int memcmp ( const void * ptr1, const void * ptr2, size_t num );

char * ets_strcpy ( char * destination, const char * source );
char * strcpy ( char * destination, const char * source );

size_t ets_strlen ( const char * str );
size_t strlen ( const char * str );

int ets_strcmp ( const char * str1, const char * str2 );

int ets_strncmp ( const char * str1, const char * str2, size_t num );
int strncmp ( const char * str1, const char * str2, size_t num );

char * ets_strncpy ( char * destination, const char * source, size_t num );
char * strncpy ( char * destination, const char * source, size_t num );

char * strcat ( char * destination, const char * source );

int strcmp ( const char *, const char * );
const char * strchr ( const char * str, int character );
const char * strrchr ( const char * str, int character );
//int stricmp( const char *, const char *);

int ets_delay_us(unsigned long);

void* pvPortMalloc (size_t size);
void* pvPortZalloc (size_t size);
void vPortFree (void* ptr);

void ets_bzero ( void * ptr, size_t size );

typedef void (*int_handler_t)(void*);

void ets_isr_mask(int intr);
void ets_isr_unmask(int intr);
void ets_isr_attach(int intr, int_handler_t handler, void *arg);
void ets_intr_lock();
void ets_intr_unlock();

void ets_wdt_enable (void);
void ets_wdt_disable (void);
void wdt_feed (void);

void ets_timer_arm_new ( ETSTimer * a, int b, int c, int isMstimer );
void ets_timer_setfn ( ETSTimer * t, ETSTimerFunc * fn, void * parg );
void ets_timer_disarm ( ETSTimer * a );

int ets_uart_printf( const char * format, ... );

int ffs ( int );

void ets_install_putc1(void* routine);
void uart_div_modify(int no, int freq);

uint32_t os_random(void);


//bool wifi_softap_set_station_info(uint8_t chaddr[16], ip_addr_t * client_address );

#ifdef	__cplusplus
}
#endif

#endif	/* COM_THOLUSI_ESP8266_OSAPI_H */

