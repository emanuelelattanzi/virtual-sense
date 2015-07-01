/*
 *	java_lang_String.c
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

#include <string.h>


#include "common/array.h"
#include "common/heap/heap.h"
#include "common/execution/execution.h"

#include "base_definitions.h"

// int java.lang.String.length()
void java_lang_String_int_length()
{
	char *str = REF_TO_VOIDP(dj_exec_stackPopRef());
	dj_exec_stackPushInt(strlen(str));
}

// java.lang.String java.lang.String.concat(java.lang.String[])
void java_lang_String_java_lang_String_concat_java_lang_String__()
{
	uint16_t i, arrayLength, totalLength, len;
	char * ret, * ptr;

	// pop array
	dj_ref_array * arr = (dj_ref_array*)REF_TO_VOIDP(dj_exec_stackPopRef());
	arrayLength = arr->array.length;

	// calculate the total string size
	totalLength = 0;
	for (i=0; i<arrayLength; i++)
	{
		char * str = (char *)REF_TO_VOIDP(arr->refs[i]);
		totalLength += strlen(str);
	}

	// allocate the new string
	ptr = ret = (char *)dj_mem_alloc(totalLength + 1, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));

	if (ptr==NULL)
	{
    	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
    	return;
	}

	for (i=0; i<arrayLength; i++)
	{
		// add the parameter string
		char * str = (char*)REF_TO_VOIDP(arr->refs[i]);
		len = strlen(str);
		memcpy(ptr, str, len);
		ptr += len;
	}

	// add the trailing zero
	*ptr = 0;

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));

}

// java.lang.String java.lang.String.join(java.lang.String, java.lang.String[])
void java_lang_String_java_lang_String_join_java_lang_String_java_lang_String__()
{
	uint16_t i, arrayLength, joinLength, totalLength, len;
	char * ret, * ptr;

	// pop array
	dj_ref_array * arr = (dj_ref_array*)REF_TO_VOIDP(dj_exec_stackPopRef());
	arrayLength = arr->array.length;

	// pop the join string
	char * joinStr = (char*)REF_TO_VOIDP(dj_exec_stackPopRef());
	joinLength = strlen(joinStr);

	// calculate the total string size
	totalLength = (arrayLength==0)?0:(arrayLength-1) * joinLength;
	for (i=0; i<arrayLength; i++)
	{
		char * str = (char *)REF_TO_VOIDP(arr->refs[i]);
		totalLength += strlen(str);
	}

	// allocate the new string
	ptr = ret = (char *)dj_mem_alloc(totalLength + 1, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));

	if (ptr==NULL)
	{
    	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
    	return;
	}

	for (i=0; i<arrayLength; i++)
	{
		// add the join string
		if (i>0)
		{
			memcpy(ptr, joinStr, joinLength);
			ptr += joinLength;
		}

		// add the parameter string
		char * str = (char*)REF_TO_VOIDP(arr->refs[i]);
		len = strlen(str);
		memcpy(ptr, str, len);

		ptr += len;
	}

	// add the trailing zero
	*ptr = 0;

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));

}

// boolean java.lang.String.equals(java.lang.String)
void java_lang_String_boolean_equals_java_lang_String()
{
	char *str1 = REF_TO_VOIDP(dj_exec_stackPopRef());
	char *str2 = REF_TO_VOIDP(dj_exec_stackPopRef());

	dj_exec_stackPushShort(strcmp(str1,str2)==0);
}
void java_lang_String_java_lang_String___split_char(){
	dj_ref_array * ref;

	char separator = (char)dj_exec_stackPopShort();
	int i = 0;
	int j = 0;
	int last_index = -1;
	int start = 0;
	int end = 0;
	int separator_count = 0;
	// pop the split string
	char * splitStr = (char*)REF_TO_VOIDP(dj_exec_stackPopRef());
	int len = strlen(splitStr);
	for(i = 0; i < len; i++){
		if((*((char *)(splitStr+i))) == separator){
			if(last_index +1 != i){
				separator_count++;

			}
			last_index = i;
		}
	}
	if(last_index == (i-1))
		separator_count --;
	if(separator_count > 0){
		//printf("separator has been found %d time\n",separator_count);
	// create the string array
		 ref = dj_ref_array_create(dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String), (uint16_t)(separator_count+1));
		 start = end = 0;
		 separator_count = 0;
		 last_index = -1;
		 for(i = 0; i < len; i++){
		 	if((*((char *)(splitStr+i))) == separator){
		 		if(last_index +1 != i){ // to remove multiple contiguous separator
		 			end = i-1;

		 			//allocate string
		 			ref->refs[separator_count] = (ref_t)dj_mem_alloc(end - start +2 , dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));
		 			memcpy((char *)ref->refs[separator_count], (char *)(splitStr+start), (end - start + 1));
		 			//printf("New token start %d end %d\n", start, end);
		 			separator_count++;
		 			start = i+1;
		 		}else
		 			start++;
		 		last_index = i;
		 	}
		 }
		 if(last_index != (i-1)){
			 end = len-1;
			 // we need to copy the last token (after separator)
			 ref->refs[separator_count] = (ref_t)dj_mem_alloc(end - start +2, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));
			 memcpy((char *)ref->refs[separator_count], (char *)(splitStr+start), (end - start + 1));
			 // printf("Last token start %d end %d\n", start, end);

		 }
	}
	dj_exec_stackPushRef(VOIDP_TO_REF(ref));

}

// byte[] java.lang.String.getBytes(java.lang.String)
void java_lang_String_byte___getBytes(){
	char *str = REF_TO_VOIDP(dj_exec_stackPopRef());
	int len = strlen(str);
	dj_int_array * ret = dj_int_array_create(T_BYTE, len);

	int i = 0;
	for(i = 0; i < len; i++)
		ret->data.bytes[i] = (int8_t)str[i];

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}


// char java_lang_String_char_charAt_int
void java_lang_String_char_charAt_int() {
	char *str = REF_TO_VOIDP(dj_exec_stackPopRef());
	int index = dj_exec_stackPopInt();
	int len = strlen(str);

	if(index > len - 1) {
		dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
		return;
	}

	dj_exec_stackPushShort((uint16_t)str[index]);
}



