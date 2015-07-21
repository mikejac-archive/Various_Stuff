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
#include <com/tholusi/esp8266/task/task_t.hpp>

using namespace com::tholusi::esp8266;

/******************************************************************************************************************
 * global data
 *
 */

#if defined(N_TASKS)
    #if (N_TASKS >= 1)
        task0_t* task0_t::m_Task;
        
        #if (N_TASKS >= 2)
            task1_t* task1_t::m_Task;
            
            #if (N_TASKS >= 3)
                task2_t* task2_t::m_Task;
            #endif
        #endif
    #endif
#endif

/******************************************************************************************************************
 * esp8266_t
 *
 */

/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR esp8266_t::init(int encoderBufSize, int decoderBufSize, int notifBufSize)
{
    return m_JsonRpc.init(encoderBufSize, decoderBufSize, notifBufSize);
}
/**
 * 
 */
void ICACHE_FLASH_ATTR esp8266_t::reset()
{
    // http://www.esp8266.com/viewtopic.php?f=6&t=2111
    system_restart();
}
