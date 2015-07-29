/*
 *  flash_SST26WF080B.c
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



#include "board.h"
#include "dev/ssi.h"
#include "flash_SST26WF080B.h"



void init_SST26WF080B() {
	ssi0_spi_init(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Set chip enable up (SST26WF080B is enabled with falling edge)
	lock_SPI();
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Write status and configuration register
	printf("Init SST26WF080B... ");
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_CONF_REG);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_STAT_REG);
	// Send status register
	ssi0_spi_write(0x00);
	// Send conf register
	ssi0_spi_write(0x88);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Read conf register
	printf("conf reg:");
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_READ_CONF_REG);
	printf("%x\n", ssi0_spi_read());
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Enable write
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_ENABLE);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Remove block protection
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_BLOCK_PROTECT_REGISTER);
	ssi0_spi_write(0x00);
	ssi0_spi_write(0x00);
	ssi0_spi_write(0x00);
	ssi0_spi_write(0x00);

	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Check for flash busy
	uint8_t status = 0x01;
	while(status != 0x00) {
		ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
		ssi0_spi_write(FLASH_READ_STAT_REG);
		status = ssi0_spi_read();
		ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	}

	release_SPI();
}


void write_page_SST26WF080B(uint32_t base_page_adr, uint8_t *data) {

	lock_SPI();

	// Enable write
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_ENABLE);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Send page program command
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_PAGE_PROG_CMD);
	// Send 24 bit base page address
	ssi0_spi_write((uint8_t)(base_page_adr >> 16));
	ssi0_spi_write((uint8_t)(base_page_adr >> 8));
	ssi0_spi_write((uint8_t)base_page_adr);
	// Send page
	uint16_t i = 0;
	for(i = 0; i < FLASH_PAGE_SIZE; i++) {
		ssi0_spi_write(data[i]);
	}
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Check for flash busy
	uint8_t status = 0x01;
	while(status != 0x00) {
		ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
		ssi0_spi_write(FLASH_READ_STAT_REG);
		status = ssi0_spi_read();
		ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	}

	release_SPI();
}


void erase_sector_SST26WF080B(uint32_t base_sector_adr) {

	lock_SPI();

	// Enable write
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_ENABLE);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Send erase sector command
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FALSH_ERASE_SECTOR_CMD);
	// Send 24 bit base page address
	ssi0_spi_write((uint8_t)(base_sector_adr >> 16));
	ssi0_spi_write((uint8_t)(base_sector_adr >> 8));
	ssi0_spi_write((uint8_t)base_sector_adr);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Check for flash busy
	uint8_t status = 0x01;
	while(status != 0x00) {
		ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
		ssi0_spi_write(FLASH_READ_STAT_REG);
		status = ssi0_spi_read();
		ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	}

	release_SPI();
}


void chip_erase_SST26WF080B() {

	lock_SPI();

	// Enable write
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FLASH_WRITE_ENABLE);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Send chip erase command
	ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	ssi0_spi_write(FALSH_CHIP_ERASE_CMD);
	ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

	// Check for flash busy
	uint8_t status = 0x01;
	while(status != 0x00) {
		ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
		ssi0_spi_write(FLASH_READ_STAT_REG);
		status = ssi0_spi_read();
		ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
	}

	release_SPI();
}


void write_sector_SST26WF080B(uint32_t base_sector_adr, uint8_t *data) {

	uint8_t page = 0;

	for(page = 0; page < 16; page++) { // 16 = 4096/256 number of page in a sector

		write_page_SST26WF080B((base_sector_adr + (page * 256)), data + (page * 256));
	}
}


void read_sector_SST26WF080B(uint32_t base_adr, uint8_t *read_sector) {

	read_sequential_SST26WF080B(base_adr, read_sector, FLASH_SECTOR_SIZE);
}


void read_sequential_SST26WF080B(uint32_t base_adr, uint8_t *read_bytes, uint32_t byte_to_read) {

		uint32_t i = 0;
		lock_SPI();

		ssi0_spi_ce_clr(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);

		// Send read sequence command
		ssi0_spi_write(FLASH_READ_SEQUENCE);

		// Send 24 bit base page address
		ssi0_spi_write((uint8_t)(base_adr >> 16));
		ssi0_spi_write((uint8_t)(base_adr >> 8));
		ssi0_spi_write((uint8_t)base_adr);

		// Read byte_to_read bytes from flash
		for(i = 0; i < byte_to_read; i++) {
			read_bytes[i] = ssi0_spi_read();
		}

		// Disable SPI communication with flash
		ssi0_spi_ce_set(NOR_FLASH_SPI_CE_PORT, NOR_FLASH_CE_PIN);
		release_SPI();
}



