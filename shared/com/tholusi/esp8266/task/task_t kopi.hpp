/* 
 * File:   task_t.hpp
 * Author: michael
 *
 * Created on 19. november 2014, 11:30
 */

#ifndef COM_THOLUSI_ESP8266_TASK_T_HPP
#define	COM_THOLUSI_ESP8266_TASK_T_HPP

#include <com/espressif/esp8266/cplusplus/cplusplus.hpp>
#include <com/espressif/esp8266/include/os_type.h>
#include <com/espressif/esp8266/include/user_interface.h>
#include <stdarg.h>

namespace com {
namespace tholusi {
namespace esp8266 {

/******************************************************************************************************************
 * forward declarations
 *
 */
class task_t;
//class console_t;

/******************************************************************************************************************
 * esp8266_t
 *
 */
class esp8266_t
{
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR esp8266_t()
    {
    }
    /**
     * 
     */
    ICACHE_FLASH_ATTR ~esp8266_t()
    {
    }
    /**
     * 
     */
    //inline void ICACHE_FLASH_ATTR start()
    //{
        
    //}
    /**
     * 
     */
    void ICACHE_FLASH_ATTR reset();
    /**
     * 
     * @param task
     * @param task_id
     * @param qlen
     * @return 
     */
    int ICACHE_FLASH_ATTR add(task_t& task, uint8 task_id, uint8 qlen = 1);
    /**
     * 
     * @param console
     * @return 
     */
    /*int setConsole(console_t* console)
    {
        m_Console = console;
        
        if(m_Console == 0) {
            return -1;
        }
        
        return 0;
    }*/
    /**
     * 
     * @return 
     */
    /*console_t* getConsole()
    {
        return m_Console;
    }*/
    /**
     * 
     * @param proc
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR info(const char* proc, const char *fmt, ...);
    /**
     * 
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR error(const char *fmt, ...);
    /**
     * 
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR warning(const char *fmt, ...);
    
protected:
    static const int PRINT_BUF_SIZE = 256;
    
private:
    /**
     * 
     * @param type
     * @param proc
     * @param fmt
     * @param argList
     * @return 
     */
    int ICACHE_FLASH_ATTR message(int type, const char* proc, const char* fmt, va_list argList);
    
    //console_t*  m_Console;
};

void ICACHE_FLASH_ATTR _task_wrapper(os_event_t* e);

/******************************************************************************************************************
 * task_t
 *
 */
class task_t
{
    friend void ICACHE_FLASH_ATTR _task_wrapper(os_event_t* e);
    friend void __task(void *ptr);
    friend class esp8266_t;
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR task_t() : m_Os(0)//, m_Queue(0)
    {
    }
    /**
     * 
     */
    ICACHE_FLASH_ATTR ~task_t()
    {
    }
    /**
     * 
     */
    virtual void runTask(os_signal_t sig) = 0;
    /**
     * 
     * @return 
     */
    inline ICACHE_FLASH_ATTR esp8266_t* os()
    {
        return m_Os;
    }
    
protected:
    /**
     * 
     * @return 
     */
    uint32_t millis()
    {
        return system_get_time() / 1000;
    }
    /**
     *
     * @param milliseconds
     */
    inline void ICACHE_FLASH_ATTR sleep(const uint16_t milliseconds)
    {
        os_delay_us((const uint32_t) milliseconds * 1000);
    }
    /**
     * 
     */
    //inline void yield()
    //{
        //taskYIELD();
    //}
    /**
     * 
     */
    //inline void enterCritical()
    //{
        //taskENTER_CRITICAL();
    //}
    /**
     * 
     */
    //inline void exitCritical()
    //{
        //taskEXIT_CRITICAL();
    //}
    /**
     * 
     * @param c
     * @return 
     */
    //int putchar(int c);
    
    esp8266_t*          m_Os;
    //os_event_t*         m_Queue;
    //uint8               m_Id;
    
private:
    /**
     * 
     * @param os
     * @param task_id
     * @param qlen
     * @return 
     */
    int config(esp8266_t* os, uint8 task_id, uint8 qlen = 1);
    /**
     * 
     * @param ptr
     */
    static void ICACHE_FLASH_ATTR __task(os_event_t* e);
    
    // no copy and no = operator
    task_t(const task_t&);
    task_t &operator=(const task_t&);    
};

}
}
}

#endif	/* COM_THOLUSI_ESP8266_TASK_HPP */

