/* 
 * MQTT ESP8266 Firmware
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

#ifndef COM_THOLUSI_ESP8266_STDLIB_H
#define	COM_THOLUSI_ESP8266_STDLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <com/espressif/esp8266/include/mem.h>
#include <com/espressif/esp8266/include/c_types.h>

void * os_malloc (size_t size);
void   os_free (void* ptr);

#ifdef	__cplusplus
}
#endif

#endif	/* COM_THOLUSI_ESP8266_STDLIB_H */

