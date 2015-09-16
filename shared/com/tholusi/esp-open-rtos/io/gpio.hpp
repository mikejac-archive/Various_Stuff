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

#ifndef COM_THOLUSI_ESP_OPEN_RTOS_GPIO_HPP
#define	COM_THOLUSI_ESP_OPEN_RTOS_GPIO_HPP

#include "espressif/esp_common.h"
#include "espressif/sdk_private.h"

#define DTXT(...)   printf(__VA_ARGS__)

namespace esp_open_rtos {
namespace io {

const int MAX_GPIO = 16;

#define GPIO_initializer_GPIO_INPUT                 { GPIO_INPUT,             0, 0 }
#define GPIO_initializer_GPIO_OUTPUT                { GPIO_OUTPUT,            0, 0 }
#define GPIO_initializer_GPIO_OUTPUT_OPEN_DRAIN     { GPIO_OUTPUT_OPEN_DRAIN, 0, 0 }
#define GPIO_initializer_GPIO_INPUT_PULLUP          { GPIO_INPUT_PULLUP,      0, 0 }

typedef struct {
    gpio_direction_t    gpio_direction;
    unsigned long       millis;
    uint8_t             val;
} gpio_stat_t;

/******************************************************************************************************************
 * gpio_t
 *
 */
template<class gpio_project_t>
class gpio_t
{
public:
    /**
     * 
     * @param gpio_project
     */
    gpio_t(gpio_project_t& gpio_project)
    {
        m_Gpio_project = &gpio_project;
        
        for(int i = 0; i < MAX_GPIO + 1; ++i) {
            if(m_Gpio_project->pinIsValid(i) == true) {
                gpio_enable(i, m_Gpio_project->m_GpioStat[i].gpio_direction);
            }
        }
    }
    /**
     * 
     * @param gpio_num
     * @param direction
     * @return 
     */
    int enable(uint8_t gpio_num, gpio_direction_t direction)
    {
        if(m_Gpio_project->pinIsValid(gpio_num) == 0) {
            m_Gpio_project->m_GpioStat[gpio_num].gpio_direction = direction;
            
            gpio_enable(gpio_num, direction);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int disable(uint8_t gpio_num)
    {
        if(m_Gpio_project->pinIsValid(gpio_num) == 0) {
            m_Gpio_project->m_GpioStat[gpio_num].gpio_direction = (gpio_direction_t) 0xFF;
            
            gpio_disable(gpio_num);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @param set
     * @return 
     */
    int write(uint8_t gpio_num, bool set)
    {
        //DTXT("gpio_t::write(): gpio_direction = %d\n", (int)(m_Gpio_project->m_GpioStat[gpio_num].gpio_direction));
        
        if(m_Gpio_project->pinIsValid(gpio_num) == true && (m_Gpio_project->m_GpioStat[gpio_num].gpio_direction == GPIO_OUTPUT || m_Gpio_project->m_GpioStat[gpio_num].gpio_direction == GPIO_OUT_OPEN_DRAIN)) {
            gpio_write(gpio_num, set);

            // record when it was changed
            m_Gpio_project->m_GpioStat[gpio_num].millis = millis();
        }
        else {
            //DTXT("gpio_t::write(): error\n");
            return -1;
        }

        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int read(uint8_t gpio_num)
    {
        if(m_Gpio_project->pinIsValid(gpio_num) == true) {
            // record when it was read
            m_Gpio_project->m_GpioStat[gpio_num].millis = millis();
            
            return gpio_read(gpio_num);
        }
        else {
            return -1;
        }
        
        return 0;
    }
    /**
     * 
     * @param gpio_num
     * @return 
     */
    int toggle(uint8_t gpio_num)
    {
        if(m_Gpio_project->pinIsValid(gpio_num) == true && (m_Gpio_project->m_GpioStat[gpio_num].gpio_direction == GPIO_OUTPUT || m_Gpio_project->m_GpioStat[gpio_num].gpio_direction == GPIO_OUT_OPEN_DRAIN)) {
            gpio_toggle(gpio_num);
            
            // record when it was changed
            m_Gpio_project->m_GpioStat[gpio_num].millis = millis();
        }
        else {
            return -1;
        }
        
        return 0;
    }
    
protected:
    gpio_project_t* m_Gpio_project;
};

} // namespace io {
} // namespace esp_open_rtos {

#endif /* COM_THOLUSI_ESP_OPEN_RTOS_GPIO_HPP */
