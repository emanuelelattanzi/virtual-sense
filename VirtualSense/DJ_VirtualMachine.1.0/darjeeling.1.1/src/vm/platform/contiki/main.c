/*
 *	main.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */

//#include "../../../../../../examples/virtualsense/project-conf.h"
#include "contiki.h"

#include "dev/leds.h"
#include "dev/digitalio.h"
// TODO to implement #include "node-id.h"


#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"
#include "common/app_manager.h"
#include "common/command_manager.h"

#include "dev/board.h"
#include "loader.h"

#include "net/rime.h"


#if SERIAL_INPUT
#include "serial-input.h"
#endif


#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(darjeeling_process, "Dj");
AUTOSTART_PROCESSES(&darjeeling_process);
/*---------------------------------------------------------------------------*/

static void digital_io_callback(uint8_t port);
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
#if SERIAL_INPUT
static void serial_input_callback(char *line, int line_len);
#endif
char * ref_t_base_address;
static unsigned char mem[HEAPSIZE];

static dj_infusion *to_init = NULL;
//static unsigned char temp_di[TEMP_DI_SIZE]; // temporary buffer to copy di files from far rom
static struct etimer et;
static dj_vm * vm;
static long nextScheduleTime = 0;
static long deltaSleep = 0;
static uint8_t resume_from_hibernation = 0; /* to resume after hibernation */

/****************************************************************************************
 *  radio call-back handler:
 *  The contiki radio driver will invoke this call-back whenever a new radio packet
 *  is arrived.
 *
 * ***************************************************************************************/


/* These hold the broadcast and unicast structures, respectively. */
static struct broadcast_conn broadcast;
static struct unicast_conn unicast;
uint16_t receiver_thread_id = -1;


#if SERIAL_INPUT
uint16_t serial_receiver_thread_id = -1;
static char *serial_buffer;
static int serial_buffer_len = 0;
#endif

PROCESS_THREAD(darjeeling_process, ev, data)
{

	PROCESS_EXITHANDLER(goto exit;)
	PROCESS_BEGIN();
	printf("Starting Darjeeling !!!!\n");
	// Initialize memory manager
	dj_mem_init(mem, HEAPSIZE);
	ref_t_base_address = (char*)mem - 42;


	//load the VM from the restored heap
	//load the heap content from the hibernation file if found
	/* TODO resume_from_hibernation = load_machine(mem); */

	// initialise timer
	// TODO dj_timer_init();

	// init hw
	leds_init();

#if 0 //TODO to implement
	if(resume_from_hibernation){
			//printf("Loading VM from hibernation\n");
			vm = dj_vm_load_from_heap(mem);
			/*DEBUG_LOG("VM_POINTER %p\n", vm);
			DEBUG_LOG("heap left pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_left_pointer()));
			DEBUG_LOG("heap right pointer %p\n", (dj_mem_get_base_pointer()+dj_mem_get_right_pointer()));
			DEBUG_LOG("current thread pointer %p\n", vm->currentThread);
			DEBUG_LOG("There are %d live threads\n", dj_vm_countLiveThreads(vm)); */
			//heap_dump(((void *)mem));
	}else { // we have to create a new VM
			//printf("Creating a new VM\n");
	    	vm = dj_vm_create();
	}

#endif

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load the embedded infusions
	//if(!resume_from_hibernation)

	dj_loadEmbeddedInfusions(vm);

	// load application table from storage memory
	app_manager_loadApplicationTable();

	//start digital I/O interface and callbacks
	init_digitalio_interface(digital_io_callback);

#if SERIAL_INPUT
	serial_input_init(serial_input_callback);
#endif

	while (true)
	{
		// start the main execution loop
		if (dj_vm_countLiveThreads(vm)>0)
		{
			nextScheduleTime = dj_vm_schedule(vm);


			//nextScheduleTime = 2147483647;
			DEBUG_LOG("Next time = %ld now %ld\n", nextScheduleTime, dj_timer_getTimeMillis());
			while (vm->currentThread!=NULL){ /* LELE: inserito while per schedulare pi� thread
												nella stessa epoca (in questo modo per� non
												abbiamo preemption ?? */
				if (vm->currentThread->status==THREADSTATUS_RUNNING)
					dj_exec_run(RUNSIZE);
				if(to_init != NULL){// execute the deferred initilization needed by the run-time app loading
					dj_vm_runClassInitialisers(vm, to_init);
					to_init = NULL;
				}
				nextScheduleTime = dj_vm_schedule(vm);

			}

		}
		deltaSleep = (nextScheduleTime - dj_timer_getTimeMillis());
		if(deltaSleep <= 0) deltaSleep = 1;
		DEBUG_LOG("delta time = %ld\n", deltaSleep);
		// can't get PROCESS_YIELD to work, quick hack to wait 1 clock tick
	    etimer_set(&et, (clock_time_t)((deltaSleep*CLOCK_SECOND)/1000));
	    PROCESS_YIELD_UNTIL((etimer_expired(&et) || ev == PROCESS_EVENT_POLL));
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/* This function is called whenever a broadcast message is received. */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	dj_thread *rec_thread;
  	/* Print out a message. */
  packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, 0/* TODO to implement node_id */);
  packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));
  /*printf("broadcast message received from %d.%d with RSSI %d, LQI %u\n",
		  from->u8[0], from->u8[1],
  	      packetbuf_attr(PACKETBUF_ATTR_RSSI),
  	      packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));*/

  rec_thread = dj_vm_getThreadById(dj_exec_getVM(), receiver_thread_id);
  if(rec_thread != nullref){
	  //printf("Thread  id %d status %d\n", rec_thread->id, rec_thread->status);
	  rec_thread->status = THREADSTATUS_RUNNING;
	  // we need to ensure that this thread will take the CPU before other running thread
	  rec_thread->need_resched = 1;
  }
  release_RF(); // release RF lock to allow power manager to shutdown the radio module
  //release_MAC(); // release mac to allow power manger to stop duty cycle.
  process_poll(&darjeeling_process);
}

/* This is where we define what function to be called when a broadcast
   is received. We pass a pointer to this structure in the
   broadcast_open() call below. */
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
/*---------------------------------------------------------------------------*/

/* This function is called for every incoming unicast packet. */
static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	dj_thread *rec_thread;
	rec_thread = dj_vm_getThreadById(dj_exec_getVM(), receiver_thread_id);
	/*printf("unicast packet received from %d.%d with RSSI %d, LQI %u\n",
           from->u8[0], from->u8[1],
           packetbuf_attr(PACKETBUF_ATTR_RSSI),
           packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));*/
	packetbuf_set_attr(PACKETBUF_ADDR_RECEIVER, 0 /* TODO to implement node_id*/);
	packetbuf_set_attr(PACKETBUF_ADDR_SENDER, ((from->u8[1]<<8) + from->u8[0]));

	if(rec_thread != nullref){
		rec_thread->status = THREADSTATUS_RUNNING;
		// we need to ensure that this thread will take the CPU before other running thread
		rec_thread->need_resched = 1;

	}
    release_RF(); // release RF lock to allow power manager to shutdown the radio module
    //release_MAC(); // release mac to allow power manger to stop duty cycle.
	process_poll(&darjeeling_process);

}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
struct broadcast_conn broadcast_network_init(void){
		// save the receiver thread pointer.
		receiver_thread_id = dj_exec_getCurrentThread()->id;
		broadcast_open(&broadcast, 129, &broadcast_call);
		//unicast_open(&unicast, 146, &unicast_callbacks);
        return broadcast;

}
struct unicast_conn unicast_network_init(void){
		// save the receiver thread pointer.
		receiver_thread_id = dj_exec_getCurrentThread()->id;
		unicast_open(&unicast, 146, &unicast_callbacks);
        return unicast;

}

#if SERIAL_INPUT
void serial_input_open(void){
	// save the receiver thread pointer.
	serial_receiver_thread_id = dj_exec_getCurrentThread()->id;
}
#endif

void dj_main_runDeferredInitializer(dj_infusion *infusion){
	to_init = infusion;
}

static void digital_io_callback(uint8_t port){
        //printf("Call port %d\n", port);
        //TODO: wake-up all threads waiting for this interrupt.
        uint16_t sem = (uint16_t)port;
        dj_thread *wake_thread;
        wake_thread = dj_vm_getThreadBySem(dj_exec_getVM(), (sem+1));
        while( wake_thread != NULL){//Increase the port number by 1 to match semaphore number (zero is not allowed)
                printf("w thread %d\n", wake_thread->id);
                wake_thread->status = THREADSTATUS_RUNNING;
                wake_thread->sem_id = 0;
                wake_thread->need_resched = 1;
                wake_thread = dj_vm_getThreadBySem(dj_exec_getVM(), (sem+1));
        }
        process_poll(&darjeeling_process);

}
#if SERIAL_INPUT
static void serial_input_callback(char *line, int line_len){
	dj_thread *rec_thread;
	rec_thread = dj_vm_getThreadById(dj_exec_getVM(), serial_receiver_thread_id);
	serial_buffer = line;
	serial_buffer_len = line_len;
	if(rec_thread != nullref){
		rec_thread->status = THREADSTATUS_RUNNING;
		// we need to ensure that this thread will take the CPU before other running thread
		rec_thread->need_resched = 1;

	}
	process_poll(&darjeeling_process);

}

char *get_serial_buffer(void){
	return serial_buffer;
}
int get_serial_buffer_len(void){
	return serial_buffer_len;
}
#endif

