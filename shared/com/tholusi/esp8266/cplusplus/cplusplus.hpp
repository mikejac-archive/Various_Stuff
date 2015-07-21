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

#ifndef COM_THOLUSI_ESP8266_CPLUSPLUS_HPP
#define	COM_THOLUSI_ESP8266_CPLUSPLUS_HPP

#ifdef	__cplusplus
extern "C" {
#endif

#include <com/espressif/esp8266/include/c_types.h>
#include <com/espressif/esp8266/include/osapi.h>
#include <com/espressif/esp8266/include/mem.h>
    
#ifdef	__cplusplus
}
#endif

#include <com/tholusi/esp8266/prototypes/osapi.h>

/******************************************************************************************************************
 * 
 *
 */

inline void *operator new(size_t size) 
{
    return os_malloc(size);
}

inline void *operator new[](size_t size) 
{
    return os_malloc(size);
}

inline void operator delete(void * ptr)
{
    os_free(ptr);
}

inline void operator delete[](void * ptr) 
{
    os_free(ptr);
}

extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));

void ICACHE_FLASH_ATTR do_global_ctors(void);

#endif	/* COM_THOLUSI_ESP8266_CPLUSPLUS_HPP */

