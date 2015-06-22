/*
 *  SI7020.h
 *
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * The SI7020 temperature humidity sensor driver.
 *
 * @author Matteo Dromedari
 *
 */

//#define SI7020_POWER_PORT			GPIO_C_BASE	//GPIO_D_BASE
//#define SI7020_POWER_PIN			GPIO_PIN_0	//GPIO_PIN_5

#define SI7020_ADDRESS 				0x40

#define SI7020_READ_TEMP_NOHOLD		0xF3
#define SI7020_READ_HUMID_NOHOLD	0xF5
#define SI7020_RESET			0xFE

void init_SI7020(void);
uint16_t read_temp_SI7020(void);
uint16_t write_humid_SI7020(void);
