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

#include <com/tholusi/esp-open-rtos/date_time/system_time.h>
//#include "espressif/esp_common.h"
//#include "espressif/sdk_private.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "esp8266.h"
//nclude "common_macros.h"


#include "esp/timer.h"


const int freq_frc1 = 1;        // 1Hz = once per second

static volatile esp_time_t _system_time;
static volatile esp_time_t _uptime;

/**
 * 
 */
static void frc1_interrupt_handler(void)
{
    _system_time++;
    _uptime++;
}
/**
 * 
 * @return 
 */
int esp_start_system_time()
{
    _system_time  = 0;
    _uptime       = 0;
    
    TIMER(0).CTRL = VAL2FIELD(TIMER_CTRL_CLKDIV, TIMER_CLKDIV_256) | TIMER_CTRL_RELOAD;
    TIMER(0).LOAD = 0x200000;
    
    DPORT.INT_ENABLE |= DPORT_INT_ENABLE_TIMER0;
    _xt_isr_attach(INUM_TIMER_FRC1, frc1_interrupt_handler);
    _xt_isr_unmask(1<<INUM_TIMER_FRC1);

    TIMER(0).CTRL |= TIMER_CTRL_RUN;
    
    
    
    
    
    // stop timer and mask it's interrupt as a precaution
    /*timer_set_interrupts(TIMER_FRC1, false);
    timer_set_run(TIMER_FRC1, false);
    
    _system_time = 0;
    _uptime      = 0;
    
    // set up ISR
    _xt_isr_attach(INUM_TIMER_FRC1, frc1_interrupt_handler);
    
    // configure timer frequency
    timer_set_frequency(TIMER_FRC1, freq_frc1);
    
    // unmask interrupt and start timer
    timer_set_interrupts(TIMER_FRC1, true);
    timer_set_run(TIMER_FRC1, true);*/

    return 0;
}
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_time(esp_time_t* timer)
{
    if(timer != NULL) {
        *timer = _system_time;
    }
    
    return _system_time;
}
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_uptime(esp_time_t* timer)
{
    if(timer != NULL) {
        *timer = _uptime;
    }
    
    return _uptime;
}
/**
 * 
 * @param t
 * @return 
 */
int esp_stime(esp_time_t *t)
{
    if(t != NULL) {
        _system_time = *t;
        return 0;
    }
    
    return -1;
}

