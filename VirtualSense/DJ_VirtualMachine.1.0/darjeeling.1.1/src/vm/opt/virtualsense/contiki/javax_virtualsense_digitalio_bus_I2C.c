/*
 *	javax_virtualsense_bus_digitalio_I2C.c
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
 * I2C bus native methods.
 *
 * @author Emanuele Lattanzi
 *
 */

#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/i2c.h"
#include "dev/board.h"


#include "base_definitions.h"



void javax_virtualsense_digitalio_bus_I2C_void__enable(){
	i2c_enable();
}
void javax_virtualsense_digitalio_bus_I2C_void__disable(){
	i2c_disable();
}
void javax_virtualsense_digitalio_bus_I2C_byte____read_byte_short(){
	dj_int_array * ret;
	uint16_t len = dj_exec_stackPopShort();
	uint8_t adr = dj_exec_stackPopShort();

    ret = dj_int_array_create(T_BYTE, len);
    uint16_t l = i2c_read(adr, ret->data.bytes, ret->array.length);
    //printf("letti: %d, daleggere: %d\n", l, len);

    dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}
void javax_virtualsense_digitalio_bus_I2C_boolean__write_byte_byte__(){
	dj_int_array * values = REF_TO_VOIDP(dj_exec_stackPopRef());
	uint8_t adr = dj_exec_stackPopShort();

	dj_exec_stackPushShort((uint16_t)i2c_write(adr, values->data.bytes, values->array.length));
}
void javax_virtualsense_digitalio_bus_I2C_byte____readBurst_byte_byte_short(){
	dj_int_array * ret;
	uint16_t len = dj_exec_stackPopShort();
	uint8_t reg_adr = dj_exec_stackPopShort();
	uint8_t dev_adr = dj_exec_stackPopShort();


    ret = dj_int_array_create(T_BYTE, len);
    uint16_t l =i2c_read_burst(dev_adr, reg_adr, ret->data.bytes, ret->array.length);
    //printf("letti: %d, daleggere: %d\n", l, len);
    dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}
void javax_virtualsense_digitalio_bus_I2C_boolean__writeBurst_byte_byte_byte__(){
	dj_int_array * values = REF_TO_VOIDP(dj_exec_stackPopRef());
	uint8_t reg_adr = dj_exec_stackPopShort();
	uint8_t dev_adr = dj_exec_stackPopShort();


	dj_exec_stackPushShort((uint16_t)i2c_write_burst(dev_adr, reg_adr, values->data.bytes, values->array.length));
}


