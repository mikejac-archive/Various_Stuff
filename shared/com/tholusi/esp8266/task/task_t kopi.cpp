/* 
 * File:   task_t.cpp
 * Author: michael
 * 
 * Created on 19. november 2014, 11:30
 */

#include <com/tholusi/esp8266/task/task_t.hpp>
#include <com/espressif/esp8266/json/BufPrint.h>

#define _STR_INFO   "%INFO: "
#define _STR_ERR    "%ERROR: "
#define _STR_WARN   "%WARN: "

typedef struct {
    uint8       m_Id;
    os_event_t  m_Queue;
    uint32_t    m_This;
} task_info_t;

task_info_t task_info[5] = {
    { 0, 3, 0x1111 },
    { 0, 3, 0x2222 },
    { 0, 3, 0x3333 },
    { 0, 3, 0x4444 },
    { 0, 3, 0x5555 },
};
//os_event_t         m_Queue[5] = { 1, 1, 1, 1, 1};

/******************************************************************************************************************
 * protypes
 *
 */

/******************************************************************************************************************
 * functions
 *
 */

using namespace com::tholusi::esp8266;

/**
 * 
 * @param print
 * @param sizeRequired
 * @return 
 */
static int ICACHE_FLASH_ATTR sprintfIsFull(struct BufPrint* print, int sizeRequired)
{
    return -1;  // failed, buffer is full
}

/******************************************************************************************************************
 * task_t
 *
 */

/**
 * 
 * @param os
 * @param task_id
 * @param qlen
 * @return 
 */
int task_t::config(esp8266_t* os, uint8 task_id, uint8 qlen)
{
    os_printf("task_t::config() begin; task_id = %u, qlen = %u: this = %08X\n", task_id, qlen, this);
    
    if(os == 0) {
        os_printf("task_t::config() os == 0\n");

        return -1;
    }
    
    m_Os = os;
    
    //m_Queue = new os_event_t[qlen];
    
    //if(m_Queue == 0) {
    //    os_printf("task_t::config() m_Queue == 0\n");
    //    return -2;
    //}
    
    task_info[task_id].m_Id   = task_id;
    task_info[task_id].m_This = (uint32_t) this;
    //m_Id = task_id;
    
    // create the so-called task
    system_os_task(/*_task_wrapper*/ com::tholusi::esp8266::task_t::__task, task_info[task_id].m_Id, &(task_info[task_id].m_Queue), 3 /*qlen*/);
    
    // send the task a signal
    system_os_post(task_info[task_id].m_Id, 0, task_info[task_id].m_Id /*(os_param_t) this*/);

    //os_printf("task_t::config() m_Id = %u, m_This = %08X\n", task_info[task_id].m_Id, task_info[task_id].m_This);
    
    //os_printf("task_t::config() end\n");
    return 0;
}
/**
 * 
 * @param c
 * @return 
 */
/*int task_t::putchar(int c)
{
    return m_Freertos->getConsole()->putchar(c);
}*/

void ICACHE_FLASH_ATTR com::tholusi::esp8266::_task_wrapper(os_event_t* e)
{
    static int n = 0;
    //os_printf("task_t::__task()\n");
    
    for(int i = 0; i < 5; ++i) {
        if(task_info[i].m_Id == e->par) {
            //if(e->par != 2)
            if(n < 10) {
                ++n;
                os_printf("_task_wrapper: par = %d, m_This = %08X; m_Id = %d\n", e->par, task_info[i].m_This, task_info[i].m_Id);
            }
            
            //task_t* me = (task_t*) task_info[i].m_This;
            
            //me->runTask(e->sig);
            
            os_delay_us(100);
            // send the task a signal
            if(e->par == 1) {
                system_os_post(2, 0, 2);
            }
            else if(e->par == 2) {
                system_os_post(1, 0, 1);
            }
            //system_os_post(task_info[i].m_Id, 0, task_info[i].m_Id);
            
            return;
        }
    }
    
    os_printf("_task_wrapper: task not found; par = %d\n", e->par);    
}
/**
 * 
 * @param ptr
 */
void ICACHE_FLASH_ATTR task_t::__task(os_event_t* e)
{
    static int n = 0;
    //os_printf("task_t::__task()\n");
    
    for(int i = 0; i < 5; ++i) {
        if(task_info[i].m_Id == e->par) {
            os_param_t id = e->par;
            
            //if(e->par != 2)
            //if(n < 10) {
            //    ++n;
            //    os_printf("task_t::__task(): par = %d, m_This = %08X; m_Id = %d\n", e->par, task_info[i].m_This, task_info[i].m_Id);
            //}
            
            task_t* me = (task_t*) task_info[i].m_This;
            
            if(me != 0) {
                me->runTask(e->sig);
            }
            else {
                os_printf("task_t::__task(): par = %d, m_This = %08X; m_Id = %d\n", e->par, task_info[i].m_This, task_info[i].m_Id);
            }
            
            // send the task a signal
            if(id == 0) {
                system_os_post(1, 0, 1);
            }
            else if(id == 1) {
                system_os_post(2, 0, 2);
            }
            else if(id == 2) {
                system_os_post(1, 0, 1);
            }
            else {
                os_printf("task_t::__task(): id = %d\n", id);
            }
            //system_os_post(task_info[i].m_Id, 0, task_info[i].m_Id);
            
            return;
        }
    }
    
    os_printf("task_t::__task(): task not found; par = %d\n", e->par);
    /*task_t* me = (task_t*) e->par;
    
    if(me != 0) {
        if(n < 100) {
            ++n;
            os_printf("task_t::__task(): par = %08X, m_Id = %d\n", e->par, e->sig);
            me->runTask(e->sig);
        }
    }
    else {
        //os_printf("task_t::__task(): me == 0; m_Id = %d\n", me->m_Id);
    }
    
    // send the task a signal
    system_os_post(e->sig, e->sig, e->par);*/
}
/******************************************************************************************************************
 * esp8266_t
 *
 */

/**
 * 
 * @param task
 * @param task_id
 * @param qlen
 * @return 
 */
int ICACHE_FLASH_ATTR esp8266_t::add(task_t& task, uint8 task_id, uint8 qlen)
{
    return task.config(this, task_id, qlen);
}
/**
 * 
 */
void ICACHE_FLASH_ATTR esp8266_t::reset()
{
}
/**
 * 
 * @param type
 * @param proc
 * @param fmt
 * @param argList
 * @return 
 */
int ICACHE_FLASH_ATTR esp8266_t::message(int type, const char* proc, const char* fmt, va_list argList)
{
    int retv;

    char* buf = new char[PRINT_BUF_SIZE];

    if(buf == 0) {
        return -1;
    }

    //va_list varg;
    BufPrint bufPrint;

    BufPrint_constructor(&bufPrint, 0, sprintfIsFull);

    bufPrint.buf     = buf;
    bufPrint.bufSize = (PRINT_BUF_SIZE - 1);    // -1 -> the string terminator
    bufPrint.cursor  = 0;                       // start position is beginning of buf

    //va_start(varg, fmt);
    retv = BufPrint_vprintf(&bufPrint, fmt, argList /*varg*/);
    
    if(retv >= 0) {
        buf[bufPrint.cursor] = 0; // zero terminate string
        retv = bufPrint.cursor;
    }
   
    //va_end(varg);
   
        if(type == 0) {                         // info
            os_printf("%INFO(%s): %s\n", proc, buf);
        }
        else if(type == 1) {                    // warning
            os_printf("%WARN(%s): %s\n", proc, buf);
        }
        else if(type == 2) {                    // error
            os_printf("%ERR(%s): %s\n", proc, buf);
        }
    
    delete[] buf;
    
    return retv;    
}
/**
 * 
 * @param proc
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR esp8266_t::info(const char* proc, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = message(0, proc, fmt, varg);
    
    va_end(varg);
    
    return ret;    
}
/**
 * 
 * @param __fmt
 * @param ...
 */
int ICACHE_FLASH_ATTR esp8266_t::error(const char* fmt, ...)
{
    /*va_list ap;
    
    va_start(ap, __fmt);

    char buffer[MAX_PRINT_LEN + 1];
    
    strcpy(buffer, _STR_ERR);
    
    if(vsnprintf(buffer + sizeof(_STR_ERR) - 1, sizeof(buffer) - sizeof(_STR_ERR), __fmt, ap) != -1) {
        puts(buffer);
    }
    
    va_end(ap);*/
}
/**
 * 
 * @param __fmt
 * @param ...
 */
int ICACHE_FLASH_ATTR esp8266_t::warning(const char* fmt, ...)
{
}
