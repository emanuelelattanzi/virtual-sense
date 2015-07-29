/*
 *	javax_virtualsense_storage_Storage.c
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
 *
 *	This file is part of VirtualSense.
 *
 *	VirtualSense is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	VirtualSense is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Storage access methods.
 *
 * @author Matteo Dromedari
 *
 */

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "common/heap/heap.h"
#include "dev/storage.h"

#include "base_definitions.h"





// javax.virtualsense.storage void _saveIntVar(short appId, short varId, int var);
void javax_virtualsense_storage_Storage_void__saveIntVar_short_short_int(){

	int32_t var =  dj_exec_stackPopInt();
	uint16_t var_id = dj_exec_stackPopShort();
	uint16_t app_id = dj_exec_stackPopShort();

	printf("c - save int var\n");
	printf(" var: %d\n", var);
	printf(" var id: %d\n", var_id);
	printf(" app id: %d\n", app_id);
}

// javax.virtualsense.storage int _readIntVar(short appId, short varId);
void javax_virtualsense_storage_Storage_int__readIntVar_short_short(){

	uint16_t var_id = dj_exec_stackPopShort();
	uint16_t app_id = dj_exec_stackPopShort();

	printf("c - read int var\n");
	printf(" var id: %d\n", var_id);
	printf(" app id: %d\n", app_id);

	dj_exec_stackPushInt(0);
}

// javax.virtualsense.storage void _deleteVar(short appId, short varId);
void javax_virtualsense_storage_Storage_void__deleteVar_short_short(){
	
	uint16_t var_id = dj_exec_stackPopShort();
	uint16_t app_id = dj_exec_stackPopShort();

	printf("c - delete var\n");
	printf(" var id: %d\n", var_id);
	printf(" app id: %d\n", app_id);
}
