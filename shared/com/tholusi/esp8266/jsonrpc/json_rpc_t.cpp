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
#include <com/tholusi/esp8266/jsonrpc/json_rpc_t.hpp>
#include <com/tholusi/esp8266/debug/debug_t.hpp>

using namespace com::tholusi::esp8266;

/******************************************************************************************************************
 * functions
 *
 */

/**
 * 
 * @param pad
 */
void ICACHE_FLASH_ATTR printPadChars(int pad)
{
    for(int i = 0 ; i < pad; i++) {
        os_printf(" ");
    }
}
/**
 * 
 * @param v
 * @param recCntr
 */
void ICACHE_FLASH_ATTR printJVal(JVal* v, int recCntr)
{
    static int x = 1;
    
   for( ; v ; v = JVal_getNextElem(v), ++x )
   {
      const char* m;
      printPadChars(recCntr * 4);
      m = v->getName();
      if(m)
          os_printf("[%d, %d] %-10s : ", x, recCntr, m); /* v is a struct member */
      else
          os_printf("[%d, %d] m == 0\n", x, recCntr);
      
      /* else: v is an array member */
      
      switch(v->getType())
      {
         case JVType_String:
            os_printf("%s\n", v->getString(0));
            break;
         case JVType_Double:
            os_printf("%g\n", v->getDouble(0));
            break;
         case JVType_Int:
            os_printf("%d\n", (int)v->getInt(0));
            break;
         case JVType_Long:
            os_printf("%lld\n", (long long)v->getLong(0));
            break;
         case JVType_Boolean:
            os_printf("%s\n", v->getBoolean(0) ? "true" : "false");
            break;
         case JVType_Null:
            os_printf("NULL\n");
            break;

         case JVType_Object:
         case JVType_Array:
            if(m)
               os_printf(" ++\n");
            printJVal(v->getJ(0), recCntr+1);
            break;

         default:
            os_printf("  ##\n");
            baAssert(0);
            break;
      }
   }
}
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
/**
 * BufPrint "flush" callback function used indirectly by JEncoder. The function is called when the buffer is full or if committed.
 * 
 * @param o
 * @param sizeRequired
 * @return 
 */
static int ICACHE_FLASH_ATTR _BufPrint_flush(BufPrint* o, int sizeRequired)
{
    BufPrint_getBuf(o)[o->cursor] = '\0';
    
    os_printf("%s\n", BufPrint_getBuf(o));

    BufPrint_erase(o);      // data flushed
   
    return 0;    // ok
}

/******************************************************************************************************************
 * JParserAllocator
 *
 */
int ICACHE_FLASH_ATTR JParserAllocator::init(int bufSize)
{
    AllocatorIntf_constructor(this, malloc, realloc, free);
    
    buf = (uint8_t*) os_malloc(bufSize);
    
    m_BufSize = bufSize;
    
    return (buf == NULL) ? -1 : 0;
}
/**
 * 
 * @param super
 * @param size
 * @return 
 */
void* ICACHE_FLASH_ATTR JParserAllocator::malloc(AllocatorIntf* super, size_t* size)
{
    JParserAllocator* o = (JParserAllocator*) super;
    
    return *size < o->m_BufSize ? o->buf : 0;
}
/**
 * Called when the one and only string buffer must grow 
 * @param super
 * @param memblock
 * @param size
 * @return 
 */
void* ICACHE_FLASH_ATTR JParserAllocator::realloc(AllocatorIntf* super, void* memblock, size_t* size)
{
    JParserAllocator* o = (JParserAllocator*) super;
    
    if(memblock != o->buf) {
        os_printf(JSONRPC_ERR6);
        
        while(true) {
            ;
        }
    }
    
    return *size < o->m_BufSize ? o->buf : 0;
}
/**
 * Called when JParser terminates
 * @param super
 * @param memblock
 */
void ICACHE_FLASH_ATTR JParserAllocator::free(AllocatorIntf* super, void* memblock)
{
   // Do nothing
}

/******************************************************************************************************************
 * json_rpc_t
 *
 */

/**
 * 
 * @param encoderBufSize
 * @param decoderBufSize
 * @param notifBufSize
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::init(int encoderBufSize, int decoderBufSize, int notifBufSize)
{
    m_Allocator.init(decoderBufSize);
    m_Out.init();
    
    m_Encoder = new JEncoder;

    if(m_Encoder == 0) {
        os_printf(JSONRPC_ERR5, "JEncoder");
        return -1;
    }
    
    m_ValFact = new JParserValFact;

    if(m_ValFact == 0) {
        os_printf(JSONRPC_ERR5, "JParserValFact");
        return -2;
    }

    m_Parser = new JParser;

    if(m_Parser == 0) {
        os_printf(JSONRPC_ERR5, "JParser");
        return -3;
    }

    m_Err = new JErr;

    if(m_Err == 0) {
        os_printf(JSONRPC_ERR5, "JErr");
        return -5;
    }
    
    m_EncoderBuffer = (char*) os_malloc(encoderBufSize);
    
    if(m_EncoderBuffer == NULL) {
        os_printf(JSONRPC_ERR5, "m_EncoderBuffer");
        return -6;
    }
    
    m_NotifBuffer = (char*) os_malloc(notifBufSize);
    
    if(m_NotifBuffer == NULL) {
        os_printf(JSONRPC_ERR5, "m_NotifBuffer");
        return -7;
    }

    m_NotifBufferSize = notifBufSize;
    
    m_Out.setBuf(m_EncoderBuffer, encoderBufSize);
    m_Out.setFlush(_BufPrint_flush);
    
    m_ValFact->init(AllocatorIntf::getDefault(), AllocatorIntf::getDefault());

    m_Parser->init(m_ValFact, m_MemberName, sizeof(m_MemberName), &m_Allocator);
    
    m_Encoder->init(m_Err, &m_Out);
    
    m_Lock = 0;
    return 0;
}
/**
 * 
 * @param c
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::parse(int c)
{
    int ret = m_Parser->parse((U8*) &c, 1);

    if(ret < 0) {          // parse error
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);

        m_Parser->init(m_ValFact, m_MemberName, sizeof(m_MemberName), &m_Allocator);     // reset parser
        
        ret = EJSON_PARSE_ERR;
    }
    else if(ret > 0) {     // a complete JSON object
        ret = _parse();

        // get ready for next run
        //m_ValFact->termFirstVal();
    }
    else {
        ret = EJSON_PARSE_INCOMPLETE;    // incomplete
    }
    
    return ret;
}
/**
 * 
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::_parse()
{
    int ret = EOK;
    
    /******************************************************************************************************************
     * validate root object
     *
     * --> {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3}
     * <-- {"jsonrpc": "2.0", "result": 19, "id": 3}
     */    
    JVal* jv_root = m_ValFact->getFirstVal();
    printJVal(m_ValFact->getFirstVal(), 0);


    const char* root = jv_root->getName();
    
    if(root != 0) {                             // root should not have a name - something is wrong
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_1;
    }
    else if(jv_root->getType() != JVType_Object) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_2;
    }
    
    // get the json rpc child object
    JVal* jv_jsonrpc = jv_root->getJ(0);
    
    /******************************************************************************************************************
     * validate JSON-RPC field
     *
     */    
    const char* json_rpc = jv_jsonrpc->getName();
    
    if(json_rpc == 0 || os_strcmp("jsonrpc", json_rpc) != 0) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_3;
    }
    else if(jv_jsonrpc->getType() != JVType_String) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_4;
    }    
    else if(os_strcmp("2.0", jv_jsonrpc->getString(0)) != 0) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_5;
    }
    else {
        DTXT("json_rpc_t::_parse(): jsonrpc ok\n");
    }
    
    /******************************************************************************************************************
     * process method
     *
     */
    jv_jsonrpc = jv_jsonrpc->getNextElem();
    
    const char* method = jv_jsonrpc->getName();
    
    if(method == 0 || os_strcmp("method", method) != 0) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_6;
    }
    else if(jv_jsonrpc->getType() != JVType_String) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_7;
    }
    else {
        m_Method = jv_jsonrpc->getString(0);
        ret      = EJSON_METHOD;
        
        DTXT("json_rpc_t::_parse(): method ok; method = [%s]\n", m_Method);
    }
    
    /******************************************************************************************************************
     * process parameters
     *
     */
    jv_jsonrpc = jv_jsonrpc->getNextElem();
    
    const char* params = jv_jsonrpc->getName();
    
    if(params == 0 || os_strcmp("params", params) != 0) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_8;
    }
    else if(jv_jsonrpc->getType() != JVType_Object) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_9;
    }
    else {
        // get the params child object
        m_Params = jv_jsonrpc->getJ(0);
        
        //printJVal(m_Params, 1);
        DTXT("json_rpc_t::_parse(): params ok\n");
    }
    
    /******************************************************************************************************************
     * process message id
     *
     */
    jv_jsonrpc = jv_jsonrpc->getNextElem();
    
    const char* id = jv_jsonrpc->getName();
    
    if(id == 0 || os_strcmp("id", id) != 0) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_10;
    }
    else if(jv_jsonrpc->getType() != JVType_Int) {
        errorReply(JSONRPC_PARSE_ERROR_NO, JSONRPC_PARSE_ERROR);
        return EJSON_PARSE_11;
    }
    else {
        m_MsgId = jv_jsonrpc->getInt(0);
        DTXT("json_rpc_t::_parse(): id ok; id = [%d]\n", m_MsgId);
    }
    
    return ret;
}
/**
 * 
 * @param proc
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::info(const char* proc, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _notification("info", proc, fmt, varg);
    
    va_end(varg);
    
    return ret;    
}
/**
 * 
 * @param proc
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::warning(const char* proc, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _notification("warning", proc, fmt, varg);
    
    va_end(varg);
    
    return ret;    
}
/**
 * 
 * @param proc
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::error(const char* proc, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _notification("error", proc, fmt, varg);
    
    va_end(varg);
    
    return ret;    
}

/**
 * 
 * @param method
 * @param system
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::notification(const char* method, const char* system, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _notification(method, system, fmt, varg);
    
    va_end(varg);
    
    return ret;        
}
/**
 * 
 * @param method
 * @param system
 * @param fmt
 * @param varg
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::_notification(const char* method, const char* system, const char* fmt, va_list varg)
{
    /******************************************************************************************************************
     * format message text
     *
     */
    BufPrint bufPrint;

    BufPrint_constructor(&bufPrint, 0, sprintfIsFull);

    bufPrint.buf     = m_NotifBuffer;
    bufPrint.bufSize = (m_NotifBufferSize - 1);     // -1 -> the string terminator
    bufPrint.cursor  = 0;                           // start position is beginning of buf

    int ret = BufPrint_vprintf(&bufPrint, fmt, varg);

    if(ret >= 0) {
        m_NotifBuffer[bufPrint.cursor] = 0;         // zero terminate string
        ret                            = bufPrint.cursor;
        
        /******************************************************************************************************************
         * build JSON RPC notification object
         *
         */
        if(m_Encoder->set("{ss{ss}}",
                        "jsonrpc", "2.0",
                        "method",  method,
                        "params",
                            "sys", system,
                            "msg", m_NotifBuffer) != 0) {
            os_printf(JSONRPC_ERR1);
        }

        if(m_Err->isError()) {
            os_printf(JSONRPC_ERR2);            
        }
        else if(m_Encoder->commit()) {            // this will activate _BufPrint_flush() which will then output the JSON data
            os_printf(JSONRPC_ERR3);
        }
    }
    else {
        os_printf(JSONRPC_ERR4);
    }
    
    return ret;    
}
/**
 * 
 * @param msgId
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::successReply(int32_t msgId, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    /******************************************************************************************************************
     * format message text
     *
     */
    BufPrint bufPrint;

    BufPrint_constructor(&bufPrint, 0, sprintfIsFull);

    bufPrint.buf     = m_NotifBuffer;
    bufPrint.bufSize = (m_NotifBufferSize - 1);     // -1 -> the string terminator
    bufPrint.cursor  = 0;                           // start position is beginning of buf

    int ret = BufPrint_vprintf(&bufPrint, fmt, varg);

    if(ret >= 0) {
        m_NotifBuffer[bufPrint.cursor] = 0;         // zero terminate string
    
        /******************************************************************************************************************
         * build JSON-RPC object
         *
         * {"jsonrpc": "2.0", "result": 19, "id": 1}
         */
        if(m_Encoder->set("{ssd}",
                         "jsonrpc", "2.0",
                         "result", m_NotifBuffer,
                         "id", msgId) != 0) {
            os_printf(JSONRPC_ERR1);
        }

        if(m_Err->isError()) {
            os_printf(JSONRPC_ERR2);            
        }
        else if(m_Encoder->commit()) {            // this will activate _BufPrint_flush() which will then output the JSON data
            os_printf(JSONRPC_ERR3);
        }
    }
    else {
        os_printf(JSONRPC_ERR4);
    }
    
    va_end(varg);
    
    return 0;
}
/**
 * 
 * @param code
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::errorReply(int16_t code, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _errorReply(code, 0, true, fmt, varg);
    
    va_end(varg);
    
    return ret;    
}
/**
 * 
 * @param code
 * @param msgId
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::errorReply(int16_t code, int32_t msgId, const char* fmt, ...)
{
    va_list varg;
    
    va_start(varg, fmt);
    
    int ret = _errorReply(code, msgId, false, fmt, varg);
    
    va_end(varg);
    
    return ret;        
}
/**
 * 
 * @param code
 * @param msgId
 * @param msgIdNull
 * @param fmt
 * @param ...
 * @return 
 */
int ICACHE_FLASH_ATTR json_rpc_t::_errorReply(int16_t code, int32_t msgId, bool msgIdNull, const char* fmt, va_list varg)
{
    /******************************************************************************************************************
     * format message text
     *
     */
    BufPrint bufPrint;

    BufPrint_constructor(&bufPrint, 0, sprintfIsFull);

    bufPrint.buf     = m_NotifBuffer;
    bufPrint.bufSize = (m_NotifBufferSize - 1);     // -1 -> the string terminator
    bufPrint.cursor  = 0;                           // start position is beginning of buf

    int ret = BufPrint_vprintf(&bufPrint, fmt, varg);

    if(ret >= 0) {
        m_NotifBuffer[bufPrint.cursor] = 0;         // zero terminate string
    
        /******************************************************************************************************************
         * build JSON-RPC object
         *
         * {"jsonrpc": "2.0", "error": {"code": -32601, "message": "Method not found"}, "id": "1"}
         * {"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}
         */
        if(msgIdNull == false) {                    // id has a value
            if(m_Encoder->set("{s{ds}d}",
                             "jsonrpc", "2.0",
                             "error",
                                 "code", code,
                                 "message", m_NotifBuffer,
                             "id", msgId) != 0) {
                os_printf(JSONRPC_ERR1);
            }
        }
        else {                                      // id set to 'null'
            if(m_Encoder->set("{s{ds}n}",
                             "jsonrpc", "2.0",
                             "error",
                                 "code", code,
                                 "message", m_NotifBuffer,
                             "id") != 0) {
                os_printf(JSONRPC_ERR1);
            }        
        }

        if(m_Err->isError()) {
            os_printf(JSONRPC_ERR2);            
        }
        else if(m_Encoder->commit()) {            // this will activate _BufPrint_flush() which will then output the JSON data
            os_printf(JSONRPC_ERR3);
        }
    }
    else {
        os_printf(JSONRPC_ERR4);
    }
    
    return 0;
}
