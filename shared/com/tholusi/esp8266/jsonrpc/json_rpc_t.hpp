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

#ifndef COM_THOLUSI_ESP8266_JSON_RPC_T_HPP
#define	COM_THOLUSI_ESP8266_JSON_RPC_T_HPP

#include <com/espressif/esp8266/include/osapi.h>
#include <com/espressif/esp8266/include/c_types.h>
#include <com/tholusi/esp8266/errorcodes/errorcodes_t.hpp>
#include <com/realtimelogic/json/esp8266/JEncoder.h>
#include <com/realtimelogic/json/esp8266/JVal.h>

namespace com {
namespace tholusi {
namespace esp8266 {

const int JSONRPC_PARSE_ERROR_NO        = -32700;
const int JSONRPC_INVALID_REQUEST_NO    = -32600;
const int JSONRPC_METHOD_NOT_FOUND_NO   = -32601;
const int JSONRPC_INVALID_PARAMS_NO     = -32602;
const int JSONRPC_INTERNAL_ERROR_NO     = -32603;

const char JSONRPC_PARSE_ERROR[]        = "Parse error";        // Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.
const char JSONRPC_INVALID_REQUEST[]    = "Invalid Request";    // The JSON sent is not a valid Request object.
const char JSONRPC_METHOD_NOT_FOUND[]   = "Method not found";   // The method does not exist / is not available.
const char JSONRPC_INVALID_PARAMS[]     = "Invalid params";     // Invalid method parameter(s).
const char JSONRPC_INTERNAL_ERROR[]     = "Internal error";     // nternal JSON-RPC error.

const char JSONRPC_ERR1[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -1,\"error\":\"unable to JSON encode message\"}}\n";
const char JSONRPC_ERR2[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -2,\"error\":\"JSON encoder error\"}}\n";
const char JSONRPC_ERR3[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -3,\"error\":\"unable to commit JSON object\"}}\n";
const char JSONRPC_ERR4[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -4,\"error\":\"BufPrint_vprintf\"}}\n";
const char JSONRPC_ERR5[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -5,\"error\":\"memory allocation failed (%s)\"}}\n";
const char JSONRPC_ERR6[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -6,\"error\":\"invalid memory block\"}}\n";
const char JSONRPC_ERR7[]               = "{\"jsonrpc\":\"2.0\",\"method\":\"jsonrpc\",\"params\":{\"code\": -5,\"error\":\"handler initialize failed (%d)\"}}\n";

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
    static void* ICACHE_FLASH_ATTR malloc(AllocatorIntf* o, size_t* size);
    /**
     * 
     * @param o
     * @param memblock
     * @param size
     * @return 
     */
    static void* ICACHE_FLASH_ATTR realloc(AllocatorIntf* o, void* memblock, size_t* size);
    /**
     * 
     * @param o
     * @param memblock
     */
    static void ICACHE_FLASH_ATTR free(AllocatorIntf* o, void* memblock);
    
public:
    /**
     * 
     */
    ICACHE_FLASH_ATTR JParserAllocator() : AllocatorIntf(malloc, realloc, free)
    {
    }
    /**
     * 
     * @param bufSize
     * @return 
     */
    int ICACHE_FLASH_ATTR init(int bufSize);
};

/******************************************************************************************************************
 * json_rpc_t
 *
 */
class json_rpc_t
{
public:
    /**
     */
    ICACHE_FLASH_ATTR json_rpc_t()
    {        
    }
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
     * @param c
     * @return 
     */
    int ICACHE_FLASH_ATTR parse(int c);
    /**
     * 
     */
    void ICACHE_FLASH_ATTR parseReset()
    {
        // get ready for next run
        m_ValFact->termFirstVal();        
    }
    /**
     * 
     * @return 
     */
    const char* ICACHE_FLASH_ATTR getMethod()
    {
        return m_Method;
    }
    /**
     * 
     * @return 
     */
    JVal* ICACHE_FLASH_ATTR getParams()
    {
        return m_Params;
    }
    /**
     * 
     * @return 
     */
    int32_t ICACHE_FLASH_ATTR getMsgId()
    {
        return m_MsgId;
    }
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
     * @param proc
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR warning(const char* proc, const char *fmt, ...);
    /**
     * 
     * @param proc
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR error(const char* proc, const char *fmt, ...);    
    /**
     * 
     * @param msgId
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR successReply(int32_t msgId, const char* fmt, ...);
    /**
     * 
     * @param code
     * @param msgId
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR errorReply(int16_t code, int32_t msgId, const char* fmt, ...);
    /**
     * 
     * @param code
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR errorReply(int16_t code, const char* fmt, ...);
    /**
     * 
     * @param method
     * @param system
     * @param fmt
     * @param ...
     * @return 
     */
    int ICACHE_FLASH_ATTR notification(const char* method, const char* system, const char* fmt, ...);
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR lock()
    {
        if(m_Lock == 0) {
            m_Lock = 1;
            return 0;
        }
        else {
            return -1;
        }
    }
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR unlock()
    {
        if(m_Lock == 0) {
            return -1;
        }
        else {
            m_Lock = 0;
            return 0;
        }        
    }
    /**
     * 
     * @return 
     */
    JEncoder* ICACHE_FLASH_ATTR getJEncoder()
    {
        if(m_Lock != 1) {
            return 0;
        }
        
        return m_Encoder;
    }
    /**
     * 
     * @return 
     */
    JErr* ICACHE_FLASH_ATTR getJErr()
    {
        if(m_Lock != 1) {
            return 0;
        }
        
        return m_Err;
    }
    
private:
    /**
     * 
     * @return 
     */
    int ICACHE_FLASH_ATTR _parse();
    /**
     * 
     * @param method
     * @param system
     * @param fmt
     * @param varg
     * @return 
     */
    int ICACHE_FLASH_ATTR _notification(const char* method, const char* system, const char* fmt, va_list varg);
    /**
     * 
     * @param code
     * @param msgId
     * @param msgIdNull
     * @param fmt
     * @param argList
     * @return 
     */
    int ICACHE_FLASH_ATTR _errorReply(int16_t code, int32_t msgId, bool msgIdNull, const char* fmt, va_list varg);
    
    const char*         m_Method;
    JVal*               m_Params;
    int32_t             m_MsgId;
    
    char                m_MemberName[20];
    JParser*            m_Parser;
    JParserValFact*     m_ValFact;
    JParserAllocator    m_Allocator;

    BufPrint            m_Out;
    JErr*               m_Err;
    JEncoder*           m_Encoder;

    char*               m_EncoderBuffer;
    char*               m_NotifBuffer;
    int                 m_NotifBufferSize;
    
    int                 m_Lock;
    
    // no copy and no = operator
    json_rpc_t(const json_rpc_t&);
    json_rpc_t &operator=(const json_rpc_t&);    
};

}
}
}

#endif	/* COM_THOLUSI_ESP8266_JSON_RPC_T_HPP */

