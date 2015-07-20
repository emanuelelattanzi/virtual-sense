/*
 *	javax_darjelling_sensors_Temperature.c
 *
 *	TODO: new Copyright (c) 2008 CSIRO, Delft University of Technology.
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
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
//#include "common/heap/heap.h"
//#include "common/djtimer.h"

// int java.io.OutputStream.write(int i)
void java_io_OutputStream_void_write_int()
{
	printf("%ld", dj_exec_stackPopInt());
}

void java_io_OutputStream_void_write_short()
{
	printf("%d", dj_exec_stackPopShort());
}

// short java.io.OutputStream.write(String s)
void java_io_OutputStream_void_write_java_lang_String()
{
	char *str = REF_TO_VOIDP(dj_exec_stackPopRef());
		printf("%s", str);
}
