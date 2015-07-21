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

#ifndef COM_THOLUSI_ESP8266_TASK_T_HPP
#define	COM_THOLUSI_ESP8266_TASK_T_HPP

//#include <com/espressif/esp8266/include/os_type.h>
//#include <com/espressif/esp8266/include/user_interface.h>
#include <com/tholusi/esp8266/cplusplus/includes.hpp>
#include <com/tholusi/esp8266/jsonrpc/json_rpc_t.hpp>
#include <stdarg.h>

#define N_TASKS  2

namespace com {
namespace tholusi {
namespace esp8266 {

const uint8_t TASK0_ID      = 0;
const uint8_t TASK1_ID      = 1;
const uint8_t TASK2_ID      = 2;

const uint8_t QUEUE_SIZE    = 2;

/******************************************************************************************************************
 * forward declarations
 *
 */

#if defined(N_TASKS)
    #if (N_TASKS >= 1)
        class task0_t;
        
        #if (N_TASKS >= 2)
            class task1_t;
            
            #if (N_TASKS >= 3)
                class task3_t;
            #endif
        #endif
    #endif
#endif

/******************************************************************************************************************
 * esp8266_t
 *
 */
class esp8266_t
{
public:
    /**
     * 
     * @param encoderBufSize
     * @param decoderBufSize
     * @param notifBufSize
     * @return 
     */
    int ICACHE_FLASH_ATTR init(int encoderBufSize = 2048, int decoderBufSize = 2048, int notifBufSize = 1024);
    /**
     * 
     */
    void ICACHE_FLASH_ATTR reset();
    /**
     * 
     * @return 
     */
    json_rpc_t* ICACHE_FLASH_ATTR jsonRpc()
    {
        return &m_JsonRpc;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR millis()
    {
        return system_get_time() / 1000;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR freeHeap()
    {
        return system_get_free_heap_size();
    }
    /**
     * 
     * @param p1
     */
    inline void ICACHE_FLASH_ATTR setPtr1(void* p1)
    {
        m_Ptr1 = p1;
    }
    /**
     * 
     * @param p1
     */
    inline void ICACHE_FLASH_ATTR setPtr2(void* p2)
    {
        m_Ptr2 = p2;
    }
    /**
     * 
     * @param p1
     */
    inline void ICACHE_FLASH_ATTR setPtr3(void* p3)
    {
        m_Ptr3 = p3;
    }
    /**
     * 
     * @return 
     */
    inline void* ICACHE_FLASH_ATTR getPtr1()
    {
        return m_Ptr1;
    }
    /**
     * 
     * @return 
     */
    inline void* ICACHE_FLASH_ATTR getPtr2()
    {
        return m_Ptr2;
    }
    /**
     * 
     * @return 
     */
    inline void* ICACHE_FLASH_ATTR getPtr3()
    {
        return m_Ptr3;
    }

protected:
    void*   m_Ptr1;
    void*   m_Ptr2;
    void*   m_Ptr3;
    
private:
    json_rpc_t  m_JsonRpc;
};

#if defined(N_TASKS)

/******************************************************************************************************************
 * task0_t
 *
 */
#if (N_TASKS >= 1)
class task0_t
{
    friend void ICACHE_FLASH_ATTR _task(os_event_t *e);
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR task0_t() : m_LastRun(0), m_RunInterval(1000)
    {
    }
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR init(esp8266_t* os)
    {
        m_Task = this;
        m_Os   = os;
        
        // create the so-called task
        system_os_task(task0_t::_task, TASK0_ID, m_Queue, QUEUE_SIZE);

        // post to ourselves
        system_os_post(TASK0_ID, 0, 0);
        
        return 0;
    }
    
protected:
    /**
     * 
     * @param runInterval
     */
    inline void ICACHE_FLASH_ATTR setRunInterval(uint32_t runInterval)
    {
        m_RunInterval = runInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR getRunInterval()
    {
        return m_RunInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR millis()
    {
        return m_Os->millis();
    }
    /**
     * 
     * @return 
     */
    inline ICACHE_FLASH_ATTR esp8266_t* os()
    {
        return m_Os;
    }
    
    os_event_t  m_Queue[QUEUE_SIZE];
    uint32_t    m_LastRun;
    uint32_t    m_RunInterval;    
    
private:
    /**
     * 
     * @param e
     */
    virtual void ICACHE_FLASH_ATTR runTask(os_event_t* e);
    /**
     * 
     * @param e
     */
    static void /*ICACHE_FLASH_ATTR*/ _task(os_event_t* e)
    {
        task0_t::m_Task->runTask(e);
        
#if (N_TASKS == 1)
        // post to ourselves
        system_os_post(TASK0_ID, 0, 0);
#else
        // post to task 1
        system_os_post(TASK1_ID, 0, 0);        
#endif
    }
    
    esp8266_t*          m_Os;
    
    static task0_t*     m_Task;
    
    // no copy and no = operator
    task0_t(const task0_t&);
    task0_t &operator=(const task0_t&);    
};

/******************************************************************************************************************
 * task1_t
 *
 */
#if (N_TASKS >= 2)
class task1_t
{
    friend void ICACHE_FLASH_ATTR _task(os_event_t *e);
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR task1_t() : m_LastRun(0), m_RunInterval(1000)
    {
    }
    /**
     * 
     * @param os
     * @return 
     */
    int ICACHE_FLASH_ATTR init(esp8266_t* os)
    {
        m_Task = this;
        m_Os   = os;
        
        // create the so-called task
        system_os_task(task1_t::_task, TASK1_ID, m_Queue, QUEUE_SIZE);

        // post to ourselves
        system_os_post(TASK1_ID, 0, 0);
        
        return 0;
    }
    
protected:
    /**
     * 
     * @param runInterval
     */
    inline void ICACHE_FLASH_ATTR setRunInterval(uint32_t runInterval)
    {
        m_RunInterval = runInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR getRunInterval()
    {
        return m_RunInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR millis()
    {
        return m_Os->millis();
    }
    /**
     * 
     * @return 
     */
    inline ICACHE_FLASH_ATTR esp8266_t* os()
    {
        return m_Os;
    }

    os_event_t  m_Queue[QUEUE_SIZE];
    uint32_t    m_LastRun;
    uint32_t    m_RunInterval;
    
private:
    virtual void ICACHE_FLASH_ATTR runTask(os_event_t* e);
    
    static void /*ICACHE_FLASH_ATTR*/ _task(os_event_t* e)
    {
        task1_t::m_Task->runTask(e);
        
#if (N_TASKS == 2)
        // post to task 0
        system_os_post(TASK0_ID, 0, 0);
#else
        // post to task 2
        system_os_post(TASK2_ID, 0, 0);        
#endif
    }

    esp8266_t*          m_Os;
    
    static task1_t*     m_Task;
        
    // no copy and no = operator
    task1_t(const task1_t&);
    task1_t &operator=(const task1_t&);    
};
/******************************************************************************************************************
 * task2_t
 *
 */
#if (N_TASKS >= 3)
class task2_t
{
    friend void ICACHE_FLASH_ATTR _task(os_event_t *e);
    
public:
    ICACHE_FLASH_ATTR task2_t() : m_LastRun(0), m_RunInterval(1000)
    {
        
    }
    
    int ICACHE_FLASH_ATTR init(esp8266_t* os)
    {
        m_Task = this;
        m_Os   = os;
        
        // create the so-called task
        system_os_task(task2_t::_task, TASK2_ID, m_Queue, QUEUE_SIZE);

        // post to ourselves
        system_os_post(TASK2_ID, 0, 0);
        
        return 0;
    }
    
protected:
    /**
     * 
     * @param runInterval
     */
    inline void ICACHE_FLASH_ATTR setRunInterval(uint32_t runInterval)
    {
        m_RunInterval = runInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR getRunInterval()
    {
        return m_RunInterval;
    }
    /**
     * 
     * @return 
     */
    inline uint32_t ICACHE_FLASH_ATTR millis()
    {
        return m_Os->millis();
    }
    /**
     * 
     * @return 
     */
    inline ICACHE_FLASH_ATTR esp8266_t* os()
    {
        return m_Os;
    }

    os_event_t  m_Queue[QUEUE_SIZE];
    uint32_t    m_LastRun;
    uint32_t    m_RunInterval;
        
private:
    virtual void ICACHE_FLASH_ATTR runTask(os_event_t* e);
    
    static void /*ICACHE_FLASH_ATTR*/ _task(os_event_t* e)
    {
        task2_t::m_Task->runTask(e);
        
        // post to task 0
        system_os_post(TASK0_ID, 0, 0);
    }
    
    esp8266_t*          m_Os;
    
    static task2_t*     m_Task;
    
    // no copy and no = operator
    task2_t(const task2_t&);
    task2_t &operator=(const task2_t&);    
};
#endif  // N_TASKS >= 3
#endif  // N_TASKS >= 2
#endif  // N_TASKS >= 1
#endif
}
}
}

#endif	/* COM_THOLUSI_ESP8266_TASK_HPP */

