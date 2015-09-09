/*
 *     ____             _________                __                _
 *    / __ \___  ____ _/ /_  __(_)___ ___  ___  / /   ____  ____ _(_)____
 *   / /_/ / _ \/ __ `/ / / / / / __ `__ \/ _ \/ /   / __ \/ __ `/ / ___/
 *  / _, _/  __/ /_/ / / / / / / / / / / /  __/ /___/ /_/ / /_/ / / /__
 * /_/ |_|\___/\__,_/_/ /_/ /_/_/ /_/ /_/\___/_____/\____/\__, /_/\___/
 *                                                       /____/
 *
 *                  Barracuda Embedded Web-Server
 ****************************************************************************
 *            PROGRAM MODULE
 *
 *   $Id$
 *
 *   COPYRIGHT:  Real Time Logic, 2014
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
#ifndef BA_LIB
#define BA_LIB 1
#endif

#include <com/espressif/esp8266/include/osapi.h>

#include <AllocatorIntf.h>
//#include <string.h>

static void* ICACHE_FLASH_ATTR
AllocatorIntf_defaultMalloc(AllocatorIntf* o, size_t* size)
{
   return (void*) baMalloc(*size);
}


static void* ICACHE_FLASH_ATTR
AllocatorIntf_defaultRealloc(AllocatorIntf* o, void* memblock, size_t* size)
{
   return baRealloc(memblock, *size);
}


static void ICACHE_FLASH_ATTR
AllocatorIntf_defaultFree(AllocatorIntf* o, void* memblock)
{
   baFree(memblock);
}


BA_API AllocatorIntf* ICACHE_FLASH_ATTR
AllocatorIntf_getDefault(void)
{
   static AllocatorIntf alloc = {
      AllocatorIntf_defaultMalloc,
      AllocatorIntf_defaultRealloc,
      AllocatorIntf_defaultFree
   };

   return &alloc;
}


BA_API char* ICACHE_FLASH_ATTR
baStrdup2(struct AllocatorIntf* a, const char* str)
{
   char* dup;
   size_t size;
   if(!str) return 0;
   size = os_strlen(str)+1;
   dup = (char*)AllocatorIntf_malloc(a, &size);
   if(dup)
      os_strcpy(dup, str);
   return dup;
}
