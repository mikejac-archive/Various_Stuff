/* 
 * ESP8266 FreeRTOS Firmware
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
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#ifndef TASK_MDNS_HPP
#define	TASK_MDNS_HPP

#include <com/tholusi/esp-open-rtos/thread/task.hpp>
#include <com/tholusi/esp-open-rtos/wifi/wifi.hpp>

/******************************************************************************************************************
 * Mdns
 *
 */
class Mdns: public esp_open_rtos::thread::task_t
{
public:
    /**
     * 
     */
    Mdns()
    {}
    /**
     * 
     * @param wifi
     * @return 
     */
    int init(esp_open_rtos::wifi::wifi_t& wifi);
    
private:
    /**
     * 
     */
    void task();

    esp_open_rtos::wifi::wifi_t* m_Wifi;

    // no copy and no = operator
    Mdns(const Mdns&);
    Mdns &operator=(const Mdns&);
};


#endif	/* TASK_MDNS_HPP */

