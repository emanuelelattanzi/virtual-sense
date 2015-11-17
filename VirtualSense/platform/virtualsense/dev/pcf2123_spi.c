/*
 *  pcf2321_spi.h
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
 * The SPI communication system with PCF2123 RTC.
 *
 * @author Emanuele Lattanzi
 *
 */


#include <msp430.h>
//#include <legacymsp430.h>
#include "pcf2123_spi.h"
#include "contiki-conf.h"
#include "power-interface.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif

uint8_t RTC_is_up(void){ //TODO: trovare un modo pulito per farlo
	uint8_t res = 0;
	if(RTC_get_year() == 11)
		res = 1;
	return res;
}

/* initilize the RTC module */
void RTC_init(void){
	//PRINTF("Init rtc\n");
	 // to remove when removed transistor
	lock_SPI(); //NON ci sarebbe bisogno ma in questo modo impedisco che PM spenga
	// la SPI durante l'inizializzazione. Non servirebbe perch� ogni write acquisisce il lock
	// in questo modo ho performace maggiori
	uint8_t cout_value = RTC_read_register(PCF2123_REG_T_CLOKOUT);
	//cout_value = 0x00; MET debug
	if(cout_value != 0x71){ // the rtc should be initialized because it has been just powerd-up
		PRINTF("Need to initialize RTC\n");
		/* reset RTC */
		RTC_write_register(PCF2123_REG_CTRL1, PCF2123_RESET);

		// setting low power mode by sourcing countdown timer with 1/60 Hz,
		// setting clockout frequency at 1Hz and disable countdown timer
		RTC_write_register(PCF2123_REG_T_CLOKOUT, PCF2123_COUT_F_1 | PCF2123_CDT_SF_1_64 | PCF2123_TIMER_DI);



		/*uint8_t oldValue = RTC_read_register(PCF2123_REG_CTRL2);
		oldValue |= PCF2123_SC_INT; // enable second interrupt for testing
		RTC_write_register(PCF2123_REG_CTRL2, oldValue); // Enable alarm interrupt
		*/

		//Set time after power up to Giulia Lattanzi's birth day Sunday 27/11/11 14:24
		RTC_write_register(PCF2123_REG_HR, bin2bcd(0x0E));
		RTC_write_register(PCF2123_REG_MN, bin2bcd(0x18));
		RTC_write_register(PCF2123_REG_SC, bin2bcd(0x01));

		RTC_write_register(PCF2123_REG_MO, bin2bcd(0x0B));
		RTC_write_register(PCF2123_REG_DM, bin2bcd(0x1B));
		RTC_write_register(PCF2123_REG_DW, bin2bcd(0x00));
		RTC_write_register(PCF2123_REG_YR, bin2bcd(0x0B));
		release_SPI();
		PRINTF("RTC initialized with minutes %d\n", bcd2bin(RTC_read_register(PCF2123_REG_MN)));
	}
}

void RTC_stop(void){
	RTC_write_register(PCF2123_REG_CTRL1, PCF2123_STOP);
}

void RTC_schedule_interrupt_at_minutes(uint8_t minutes){
	lock_SPI();
	uint8_t hours = minutes/60;
	uint8_t currentMinutes = minutes % 60;
	uint8_t current_value = RTC_read_register(PCF2123_REG_CTRL2);
	current_value |= PCF2123_AL_INT;
	RTC_write_register(PCF2123_REG_CTRL2, current_value); // Enable alarm interrupt
	PRINTF("Setting interrupt  at hours %d and minutes %d\n",hours,minutes);
	if(hours)
		RTC_write_register(PCF2123_REG_HR_ALARM, bin2bcd(hours)); //set alarm hours
	RTC_write_register(PCF2123_REG_MN_ALARM, bin2bcd(minutes)); //set alarm minutes
	release_SPI();
}
void RTC_schedule_interrupt_at_hours(uint8_t hours){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_HR_ALARM, bin2bcd(hours)); //set alarm hours
}
void RTC_schedule_interrupt_at_days(uint8_t days){
	RTC_write_register(PCF2123_REG_CTRL2, PCF2123_AL_INT); // Enable alarm interrupt
	RTC_write_register(PCF2123_REG_DA_ALARM, bin2bcd(days)); //set alarm minutes
}


void RTC_clear_interrupt(void){
	uint8_t actual_value = RTC_read_register(PCF2123_REG_CTRL2);
	RTC_write_register(PCF2123_REG_CTRL2, (actual_value & 0xD3)); //clear BIT 5,3,2 --> 11010011
}
void RTC_disable_all_interrupts(void){
	RTC_write_register(PCF2123_REG_CTRL2, 0x00);
}
uint8_t RTC_get_year(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_YR));
}

uint8_t RTC_get_month(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_MO));
}

uint8_t RTC_get_day_of_month(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_DM));
}

uint8_t RTC_get_day_of_week(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_DW));
}

uint8_t RTC_get_hours(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_HR));
}

uint8_t RTC_get_minutes(void){
	return bcd2bin(RTC_read_register(PCF2123_REG_MN));
}

uint8_t RTC_get_seconds(void){
	uint8_t sec = bcd2bin(RTC_read_register(PCF2123_REG_SC));
	if (sec > 60)
		sec = sec -80;

	return sec; //bcd2bin(RTC_read_register(PCF2123_REG_SC));
}

/*
 *	transmit a byte
 */
void B1_tx(uint8_t val){
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	UCB1TXBUF = val;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );

}

/*
 * read a byte
 */
uint8_t B1_rx(void){

	uint8_t read;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	UCB1TXBUF = 0x00;
	/// wait until a tx operation end
	while( UCB1STAT & UCBUSY );
	/// when write operation is finished, received data is ready.
	read = UCB1RXBUF;
	/// close connection
	return read;
}

uint8_t RTC_read_register(uint8_t aReg)
{
	uint8_t result;
	spi_UCB1_init(0x02);
	lock_SPI();
	PCF2123_SPI_ENABLE();//CE_ACTIVE;
	// Write "command"
	B1_tx(aReg|PCF2123_READ);
	// Read back "data"
	result = B1_rx();
	PCF2123_SPI_DISABLE();//CE_INACTIVE;
	release_SPI();
	return result;
}

void RTC_write_register(uint8_t aReg, uint8_t aValue)
{
	spi_UCB1_init(0x02);
	lock_SPI();
	PCF2123_SPI_ENABLE();//CE_ACTIVE;
	B1_tx(aReg|PCF2123_WRITE);
	B1_tx(aValue);
	PCF2123_SPI_DISABLE();//CE_INACTIVE;
	release_SPI();
}



