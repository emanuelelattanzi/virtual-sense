/*
 *      javax_virtualsense_sensosors_Light.c
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
 *
 *      This file is part of VirtualSense.
 *
 *      VirtualSense is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      VirtualSense is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Light access methods.
 *
 * @author Matteo Dromedari
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"


//public static native short getBoardValue();
void javax_virtualsense_sensors_Light_short_getValue()
{
        const short LIGHTCHANNEL = 0;
        const short REF_2_5 = 2;

        // Enable light sensor
        P2DIR  &= ~BIT6;
        P2OUT  |= BIT6;

        // Set high gain
        P5DIR  &= ~(BIT6 + BIT7);
        P5OUT  |= BIT6;
        P5OUT  &= ~BIT7;

        // Bh1620fvc spec => H Gain (GC2:0  GC1:1) => Viout = 0.57e-6 * Ev * R1Ev = Viout / (0.57e-6 * R1)
        // R1 = viout / (0.57e-6 * Ev) => 2.5 / (0.57e-6 * 1000) = 4K4
        // Ev = Viout / (0.57e-6 * R1) => (mViout * 1000) / 2508 [Lux]

        // Read ADC light channel (adc channel 1 with referiment 2.5V)
        uint32_t mViout = (uint32_t)read_channel_intref(LIGHTCHANNEL, REF_2_5);

        // Convert for get illuminance in Lux
        dj_exec_stackPushShort((short)((mViout * 1000) / 2508));

        // Disable light sensor
        //P2OUT &= ~BIT6;

}



