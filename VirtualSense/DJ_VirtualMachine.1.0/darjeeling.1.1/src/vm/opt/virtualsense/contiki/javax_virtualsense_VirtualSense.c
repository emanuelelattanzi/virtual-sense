/*
 *	javax_lang_Darjeeling.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#ifdef PLATFORM_HAS_RTC_PCF2123
#include "dev/pcf2123_spi.h"
#endif
#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "common/djtimer.h"
#include "net/rime/rimeaddr.h"
#include "../../../../node_id.h"



// int javax.virtualsense.VirtualSense.getNodeId()
void javax_virtualsense_VirtualSense_short_getNodeId()
{
	// Push node id on the stack
	dj_exec_stackPushShort(virtualsense_node_id);
}

// void javax.virtualsense.VirtualSense.setNodeId(int nodeId)
/*void javax_virtualsense_VirtualSense_void_setNodeId_short()
{
	// Pop node id from the stack and write it on the flash
	uint16_t node_id = dj_exec_stackPopShort();
	node_id_burn(node_id);
	NODEID_write(dj_exec_stackPopInt());
}*/

void javax_virtualsense_VirtualSense_void_printTime(){
#ifdef PLATFORM_HAS_RTC_PCF2123

	uint8_t sec = RTC_get_seconds();
	uint8_t min = RTC_get_minutes();
	uint8_t hou = RTC_get_hours();

	printf(" TIME %u:%u:%u", hou,min,sec);
#endif

/*void javax_virtualsense_VirtualSense_java_lang_String_getTime(){
#ifdef PLATFORM_HAS_RTC_PCF2123

	uint8_t sec = RTC_get_seconds();
	uint8_t min = RTC_get_minutes();
	uint8_t hou = RTC_get_hours();

	printf("%u:%u:%u", hou,min,sec);
#endif*/

}
