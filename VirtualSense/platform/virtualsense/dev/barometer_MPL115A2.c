/*
 *  barometer_MPL115A2.c
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
 * The MPL115A2 barometer/temperature sensor driver.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
#include "platform-conf.h"
#include "dev/i2c.h"
#include "dev/leds.h"
#include "barometer_MPL115A2.h"

static  float _mpl115a2_a0;
static  float _mpl115a2_b1;
static  float _mpl115a2_b2;
static  float _mpl115a2_c12;

 void barometer_MPL115A2_init(){
	 P2DIR |= BIT7;
	 read_coefficients();
	 // read coefficient ??
 }

 void read_coefficients() {
   int16_t a0coeff;
   int16_t b1coeff;
   int16_t b2coeff;
   int16_t c12coeff;
   BAR_POWER_UP();
   i2c_enable();

   i2c_start();
   i2c_write((DEVICE_ADDRESS | WRITE));
   i2c_write((READ_COEFFICIENTS_COMMAND));

   i2c_start();
   i2c_write((DEVICE_ADDRESS | READ));

   a0coeff = (( (uint16_t) i2c_read(1) << 8) | i2c_read(1));
   b1coeff = (( (uint16_t) i2c_read(1) << 8) | i2c_read(1));
   b2coeff = (( (uint16_t) i2c_read(1) << 8) | i2c_read(1));
   c12coeff = (( (uint16_t) (i2c_read(1) << 8) | i2c_read(0))) >> 2;


   _mpl115a2_a0 = (float)a0coeff / 8;
   _mpl115a2_b1 = (float)b1coeff / 8192;
   _mpl115a2_b2 = (float)b2coeff / 16384;
   _mpl115a2_c12 = (float)c12coeff;
   _mpl115a2_c12 /= 4194304.0;

   i2c_stop();
   i2c_disable();
   BAR_POWER_DOWN();
 }

 uint16_t read_pressure_barometer_MPL115A2(){
	 uint16_t press = 0;
	 uint16_t temp = 0;
	 float press_comp = 0;
	 uint8_t res = 0;
	 	 BAR_POWER_UP();
	 	 i2c_enable();

	 	 i2c_start();
	 	 i2c_write((DEVICE_ADDRESS | WRITE));
	 	 i2c_write((START_CONVERSION_COMMAND));
	 	 i2c_write((0x01)); // trovato su un forum di uno spagnolo
	 	 i2c_stop();
	 	 //delay 1.6 ms
	 	 __delay_cycles(1662500);

	 	 // read temperature first

	 	i2c_start();
	 	i2c_write((DEVICE_ADDRESS | WRITE));
	 	i2c_write((READ_TEMPERATURE_MSB_COMMAND));

	 	i2c_start();
	 	i2c_write((DEVICE_ADDRESS | READ));
        res = i2c_read(1);
	 	//printf("temp MSB %x\n", res);
	 	temp = (res << 8);
	 	res = i2c_read(0);
	 	temp |= res;
	 	temp = temp >> 6;

	 	//printf("temp is %d\n", temp);

	 	i2c_stop();

	 	i2c_start();
	 	i2c_write((DEVICE_ADDRESS | WRITE));
	 	i2c_write((READ_PRESSURE_MSB_COMMAND));

	 	 i2c_start();
	 	 i2c_write((DEVICE_ADDRESS | READ));

	 	 res = i2c_read(1);
	 	 //printf("temp MSB %x\n", res);
	 	 press = (res << 8);
	 	 res = i2c_read(0);
	 	 press |= res;
	 	 press = press >> 6;

	 	 i2c_stop();
	 	 i2c_disable();
	 	 BAR_POWER_DOWN();

	 	//printf("press is %d\n", press);

	 	press_comp = _mpl115a2_a0 + (_mpl115a2_b1 + _mpl115a2_c12 * temp ) * press + _mpl115a2_b2 * temp;

	 	// hPa
	 	 return (uint16_t)((((65.0F / 1023.0F) * press_comp) + 50.0F)*10);

 }

 uint16_t read_temperature_barometer_MPL115A2(){
 	 uint16_t ret = 0;
 	 uint8_t res = 0;
 	 float t;
 	 BAR_POWER_UP();
 	 i2c_enable();

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | WRITE));
 	 i2c_write((START_CONVERSION_COMMAND));
 	 i2c_write((0x01)); // trovato su un forum di uno spagnolo
 	 i2c_stop();
 	 //delay 1.6 ms
 	 __delay_cycles(1662500);

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | WRITE));
 	 i2c_write((READ_TEMPERATURE_MSB_COMMAND));

 	 i2c_start();
 	 i2c_write((DEVICE_ADDRESS | READ));

 	 res = i2c_read(1);
 	 //printf("temp MSB %x\n", res);
 	 ret = (res << 8);
 	 res = i2c_read(0);
 	 ret |= res;
 	 ret = ret >> 6;

 	 i2c_stop();
 	 i2c_disable();
 	 BAR_POWER_DOWN();


 	  t = ((float) ret - 498.0F) / -5.35F +25.0F;           // C
 	  ret = (uint16_t)(t*100);
 	 return ret;
 }



