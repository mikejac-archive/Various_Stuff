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

#ifndef COM_THOLUSI_ESP_OPEN_RTOS_MQTT_BLUEMIX_HPP
#define	COM_THOLUSI_ESP_OPEN_RTOS_MQTT_BLUEMIX_HPP

#include <com/realtimelogic/json/esp_open_rtos/JEncoder.h>
#include <com/realtimelogic/json/esp_open_rtos/JVal.h>
#include <stdint.h>
#include <string.h>

#include "mqtt_config.hpp"

namespace esp_open_rtos {
namespace mqtt {

/******************************************************************************************************************
 * JParserAllocator
 *
 */
class JParserAllocator: public AllocatorIntf
{
    uint8_t*    buf;
    size_t      m_BufSize;
    /**
     * 
     * @param o
     * @param size
     * @return 
     */
    static void* malloc(AllocatorIntf* o, size_t* size);
    /**
     * 
     * @param o
     * @param memblock
     * @param size
     * @return 
     */
    static void* realloc(AllocatorIntf* o, void* memblock, size_t* size);
    /**
     * 
     * @param o
     * @param memblock
     */
    static void free(AllocatorIntf* o, void* memblock);
    
public:
    /**
     * 
     */
    JParserAllocator() : AllocatorIntf(malloc, realloc, free)
    {
    }
    /**
     * 
     * @param bufSize
     * @return 
     */
    int init(int bufSize);
};

/******************************************************************************************************************
 * bluemix_t
 *
 */
class bluemix_t
{
public:
    /**
     * 
     * @param encoderBufSize
     * @param decoderBufSize
     * @return 
     */
    int bmixInit(int encoderBufSize = 1 + 256, int decoderBufSize = 1 + 256);
    /**
     * 
     * @param buf
     * @param size
     * @return 
     */
    JVal* bmixDecoderBegin(void* buf, uint32_t size);
    /**
     * 
     * @return 
     */
    int bmixDecoderEnd();
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixGetString(const char* name, const char** value);
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixGetInt(const char* name, int32_t& value);
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixGetInt(const char* name, int& value);
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixGetLong(const char* name, int64_t& value);
    /**
     * 
     * @return 
     */
    JErr* bmixErr()
    {
        return m_Err;
    }
    /**
     * 
     * @return 
     */
    int bmixEncoderBegin();
    /**
     * 
     * @return 
     */
    char* bmixEncoderEnd();
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixAddString(const char* name, const char* value);
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixAddInt(const char* name, int32_t value);
    /**
     * 
     * @param name
     * @param value
     * @return 
     */
    int bmixAddLong(const char* name, int64_t value);
    
protected:
    // encoder
    char*               m_EncoderBuffer;
    BufPrint            m_Out;
    JEncoder*           m_Encoder;
    
    // decoder
    char                m_MemberName[20];
    JParser*            m_Parser;
    JParserValFact*     m_ValFact;
    JParserAllocator    m_Allocator;
    
    // both
    JErr*               m_Err;    

private:
    /**
     * 
     * @param name
     * @return 
     */
    JVal* bmixFind(const char* name);
    
    JVal* m_Jv_d;           // used by the decoder
    
};

} // namespace mqtt {
} // namespace esp_open_rtos {

#endif	/* COM_THOLUSI_ESP_OPEN_RTOS_MQTT_BLUEMIX_HPP */

