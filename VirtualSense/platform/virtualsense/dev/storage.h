/*
 *	storage.h
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
 * Storage definition.
 *
 * @author Matteo Dromedari
 *
 */

#include "flash_SST26WF080B.h"

#ifndef __STORAGE_H__
#define __STORAGE_H__


#define STORAGE_SIZE 						524288
#define STORAGE_SECTOR						128

#define STORAGE_BASE_ADR 					0
#define STORAGE_MAX_ADR 					STORAGE_SIZE - 1

#define STORAGE_ENTRY_SIZE 					6					// | app_id (1byte) | var_id (1byte) | var (4byte) |
																//     0xFF - free

#define STORAGE_SECTOR_HEADER_SIZE			4
#define STORAGE_SECTOR_ENTRY_COUNT			682




void save_var(uint16_t, uint16_t, uint32_t);
uint32_t read_var(uint16_t, uint16_t);
void delete_var(uint16_t, uint16_t);
void write_mem(uint8_t);
void format_mem();



#endif /* __STORAGE_H__ */
