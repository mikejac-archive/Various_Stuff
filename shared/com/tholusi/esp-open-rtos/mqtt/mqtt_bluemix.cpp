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

#if defined(REMOTE_BUILD)
    #include <com/tholusi/esp-open-rtos/cplusplus/cplusplus.hpp>
    #include <com/tholusi/esp-open-rtos/mqtt/mqtt_bluemix.hpp>
#else
    #include <cplusplus/cplusplus.hpp>
    #include <mqtt/mqtt_bluemix.hpp>
#endif

#include <stdio.h>

using namespace esp_open_rtos::mqtt;

#define DTXT(...)       printf(__VA_ARGS__)

/******************************************************************************************************************
 * JParserAllocator
 *
 */
int JParserAllocator::init(int bufSize)
{
    AllocatorIntf_constructor(this, malloc, realloc, free);
    
    buf = new uint8_t[bufSize];
    
    m_BufSize = bufSize;
    
    DTXT("JParserAllocator::init(): m_BufSize = %d\n", m_BufSize);
    
    return (buf == NULL) ? -1 : 0;
}
/**
 * 
 * @param super
 * @param size
 * @return 
 */
void* JParserAllocator::malloc(AllocatorIntf* super, size_t* size)
{
    JParserAllocator* o = (JParserAllocator*) super;
    
    DTXT("JParserAllocator::malloc(): size = %d; m_BufSize = %d\n", *size, o->m_BufSize);
    
    return *size < o->m_BufSize ? o->buf : 0;
}
/**
 * Called when the one and only string buffer must grow 
 * @param super
 * @param memblock
 * @param size
 * @return 
 */
void* JParserAllocator::realloc(AllocatorIntf* super, void* memblock, size_t* size)
{
    JParserAllocator* o = (JParserAllocator*) super;
    
    if(memblock != o->buf) {
        while(true) {
            ;
        }
    }
    
    DTXT("JParserAllocator::realloc(): size = %d; m_BufSize = %d\n", *size, o->m_BufSize);
    
    return *size < o->m_BufSize ? o->buf : 0;
}
/**
 * Called when JParser terminates
 * @param super
 * @param memblock
 */
void JParserAllocator::free(AllocatorIntf* super, void* memblock)
{
    DTXT("JParserAllocator::free()\n");
   // Do nothing
}

/**
 * 
 * @param o
 * @param sizeRequired
 * @return 
 */
static int _BufPrint_flush(BufPrint* o, int sizeRequired)
{
    BufPrint_getBuf(o)[o->cursor] = '\0';
    
    //printf("%s\n", BufPrint_getBuf(o));

    BufPrint_erase(o);      // data flushed
   
    return 0;    // ok
}

/******************************************************************************************************************
 * bluemix_t
 *
 */

/**
 * 
 * @param root_topic
 * @param nodename
 * @param platform_id
 * @return 
 */
int bluemix_t::bmixInit(int encoderBufSize, int decoderBufSize)
{
    // prepare JSON encoder
    m_EncoderBuffer = new char[encoderBufSize];
    
    if(m_EncoderBuffer == 0) {
        DTXT("bluemix_t::bmixInit(): EncoderBuffer\n");
        return -1;
    }
    
    m_Encoder = new JEncoder;

    if(m_Encoder == 0) {
        DTXT("bluemix_t::bmixInit(): JEncoder\n");
        
        delete[] m_EncoderBuffer;
        
        return -2;
    }
    
    m_Err = new JErr;

    if(m_Err == 0) {
        DTXT("bluemix_t::bmixInit(): JErr\n");
        
        delete   m_Encoder;
        delete[] m_EncoderBuffer;
        
        return -3;
    }
    
    m_Out.setBuf(m_EncoderBuffer, encoderBufSize);
    m_Out.setFlush(_BufPrint_flush);

    m_Encoder->init(m_Err, &m_Out);
    
    // prepare JSON decoder
    if(m_Allocator.init(decoderBufSize) != 0) {
        DTXT("bluemix_t::bmixInit(): m_Allocator.init\n");
        
        delete   m_Encoder;
        delete[] m_EncoderBuffer;
        delete   m_Err;
        
        return -4;
    }
    
    m_Parser = new JParser;

    if(m_Parser == 0) {
        DTXT("bluemix_t::bmixInit(): m_Parser\n");
        
        delete   m_Encoder;
        delete[] m_EncoderBuffer;
        delete   m_Err;
        
        return -5;
    }
    
    m_ValFact = new JParserValFact;

    if(m_ValFact == 0) {
        DTXT("bluemix_t::bmixInit(): m_ValFact\n");
        
        delete   m_Encoder;
        delete[] m_EncoderBuffer;
        delete   m_Err;
        delete   m_Parser;
        
        return -6;
    }
    
    m_ValFact->init(AllocatorIntf::getDefault(), AllocatorIntf::getDefault());

    m_Parser->init(m_ValFact, m_MemberName, sizeof(m_MemberName), &m_Allocator);
    
    return 0;
}
/**
 * 
 * @param buf
 * @param size
 * @return 
 */
JVal* bluemix_t::bmixDecoderBegin(void* buf, uint32_t size)
{
    m_ValFact->termFirstVal();
    
    int ret = m_Parser->parse((const U8*)buf, size);

    if(ret < 0) {           // parse error
        DTXT("bluemix_t::bmixDecoderBegin(): parse error\n");
        m_Parser->init(m_ValFact, m_MemberName, sizeof(m_MemberName), &m_Allocator);
        
        return NULL;
    }
    else if(ret > 0) {      // a complete JSON object
        DTXT("bluemix_t::bmixDecoderBegin(): JSON data complete\n");

        /*
            {
               "d" : {
                  "_type" : "clock",
                  "seconds" : 1440688704
               }
            }
        */

        JVal* jv_root = m_ValFact->getFirstVal();

        /******************************************************************************************************************
         * validate root object
         *
         */    
        const char* root = jv_root->getName();

        if(root != 0) {                             // root should not have a name - something is wrong
            DTXT("bluemix_t::bmixDecoderBegin(): root != 0; root = %s\n", root);
            return NULL;
        }
        else if(jv_root->getType() != JVType_Object) {
            DTXT("bluemix_t::bmixDecoderBegin(): jv_root->getType() != JVType_Object\n");
            return NULL;
        }

        /******************************************************************************************************************
         * validate 'd' object
         *
         */    
        // get the json child object
        JVal* jv_d = jv_root->getJ(0);

        const char* d = jv_d->getName();

        if(d == 0 || strcmp("d", d) != 0) {
            DTXT("bluemix_t::bmixDecoderBegin(): d == 0 || strcmp(\"d\", d) != 0\n");
            return NULL;
        }
        else if(jv_d->getType() != JVType_Object) {
            DTXT("bluemix_t::bmixDecoderBegin(): jv_d->getType() != JVType_Object\n");
            return NULL;
        }

        m_Err->reset();
        
        // get the 'd' child object
        m_Jv_d = jv_d->getJ(0);
        
        return  m_Jv_d;
    }
    else {                  // incomplete
        DTXT("bluemix_t::bmixDecoderBegin(): incomplete\n");
        
        return NULL;
    }
}
/**
 * 
 * @return 
 */
int bluemix_t::bmixDecoderEnd()
{
    // get ready for next run
    //m_ValFact->termFirstVal();

    return 0;
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixGetString(const char* name, const char** value)
{
    JVal* jv_dd = bmixFind(name);
    
    if(jv_dd != 0 && jv_dd->getType() == JVType_String) {
        *value = jv_dd->getString(m_Err);
        
        // check for errors
        if(m_Err->isError()) {        
            DTXT("bluemix_t::bmixGetString(): JSON decoder error; %s\n", m_Err->msg);
            return -2;
        }
        else {
            return 0;
        }
    }
    else {
        // not found
        return -1;
    }
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
/*int bluemix_t::bmixGetInt(const char* name, int32_t& value)
{
    JVal* jv_dd = bmixFind(name);
    
    if(jv_dd != 0 && jv_dd->getType() == JVType_Int) {
        value = jv_dd->getInt(m_Err);
        
        // check for errors
        if(m_Err->isError()) {        
            DTXT("bluemix_t::bmixGetInt(): JSON decoder error; %s\n", m_Err->msg);
            return -2;
        }
        else {
            return 0;
        }
    }
    else {
        // not found
        return -1;
    }
}*/
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixGetInt(const char* name, int& value)
{
    JVal* jv_dd = bmixFind(name);
    
    if(jv_dd != 0 && jv_dd->getType() == JVType_Int) {
        value = jv_dd->getInt(m_Err);
        
        // check for errors
        if(m_Err->isError()) {        
            DTXT("bluemix_t::bmixGetInt(): JSON decoder error; %s\n", m_Err->msg);
            return -2;
        }
        else {
            return 0;
        }
    }
    else {
        // not found
        return -1;
    }
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixGetLong(const char* name, int64_t& value)
{
    JVal* jv_dd = bmixFind(name);
    
    if(jv_dd != 0 && (jv_dd->getType() == JVType_Int || jv_dd->getType() == JVType_Long)) {
        value = jv_dd->getLong(m_Err);
        
        // check for errors
        if(m_Err->isError()) {        
            DTXT("bluemix_t::bmixGetLong(): JSON decoder error; %s\n", m_Err->msg);
            return -2;
        }
        else {
            return 0;
        }
    }
    else {
        // not found
        return -1;
    }
}
/**
 * 
 * @param name
 * @return 
 */
JVal* bluemix_t::bmixFind(const char* name)
{
    JVal* jv_dd = m_Jv_d;
    
    while(jv_dd != 0) {
        const char* _name = jv_dd->getName();

        if(_name != 0 && strcmp(_name, name) == 0) {
            return jv_dd;
        }
        
        jv_dd = jv_dd->getNextElem();
    }

    DTXT("bluemix_t::bmixFind(): not found; %s\n", name);
    
    // didn't find it
    return 0;
}
/**
 * 
 * @return 
 */
int bluemix_t::bmixEncoderBegin()
{
    m_Err->reset();
    
    m_Encoder->beginObject();   // root object
    
    m_Encoder->setName("d");
    m_Encoder->beginObject();   // 'd' object
    
    return 0;
}
/**
 * 
 * @return 
 */
char* bluemix_t::bmixEncoderEnd()
{
    m_Encoder->endObject();     // 'd' object
    m_Encoder->endObject();     // root object
    
    if(m_Err->isError()) {
        DTXT("bluemix_t::bmixEncoderEnd: error; %s\n", m_Err->msg);
        
        return 0;
    }
    else if(m_Encoder->commit()) {
        DTXT("bluemix_t::bmixEncoderEnd(): error; m_Encoder commit\n");
        
        return 0;
    }
    
    return m_Out.getBuf();
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixAddString(const char* name, const char* value)
{
    m_Encoder->setName(name);
    m_Encoder->setString(value);
    
    return 0;
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixAddInt(const char* name, int32_t value)
{
    m_Encoder->setName(name);
    m_Encoder->setInt(value);
    
    return 0;
}
/**
 * 
 * @param name
 * @param value
 * @return 
 */
int bluemix_t::bmixAddLong(const char* name, int64_t value)
{
    m_Encoder->setName(name);
    m_Encoder->setLong(value);
    
    return 0;
}
