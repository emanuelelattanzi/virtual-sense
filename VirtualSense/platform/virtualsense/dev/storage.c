/*
 *	storage.c
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
 * Storage definitions.
 *
 * @author Matteo Dromedari
 *
 */



#include <contiki.h>
//#include <dev/spi.h>
//#include <dev/leds.h>

#include "board.h"
#include "storage.h"


uint8_t sector_buffer[FLASH_SECTOR_SIZE];


void save_int_var(uint16_t app_id, uint16_t var_id, int32_t var) {

	uint8_t find = 0;
	uint8_t i = 0;
	uint32_t first_free_entry = 0xFFFFFFFF;

	// For each page table sector find free position
	for(i = 0; i < STORAGE_SECTOR && !find; i++) {
		uint8_t entries_count[4];
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);

		read_sequential_SST26WF080B(current_sector_base, entries_count, PAGE_TABLE_SECTORS_EADER_SIZE);
		// Store read byte on buffer
		sector_buffer[current_sector_base] = entries_count[0];
		sector_buffer[current_sector_base+1] = entries_count[1];
		sector_buffer[current_sector_base+2] = entries_count[2];
		sector_buffer[current_sector_base+3] = entries_count[3];

		if(entries_count[3] != 0xFF) {
			// Scan current sector
			current_sector_base += 4;
			uint8_t j = 0;
			while(j < STORAGE_SECTOR_ENTRY_COUNT && (entries_count[3] > 0x00 || find)) {
				uint8_t entry[STORAGE_ENTRY_SIZE];
				uint32_t current_entry_base = current_sector_base + (j*PAGE_TABLE_ENTRY_SIZE);
				// Scan current entry
				read_sequential_SST26WF080B(current_entry_base, entry, PAGE_TABLE_ENTRY_SIZE);

				if(entry[0] != 0xFF) {
					sector_buffer[current_entry_base] = entry[0];
					sector_buffer[current_entry_base+1] = entry[1];

					if(entry[0] == (uint8_t)app_id &&
						entry[1] == (uint8_t)var_id) {

						find = 0x01;

						sector_buffer[current_entry_base+2] = (uint8_t)(var >> 24);
						sector_buffer[current_entry_base+3] = (uint8_t)(var >> 16);
						sector_buffer[current_entry_base+4] = (uint8_t)(var >> 8);
						sector_buffer[current_entry_base+5] = (uint8_t)var;
					}
					else {
						sector_buffer[current_entry_base+2] = entry[2];
						sector_buffer[current_entry_base+3] = entry[3];
						sector_buffer[current_entry_base+4] = entry[4];
						sector_buffer[current_entry_base+5] = entry[5];
					}
					entries_count[3]--;
				}
				else {
					sector_buffer[current_entry_base] = 0xFF;
					// Set first free entry
					if(first_free_entry == 0xFFFFFFFF)
						first_free_entry = current_entry_base;
				}
			}

			// Update sector on memory
			write_sector_SST26WF080B(current_sector_base - 4, sector_buffer);
		}
		else {
			printf("page table sector%d: empty", i);
			// Set first free entry
			if(first_free_entry == 0xFFFFFFFF)
				first_free_entry = current_sector_base + 4;;
		}

	}

	if(!find && first_free_entry != 0xFFFFFFFF) {
		// Reload sector to update
		uint32_t current_sector_base = (first_free_entry % STORAGE_SECTOR)*FLASH_SECTOR_SIZE;
		read_sector_SST26WF080B(current_sector_base, sector_buffer);
		// Update number of entry
		sector_buffer[3]++;
		uint32_t current_entry_base = current_sector_base - first_free_entry;
		sector_buffer[current_entry_base] = app_id;
		sector_buffer[current_entry_base+1] = var_id;
		sector_buffer[current_entry_base+2] = (uint8_t)(var >> 24);
		sector_buffer[current_entry_base+3] = (uint8_t)(var >> 16);
		sector_buffer[current_entry_base+4] = (uint8_t)(var >> 8);
		sector_buffer[current_entry_base+5] = (uint8_t)var;

		// Update sector on memory
		write_sector_SST26WF080B(current_sector_base, sector_buffer);
	}
}


int32_t read_int_var(uint16_t app_id, uint16_t var_id) {

	uint32_t var = 0;
	uint8_t find = 0;
	uint8_t i = 0;
	// For each page table sector find var
	for(i = 0; i < STORAGE_SECTOR && (find != 0); i++) {
		uint8_t entries_count[4];
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);

		read_sequential_SST26WF080B(current_sector_base, entries_count, STORAGE_SECTOR_HEADER_SIZE);

		if(entries_count[3] != 0xFF) {
			// Scan current page table sector
			current_sector_base += 4;
			uint8_t j = 0;
			for(j = 0; j < entries_count[3]; j++) {
				uint8_t entry[STORAGE_SECTOR_HEADER_SIZE];

				read_sequential_SST26WF080B(current_sector_base + (j*STORAGE_SECTOR_HEADER_SIZE), entry, STORAGE_SECTOR_HEADER_SIZE);

				if(entry[0] == (uint8_t)app_id &&
					entry[1] == (uint8_t)var_id) {
					// Find entry compose mem adr
					find = 0x01;
					var = var | ((uint32_t)entry[2]) << 24;
					var = var | ((uint32_t)entry[3]) << 16;
					var = var | ((uint32_t)entry[4]) << 8;
					var = var | ((uint32_t)entry[3]);
				}
			}
		}
		else
			printf("page table sector%d: empty", i);
	}

	return mem_adr;
}


void delete_var(uint16_t app_id, uint16_t var_id) {

	uint8_t find = 0;
	uint8_t i = 0;

	// For each page table sector find entry
	for(i = 0; i < STORAGE_SECTOR && !find; i++) {
		uint8_t entries_count[4];
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);

		read_sequential_SST26WF080B(current_sector_base, entries_count, PAGE_TABLE_SECTORS_EADER_SIZE);
		// Store read byte on buffer
		sector_buffer[current_sector_base] = entries_count[0];
		sector_buffer[current_sector_base+1] = entries_count[1];
		sector_buffer[current_sector_base+2] = entries_count[2];
		sector_buffer[current_sector_base+3] = entries_count[3];

		if(entries_count[3] != 0xFF) {
			// Scan current sector
			current_sector_base += 4;
			uint8_t j = 0;
			while(j < STORAGE_SECTOR_ENTRY_COUNT && (entries_count[3] > 0x00 || find)) {
				uint8_t entry[STORAGE_ENTRY_SIZE];
				uint32_t current_entry_base = current_sector_base + (j*PAGE_TABLE_ENTRY_SIZE);
				// Scan current entry
				read_sequential_SST26WF080B(current_entry_base, entry, PAGE_TABLE_ENTRY_SIZE);

				if(entry[0] != 0xFF) {
					if(entry[0] == (uint8_t)app_id &&
						entry[1] == (uint8_t)var_id) {

						find = 0x01;
						sector_buffer[current_entry_base] = 0xFF;
						sector_buffer[current_entry_base+1] = 0xFF;
					}
					else {
						sector_buffer[current_entry_base] = entry[0];
						sector_buffer[current_entry_base+1] = entry[1];
						sector_buffer[current_entry_base+2] = entry[2];
						sector_buffer[current_entry_base+3] = entry[3];
						sector_buffer[current_entry_base+4] = entry[4];
						sector_buffer[current_entry_base+5] = entry[5];
					}
					entries_count[3]--;
				}
				else {
					sector_buffer[current_entry_base] = 0xFF;
				}
			}
			sector_buffer[3]--;

			// Update sector on memory
			write_sector_SST26WF080B(current_sector_base - 4, sector_buffer);
		}
	}
}























































