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

#ifndef IO_HPP
#define	IO_HPP

#if defined(REMOTE_BUILD)
    #include <com/tholusi/esp-open-rtos/io/gpio.hpp>
#else
    #include <io/gpio.hpp>
#endif

#define DTXT(...)   printf(__VA_ARGS__)

// outputs on the ESP-12 Outdoor Socket board
const int GPIO_U2   = 4;
const int GPIO_U3   = 5;
const int GPIO_BLUE = 2;

/******************************************************************************************************************
 * MyGpio
 *
 */
class MyGpio
{
public:
    MyGpio()
    {}
    /**
     * 
     * @param gpio_num
     * @return 
     */
    bool pinIsValid(uint8_t gpio_num)
    {
        //DTXT("MyGpio::pinIsValid(): gpio_num = %u\n", gpio_num);
        
        switch(gpio_num) {
            case 0:     return false;
            case 1:     return false;       // true;
            case 2:     return true;        // blue LED on ESP-12E
            case 3:     return false;
            case 4:     return true;        // To U2
            case 5:     return true;        // To U3
            case 6:     return false;
            case 7:     return false;
            case 8:     return false;
            case 9:     return false;
            case 10:    return false;
            case 11:    return false;
            case 12:    return true;        // From VO1
            case 13:    return true;        // From VO2
            case 14:    return false;       // true;
            case 15:    return false;       // must be connected to GND in order for the ESP-12E to work
            case 16:    return false;       // true;
            default:    return false;
        }
    }

//protected:    
    esp_open_rtos::io::gpio_stat_t m_GpioStat[esp_open_rtos::io::MAX_GPIO + 1] = {
        /* 0 */     GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_OUTPUT,                   // To blue LED on ESP-12E
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_OUTPUT,                   // To U2
        /* 5 */     GPIO_initializer_GPIO_OUTPUT,                   // To U3
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,
        /* 10 */    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP,             // From VO1
                    GPIO_initializer_GPIO_INPUT_PULLUP,             // From VO2
                    GPIO_initializer_GPIO_INPUT_PULLUP,
        /* 15 */    GPIO_initializer_GPIO_INPUT_PULLUP,
                    GPIO_initializer_GPIO_INPUT_PULLUP
    };

private:
    // no copy and no = operator
    MyGpio(const MyGpio&);
    MyGpio &operator=(const MyGpio&);    
};

typedef esp_open_rtos::io::gpio_t<MyGpio> MyGpio_t;

#endif	/* IO_HPP */

