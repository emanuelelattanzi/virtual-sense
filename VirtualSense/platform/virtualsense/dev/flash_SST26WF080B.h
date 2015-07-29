/*
 *  flash_SST26WF080B.h
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
 * The flash implementation by SST26WF080B.
 *
 * @author Matteo Dromedari
 *
 */


#define	FLASH_PAGE_SIZE						256
#define FLASH_SECTOR_SIZE 					4096

#define FLASH_SIZE 							1048576
#define FLASH_BASE_ADR 						0
#define FLASH_MAX_ADR 						FLASH_SIZE - 1

#define FLASH_PAGE_PROG_CMD 				0x02
#define FLASH_READ_SEQUENCE					0x03
#define FLASH_HI_SPEED_READ					0x0B

#define FALSH_ERASE_SECTOR_CMD				0x20
#define FALSH_CHIP_ERASE_CMD				0xC7

#define FLASH_READ_STAT_REG					0x05
#define FLASH_WRITE_STAT_REG				0x01
#define FLASH_READ_CONF_REG					0x35
#define FLASH_WRITE_CONF_REG				0x06
#define FLASH_WRITE_ENABLE					0x06
#define FLASH_WRITE_BLOCK_PROTECT_REGISTER	0x42
#define FLASH_GLOBAL_BLOCK_PROTECT_UNLOCK	0x98



void init_SST26WF080B();
void chip_erase_SST26WF080B();
void erase_sector_SST26WF080B(uint32_t base_sector_adr);
void write_sector_SST26WF080B(uint32_t base_sector_adr, uint8_t *data);
void write_page_SST26WF080B(uint32_t base_page_adr, uint8_t *data);
void read_sector_SST26WF080B(uint32_t base_adr, uint8_t *read_sector);
void read_sequential_SST26WF080B(uint32_t base_adr, uint8_t *read_bytes, uint32_t byte_to_read);
