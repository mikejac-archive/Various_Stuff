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

#include <com/tholusi/esp8266/cplusplus/cplusplus.hpp>

/******************************************************************************************************************
 * 
 *
 */

extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);

void ICACHE_FLASH_ATTR do_global_ctors(void)
{
    void (**p)(void);
    
    for (p = &__init_array_start; p != &__init_array_end; ++p) {
        (*p)();
    }
}

extern "C" void abort() 
{
    while(true); // enter an infinite loop and get reset by the WDT
}

void __cxa_pure_virtual(void) 
{
    abort();
}

void __cxa_deleted_virtual(void) 
{
    abort();
}
