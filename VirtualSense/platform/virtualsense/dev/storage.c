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

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif


uint8_t sector_buffer[FLASH_SECTOR_SIZE];


void save_int_var(uint16_t app_id, uint16_t var_id, int32_t var) {

	uint8_t find = 0;
	uint8_t i = 0;
	uint32_t first_free_entry = 0xFFFFFFFF;

	PRINTDEBUG("Save var: %d\n\n", var);
	// For each page table sector find free position
	for(i = 0; i < STORAGE_SECTOR && !find; i++) {

		// Read sector header
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);
		read_sequential_SST26WF080B(current_sector_base, sector_buffer, STORAGE_SECTOR_HEADER_SIZE);

		PRINTDEBUG("sector%d entries count: %d\n", i, sector_buffer[3]);
		if(sector_buffer[3] != 0xFF) {

			current_sector_base += 4;
			uint16_t entries_count = sector_buffer[3];
			uint16_t j = 0;
			// Scan current sector
			while(j < STORAGE_SECTOR_ENTRY_COUNT && (entries_count > 0 || first_free_entry == 0xFFFFFFFF || find)) {

				// Read current entry
				uint32_t buffer_base_entry = 4 + (j*STORAGE_ENTRY_SIZE);
				read_sequential_SST26WF080B(current_sector_base + (j*STORAGE_ENTRY_SIZE), &sector_buffer[buffer_base_entry], STORAGE_ENTRY_SIZE);
				PRINTDEBUG(" entry%d: app id: %d var id: %d", j, sector_buffer[buffer_base_entry], sector_buffer[buffer_base_entry+1]);

				if(sector_buffer[buffer_base_entry] != 0xFF) {
					// Check if current entry contains specified var
					if(sector_buffer[buffer_base_entry] == (uint8_t)app_id &&
					   sector_buffer[buffer_base_entry+1] == (uint8_t)var_id) {

						// Update var value
						find = 0x01;
						sector_buffer[buffer_base_entry+2] = (uint8_t)(var >> 24);
						PRINTDEBUG("  Save var: var[2]: %x\n", sector_buffer[buffer_base_entry+2]);
						sector_buffer[buffer_base_entry+3] = (uint8_t)(var >> 16);
						PRINTDEBUG("  Save var: var[3]: %x\n", sector_buffer[buffer_base_entry+3]);
						sector_buffer[buffer_base_entry+4] = (uint8_t)(var >> 8);
						PRINTDEBUG("  Save var: var[4]: %x\n", sector_buffer[buffer_base_entry+4]);
						sector_buffer[buffer_base_entry+5] = (uint8_t)var;
						PRINTDEBUG("  Save var: var[5]: %x", sector_buffer[buffer_base_entry+5]);
					}
					entries_count--;
				}
				else {
					// Set first free entry
					if(first_free_entry == 0xFFFFFFFF) {
						first_free_entry = current_sector_base + (buffer_base_entry - 4);
						PRINTDEBUG("  set first entry: %d", first_free_entry);
					}
				}
				j++;
				PRINTDEBUG("\n");
			}
			if(find) {
				PRINTDEBUG("\nFind update sector: %d\n", current_sector_base - 4);
				// Update sector on memory
				write_sector_SST26WF080B(current_sector_base - 4, sector_buffer);
			}
		}
		else {
			// Set first free entry
			if(first_free_entry == 0xFFFFFFFF) {
				first_free_entry = current_sector_base + 4;
				PRINTDEBUG("  set first entry: %d\n", first_free_entry);
			}
		}
	}

	if(!find && first_free_entry != 0xFFFFFFFF) {
		// Reload sector to update
		uint32_t current_sector_base = (first_free_entry / FLASH_SECTOR_SIZE) * FLASH_SECTOR_SIZE;
		read_sector_SST26WF080B(current_sector_base, sector_buffer);
		// Update number of entry
		if(sector_buffer[3] == 0xFF)
			sector_buffer[3] = 0x01;
		else
			sector_buffer[3]++;
		uint32_t current_entry_base = first_free_entry - current_sector_base;
		sector_buffer[current_entry_base] = app_id;
		sector_buffer[current_entry_base+1] = var_id;
		sector_buffer[current_entry_base+2] = (uint8_t)(var >> 24);
		sector_buffer[current_entry_base+3] = (uint8_t)(var >> 16);
		sector_buffer[current_entry_base+4] = (uint8_t)(var >> 8);
		sector_buffer[current_entry_base+5] = (uint8_t)var;
		PRINTDEBUG("\nSave var: write var(%d) on sector: %d entry: %d (first free entry: %d)\n", var, current_sector_base, current_entry_base, first_free_entry);
		// Update sector on memory
		write_sector_SST26WF080B(current_sector_base, sector_buffer);
	}
}


int32_t read_int_var(uint16_t app_id, uint16_t var_id) {

	uint32_t var = 0;
	uint8_t find = 0;
	uint8_t i = 0;
	// For each page table sector find var
	for(i = 0; i < STORAGE_SECTOR && !find; i++) {
		uint8_t entries_count[4];
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);

		read_sequential_SST26WF080B(current_sector_base, entries_count, STORAGE_SECTOR_HEADER_SIZE);

		PRINTDEBUG("Read var: sector: %d entries count: %d\n", i, entries_count[3]);
		if(entries_count[3] != 0xFF) {
			// Scan current page table sector
			current_sector_base += 4;
			uint8_t j = 0;
			for(j = 0; j < entries_count[3]; j++) {
				uint8_t entry[STORAGE_ENTRY_SIZE];

				read_sequential_SST26WF080B(current_sector_base + (j*STORAGE_ENTRY_SIZE), entry, STORAGE_ENTRY_SIZE);

				if(entry[0] == (uint8_t)app_id &&
					entry[1] == (uint8_t)var_id) {
					// Find entry compose mem adr
					PRINTDEBUG("Read var: find var ");
					find = 0x01;
					var = var | ((uint32_t)entry[2]) << 24;
					PRINTDEBUG("var[2]: %x ", entry[2]);
					var = var | ((uint32_t)entry[3]) << 16;
					PRINTDEBUG("var[3]: %x ", entry[3]);
					var = var | ((uint32_t)entry[4]) << 8;
					PRINTDEBUG("var[4]: %x ", entry[4]);
					var = var | ((uint32_t)entry[5]);
					PRINTDEBUG("var[5]: %x\n", entry[5]);
				}
			}
		}
		else
			PRINTDEBUG("Read var: page table sector%d: empty\n", i);
	}

	PRINTDEBUG("Read var: ret uint vatr: %d", var);

	return (int32_t)var;
}


void delete_var(uint16_t app_id, uint16_t var_id) {

	uint8_t find = 0;
	uint8_t i = 0;

	// For each page table sector find entry
	for(i = 0; i < STORAGE_SECTOR && !find; i++) {

		// Read sector header
		uint32_t current_sector_base = STORAGE_BASE_ADR + (i*FLASH_SECTOR_SIZE);
		read_sequential_SST26WF080B(current_sector_base, sector_buffer, STORAGE_SECTOR_HEADER_SIZE);

		if(sector_buffer[3] != 0xFF) {

			current_sector_base += 4;
			uint8_t entries_count = sector_buffer[3];
			uint8_t j = 0;
			// Scan current sector
			while(j < STORAGE_SECTOR_ENTRY_COUNT && (entries_count > 0x00 || find)) {

				// Read current entry
				uint32_t buffer_base_entry = 4 + (j*STORAGE_ENTRY_SIZE);
				read_sequential_SST26WF080B(current_sector_base + (j*STORAGE_ENTRY_SIZE), &sector_buffer[buffer_base_entry], STORAGE_ENTRY_SIZE);

				if(sector_buffer[buffer_base_entry] != 0xFF) {
					if(sector_buffer[buffer_base_entry] == (uint8_t)app_id &&
					   sector_buffer[buffer_base_entry+1] == (uint8_t)var_id) {

						find = 0x01;
						sector_buffer[buffer_base_entry] = 0xFF;
						sector_buffer[buffer_base_entry+1] = 0xFF;
					}
					entries_count--;
				}
				j++;
			}
			if(find) {
				// Update sector on memory
				sector_buffer[3]--;
				write_sector_SST26WF080B(current_sector_base - 4, sector_buffer);
			}
		}
	}
}


void write_mem(uint8_t sector_count) {
	uint8_t i = 0;
	for(i = 0; i < sector_count; i++) {//STORAGE_SECTOR; i++) {
		read_sequential_SST26WF080B(i * FLASH_SECTOR_SIZE, sector_buffer, FLASH_SECTOR_SIZE);

		printf("\n Write mem\nSector %d (entries count: %d)\n", i, sector_buffer[3]);
		uint16_t j = 0;
		for(j = 4; j < FLASH_SECTOR_SIZE; j += STORAGE_ENTRY_SIZE) {

			uint32_t var = 0;
			var = var | ((uint32_t)sector_buffer[j+2]) << 24;
			var = var | ((uint32_t)sector_buffer[j+3]) << 16;
			var = var | ((uint32_t)sector_buffer[j+4]) << 8;
			var = var | ((uint32_t)sector_buffer[j+5]);

			printf("entry[%d]: app_id: %d var_id: %d var: %d (%x %x %x %x)\n", (j-4)/6, sector_buffer[j], sector_buffer[j+1], var, sector_buffer[j+2], sector_buffer[j+3], sector_buffer[j+4], sector_buffer[j+5]);
		}
		printf("\n");
	}
	printf("\n\n");
}


void format_mem() {
	uint8_t i = 0;
	for(i = 0; i < STORAGE_SECTOR; i++) {
		erase_sector_SST26WF080B((uint32_t)i * FLASH_SECTOR_SIZE);
	}
}




















































