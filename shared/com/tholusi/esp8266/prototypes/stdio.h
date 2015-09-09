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

#ifndef COM_THOLUSI_ESP8266_STDIO_H
#define	COM_THOLUSI_ESP8266_STDIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <com/espressif/esp8266/include/osapi.h>
//#include <com/espressif/esp8266/include/c_types.h>
//#include <com/espressif/esp8266/include/ets_sys.h>

#define sprintf     ets_sprintf

int ets_printf ( const char * format, ... );
int ets_sprintf ( char * str, const char * format, ... );
int sprintf ( char * str, const char * format, ... );
int os_printf_plus ( const char * format, ... );

#ifdef	__cplusplus
}
#endif

#endif	/* COM_THOLUSI_ESP8266_STDIO_H */

