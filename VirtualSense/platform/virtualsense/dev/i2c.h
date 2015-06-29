/*
 *	i2c.h
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
 * I2C access definitions.
 *
 * @author Matteo Dromedari
 *
 */

#ifndef I2C_H
#define I2C_H

#include "lib/_i2c.h"
#include "lib/_gpio.h"
#include "lib/_interrupt.h"
#include "lib/_ioc.h"
#include "lib/_sys_ctrl.h"
#include "lib/hw_ints.h"
#include "lib/hw_memmap.h"
#include "lib/hw_ioc.h"
#include "lib/hw_i2cm.h"
#include "lib/hw_i2cs.h"

#define I2C_ENABLE()  (i2c_enable())
#define I2C_DISABLE() (i2c_disable())

void 	 i2c_init(void);
void     i2c_enable(void);
void     i2c_disable(void);

uint8_t i2c_write(uint8_t, uint8_t *, uint16_t);
uint16_t i2c_read(uint8_t, uint8_t *, uint16_t);
uint8_t i2c_write_burst(uint8_t, uint8_t, uint8_t *, uint16_t);
uint16_t i2c_read_burst(uint8_t, uint8_t, uint8_t *, uint16_t);


#endif /* I2C_H */
