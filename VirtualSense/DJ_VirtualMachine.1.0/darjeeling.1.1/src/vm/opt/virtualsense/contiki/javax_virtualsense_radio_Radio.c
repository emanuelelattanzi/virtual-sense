/*
 *	javax_virtualsense_radio_Radio.c
 *
 *  Copyright (c) 2012 DiSBeF, University of Urbino.
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
 * Radio native methods.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "common/execution/execution.h"
#include "common/array.h"
#include "common/types.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/leds.h"
#include "dev/board.h"


#include "base_definitions.h"


dj_thread * receiver_thread;
//static struct abc_conn uc;
static struct broadcast_conn broadcast;
static struct unicast_conn unicast;


void javax_virtualsense_radio_Radio_void__waitForMessage()
{
		// wait for radio
		lock_RF(); // lock radio module to prevent rf shutdown
		lock_MAC();// lock mac layer to prevent duty cycle shutdown;
        receiver_thread = dj_exec_getCurrentThread();
        //printf("Thread waiting for message is %d\n", receiver_thread->id);
        receiver_thread->status = THREADSTATUS_BLOCKED_FOR_IO;
        dj_exec_breakExecution();
}

// byte[] javax.virtualsense.radio.Radio._readBytes()
void javax_virtualsense_radio_Radio_byte____readBytes()
{
        dj_int_array * ret;
        int length = packetbuf_datalen();

        ret = dj_int_array_create(T_BYTE, length);

        // copy data from the rimebuf to the return array

        packetbuf_copyto(ret->data.bytes);


        //packetbuf_clear();
        dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}

// void javax.virtualsense.radio.Radio._init()
void javax_virtualsense_radio_Radio_void__init()
{

	//uc = network_init();
	broadcast	= broadcast_network_init();
	unicast 	= unicast_network_init();

}


// void javax.virtualsense.radio.Radio._broadcast(byte[])
void javax_virtualsense_radio_Radio_void__broadcast_byte__()
{

	rimeaddr_t addr;


	dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());

	// check null
	if (byteArray==nullref)
			dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);

	// copy bytes to the rime buffer
    packetbuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    //printf("Setting data length to %d\n",byteArray->array.length);
    packetbuf_set_datalen(byteArray->array.length);
    // abc
    lock_RF();
    broadcast_send(&broadcast);
    release_RF();

}


// boolean javax.virtualsense.radio.Radio._send(short, byte[])
void javax_virtualsense_radio_Radio_boolean__send_short_byte__()
{

	rimeaddr_t addr;


	dj_int_array * byteArray = REF_TO_VOIDP(dj_exec_stackPopRef());
	int16_t id = dj_exec_stackPopShort();
	addr.u8[0] = id & 0xff;
	addr.u8[1] = id >> 8;
	printf("adr0: %x", addr.u8[0]);
	printf("adr1: %x", addr.u8[1]);

	// check null
	if (byteArray==nullref)
			dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);

	// copy bytes to the rime buffer
    packetbuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    packetbuf_set_datalen(byteArray->array.length);

    // abc
    lock_RF();
    //rime_mac->on();
    unicast_send(&unicast, &addr);
    release_RF();

}
// short javax.virtualsense.radio.Radio.getDestId()
void javax_virtualsense_radio_Radio_short_getDestId(){
	// Destination should be broadcast or node id because otherwise
	// contiki network stack does not call the callback
	dj_exec_stackPushShort(packetbuf_attr(PACKETBUF_ADDR_RECEIVER));
}

void javax_virtualsense_radio_Radio_short_getSenderId(){
	dj_exec_stackPushShort(packetbuf_attr(PACKETBUF_ADDR_SENDER));
}

