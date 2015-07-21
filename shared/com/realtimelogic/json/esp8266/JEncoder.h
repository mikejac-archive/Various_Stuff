/*
 *     ____             _________                __                _     
 *    / __ \___  ____ _/ /_  __(_)___ ___  ___  / /   ____  ____ _(_)____
 *   / /_/ / _ \/ __ `/ / / / / / __ `__ \/ _ \/ /   / __ \/ __ `/ / ___/
 *  / _, _/  __/ /_/ / / / / / / / / / / /  __/ /___/ /_/ / /_/ / / /__  
 * /_/ |_|\___/\__,_/_/ /_/ /_/_/ /_/ /_/\___/_____/\____/\__, /_/\___/  
 *                                                       /____/          
 *
 *                  Barracuda Application Server
 *
 ****************************************************************************
 *			      HEADER
 *
 *   $Id: JEncoder.h 3465 2014-08-21 22:58:04Z wini $
 *
 *   COPYRIGHT:  Real Time Logic LLC, 2006-2014
 *
 *   This software is copyrighted by and is the sole property of Real
 *   Time Logic LLC.  All rights, title, ownership, or other interests in
 *   the software remain the property of Real Time Logic LLC.  This
 *   software may only be used in accordance with the terms and
 *   conditions stipulated in the corresponding license agreement under
 *   which the software has been supplied.  Any unauthorized use,
 *   duplication, transmission, distribution, or disclosure of this
 *   software is expressly forbidden.
 *                                                                        
 *   This Copyright notice may not be removed or modified without prior
 *   written consent of Real Time Logic LLC.
 *                                                                         
 *   Real Time Logic LLC. reserves the right to modify this software
 *   without notice.
 *
 *               http://realtimelogic.com
 ****************************************************************************
 *
 */

#ifndef __JEncoder_h
#define __JEncoder_h

#include "JVal.h"
#include "BufPrint.h"

/** @addtogroup JSONRef
@{
*/
 

/** The JEncoder can serialize a JSON JVAL syntax tree to the JSON
    text format. The JEncoder can also be used for assembling JSON
    text from calling the primitive methods in this class.
 */
typedef struct JEncoder
{
#ifdef __cplusplus
      /** Create an instance of the JEncoder class.
       */
      ICACHE_FLASH_ATTR JEncoder(JErr* err, BufPrint* out);
      ICACHE_FLASH_ATTR JEncoder() {}

      ICACHE_FLASH_ATTR ~JEncoder();

      void ICACHE_FLASH_ATTR init(JErr* err, BufPrint* out);
      
      /** Format an integer value.
       */
      int ICACHE_FLASH_ATTR setInt(S32 val);

      /** Format a long integer value.
       */
      int ICACHE_FLASH_ATTR setLong(S64 val);
#ifndef NO_DOUBLE

      /** Format a double value.
       */
      int ICACHE_FLASH_ATTR setDouble(double val);
#endif

      /** Format a string value. The method is internally using 
          BufPrint::jsonString.
       */
      int ICACHE_FLASH_ATTR setString(const char* val);

      /** Converts (according to format 'fmt') and writes to stream.
          The method is internally using BufPrint::vprintf, which
          means that you can use the specialized format flags such as
          %%j.
      */
      int ICACHE_FLASH_ATTR fmtString(const char* fmt,...);

      /** Equivalent to fmtString with variable argument list replaced
          by argList.
       */ 
      int ICACHE_FLASH_ATTR vFmtString(const char* fmt,va_list argList);

      /** Format a boolean value.
       */
      int ICACHE_FLASH_ATTR setBoolean(bool val);

      /** Set a JSON null value.
       */
      int ICACHE_FLASH_ATTR setNull();

      /** Format a node or a tree of JVal nodes.
       */
      int ICACHE_FLASH_ATTR setJV(struct JVal* val, bool iterateNext=false);

      /** Encode/serialize C structs/data to JSON using formatted output.

      The set method works in a similar fashion to the ANSII C
      function printf; thus, it performs formatted output conversion.

      The method internally uses the 'setXXX' methods in this class
      for setting the actual value(s), where XXX is setInt, setLong, etc.. The
      following table shows the relationship between the format flags and
      the JSON value:

      <table>
      <tr><th>JSON type</th><th>Format flag</th><th>C type</th>
      <th>Method</th></tr>
      <tr><td>Number</td><td> d </td><td>S32</td>
      <td>JVal::setInt</td></tr>
      <tr><td>Number</td><td> l </td><td>S64</td>
      <td>JVal::setLong</td></tr>
      <tr><td>Number</td><td> f </td><td>double</td>
      <td>JVal::setDouble</td></tr>
      <tr><td>boolean</td><td> b </td><td>BaBool</td>
      <td>JVal::setBoolean</td></tr>
      <tr><td>string</td><td> s </td><td>const char*</td>
      <td>JVal::setString</td></tr>
      <tr><td>Any type</td><td> J </td><td>JVal*</td>
      <td>All methods</td></tr>
      <tr><td>Start object</td><td> { </td><td>n/a</td>
      <td>JVal::beginObject</td></tr>
      <tr><td>End object</td><td> } </td><td>n/a</td>
      <td>JVal::endObject</td></tr>
      <tr><td>Start array</td><td> [ </td><td>n/a</td>
      <td>JVal::beginArray</td></tr>
      <tr><td>End array</td><td> ] </td><td>n/a</td>
      <td>JVal::endArray</td></tr>
      <tr><td>JVal[size]</td><td> A </td><td>Array of JVal</td>
      <td>All setXXX methods</td></tr>
      </table>

      \sa JE_MEMBER
      \sa JVal::get
      \sa JDecoder::get
      */
      int /*ICACHE_FLASH_ATTR*/ set(const char* fmt, ...);

      /** Set the member name before calling any of the set<value> members
        if formatting an object.
        \param name the object member name.
        \sa beginObject
       */
      int ICACHE_FLASH_ATTR setName(const char* name);

      /** Begin object formatting.

         All calls to the the set<value> members must be proceeded by
         a call to JEncoder::name after you call
         beginObject. This condition is terminated if you call
         JEncoder::endObject or call
         JEncoder::beginArray.
      */

      /** Begin object formatting
       */
      int ICACHE_FLASH_ATTR beginObject();

      /** End object formatting.
       */
      int ICACHE_FLASH_ATTR endObject();

      /** Begin array formatting.
       */
      int ICACHE_FLASH_ATTR beginArray();

      /** End array formatting.
       */
      int ICACHE_FLASH_ATTR endArray();

      /** Returns the JErr object.
       */
      JErr* ICACHE_FLASH_ATTR getErr();

      /** Flush JEncoder data to the output stream.
       */
      int ICACHE_FLASH_ATTR flush();

      /** Calls flush and enables the construction of a new object
       */
      int ICACHE_FLASH_ATTR commit();

      /** Fetch the internal BufPrint object.
       */
      BufPrint* ICACHE_FLASH_ATTR getBufPrint();

#endif
      JErr* err;
      struct
      {
            S32 level;
            U8 data[64];
      }objectStack;
      BufPrint* out;
      BaBool objectMember;
      BaBool startNewObj;
} JEncoder;
#ifdef __cplusplus
extern "C" {
#endif
BA_API void ICACHE_FLASH_ATTR JEncoder_constructor(JEncoder* o, JErr* err, BufPrint* out);
#define JEncoder_destructor(o) JEncoder_flush(o)
BA_API int ICACHE_FLASH_ATTR JEncoder_flush(JEncoder* o);
BA_API int ICACHE_FLASH_ATTR JEncoder_commit(JEncoder* o);
#define JEncoder_getErr(o) (o)->err
BA_API int ICACHE_FLASH_ATTR JEncoder_setInt(JEncoder* o, S32 val);
BA_API int ICACHE_FLASH_ATTR JEncoder_setLong(JEncoder* o, S64 val);
#ifdef NO_DOUBLE
#define ICACHE_FLASH_ATTR JEncoder_setDouble JEncoder_setInt
#else
BA_API int ICACHE_FLASH_ATTR JEncoder_setDouble(JEncoder* o, double val);
#endif

BA_API int ICACHE_FLASH_ATTR JEncoder_fmtString(JEncoder* o, const char* fmt,...);
BA_API int ICACHE_FLASH_ATTR JEncoder_vFmtString(
   JEncoder* o, const char* fmt,va_list argList);
BA_API int ICACHE_FLASH_ATTR JEncoder_setString(JEncoder* o, const char* val);
BA_API int ICACHE_FLASH_ATTR JEncoder_setBoolean(JEncoder* o, BaBool val);
BA_API int ICACHE_FLASH_ATTR JEncoder_setNull(JEncoder* o);
BA_API int ICACHE_FLASH_ATTR JEncoder_setJV(
   JEncoder* o, struct JVal* val, BaBool iterateNext);
BA_API int ICACHE_FLASH_ATTR JEncoder_vSetJV(
   JEncoder* o,const char** fmt,va_list* argList);
BA_API int ICACHE_FLASH_ATTR JEncoder_set(JEncoder* o, const char* fmt, ...);
BA_API int ICACHE_FLASH_ATTR JEncoder_setName(JEncoder* o, const char* name);
BA_API int ICACHE_FLASH_ATTR JEncoder_beginObject(JEncoder* o);
BA_API int ICACHE_FLASH_ATTR JEncoder_endObject(JEncoder* o);
BA_API int ICACHE_FLASH_ATTR JEncoder_beginArray(JEncoder* o);
BA_API int ICACHE_FLASH_ATTR JEncoder_endArray(JEncoder* o);
#define JEncoder_getBufPrint(o) (o)->out
#ifdef __cplusplus
}
inline ICACHE_FLASH_ATTR JEncoder::JEncoder(JErr* err, BufPrint* out) {
   JEncoder_constructor(this,err, out); }
inline ICACHE_FLASH_ATTR JEncoder::~JEncoder() {
   JEncoder_destructor(this); }
inline void ICACHE_FLASH_ATTR JEncoder::init(JErr* err, BufPrint* out) {
   JEncoder_constructor(this,err, out); }
inline int ICACHE_FLASH_ATTR JEncoder::setInt(S32 val) {
   return  JEncoder_setInt(this, val); }
inline int ICACHE_FLASH_ATTR JEncoder::setLong(S64 val) {
   return  JEncoder_setLong(this, val); }
#ifndef NO_DOUBLE
inline int ICACHE_FLASH_ATTR JEncoder::setDouble(double val) {
   return  JEncoder_setDouble(this, val); }
#endif
inline int ICACHE_FLASH_ATTR JEncoder::fmtString(const char* fmt,...) {
   int retv; va_list argList;
   va_start(argList, fmt); 
   retv = JEncoder_fmtString(this, fmt, argList);
   va_end(argList);
   return retv;
}
inline int ICACHE_FLASH_ATTR JEncoder::vFmtString(const char* fmt,va_list argList) {
   return JEncoder_vFmtString(this, fmt, argList); }
inline int ICACHE_FLASH_ATTR JEncoder::setString(const char* val) {
   return  JEncoder_setString(this, val); }
inline int ICACHE_FLASH_ATTR JEncoder::setBoolean(bool val) {
   return  JEncoder_setBoolean(this, val ? TRUE : FALSE); }
inline int ICACHE_FLASH_ATTR JEncoder::setNull() {
   return  JEncoder_setNull(this); }
inline int ICACHE_FLASH_ATTR JEncoder::setJV(struct JVal* val, bool iterateNext) {
   return  JEncoder_setJV(this,val,iterateNext?TRUE:FALSE); }
inline int /*ICACHE_FLASH_ATTR*/ JEncoder::set(const char* fmt, ...) {
   int retv; va_list argList;
   va_start(argList, fmt); 
   retv=JEncoder_vSetJV(this,&fmt,&argList);
   va_end(argList);
   return retv;
}
inline int ICACHE_FLASH_ATTR JEncoder::setName(const char* name) {
   return  JEncoder_setName(this, name); }
inline int ICACHE_FLASH_ATTR JEncoder::beginObject() {
   return  JEncoder_beginObject(this); }
inline int ICACHE_FLASH_ATTR JEncoder::endObject() {
   return  JEncoder_endObject(this); }
inline int ICACHE_FLASH_ATTR JEncoder::beginArray() {
   return  JEncoder_beginArray(this); }
inline int ICACHE_FLASH_ATTR JEncoder::endArray() {
   return  JEncoder_endArray(this); }
inline JErr* ICACHE_FLASH_ATTR JEncoder::getErr() {
   return JEncoder_getErr(this); }
inline int ICACHE_FLASH_ATTR JEncoder::flush() {
   return JEncoder_flush(this); }
inline int ICACHE_FLASH_ATTR JEncoder::commit() {
   return JEncoder_commit(this); }
inline BufPrint* ICACHE_FLASH_ATTR JEncoder::getBufPrint() {
   return JEncoder_getBufPrint(this); }
#endif

/** JEncoder::set helper macro, used when setting a value for an object.
    encoder.set("{d}", JE_MEMBER(structval, membername));
 */
#define JE_MEMBER(o, m) #m, (o)->m


/** @} */ /* end of JSONRef */ 

#endif
