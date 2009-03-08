/******************************************************************************
 *   FullFAT - Embedded FAT File-System
 *
 *   Provides a full, thread-safe, implementation of the FAT file-system
 *   suitable for low-power embedded systems.
 *
 *   Written by James Walmsley, james@worm.me.uk, www.worm.me.uk/fullfat/
 *
 *   Copyright 2009 James Walmsley
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 *   Commercial support is available for FullFAT, for more information
 *   please contact the author, james@worm.me.uk
 *
 *   Removing the above notice is illegal and will invalidate this license.
 *****************************************************************************/

/**
 *	@file		ff_memory.c
 *	@author		James Walmsley
 *	@ingroup	MEMORY
 *
 *	@defgroup	MEMORY	FullFAT Memory Access Routines
 *	@brief		Handles memory access in a portable way.
 *
 *	Provides simple, fast, and portable access to memory routines.
 *	These are only used to read data from buffers. That are LITTLE ENDIAN
 *	due to the FAT specification.
 *
 **/

#include "ff_memory.h"
#include <string.h>

#ifdef FF_LITTLE_ENDIAN
/* This exists because some systems don't have byte access, in this case make this return the byte of interest */
FF_T_UINT8 FF_getChar(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT8) (pBuffer[offset]);
}

FF_T_UINT16 FF_getShort(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT16) (pBuffer[offset] & 0x00FF) | ((FF_T_UINT16) (pBuffer[offset+1] << 8) & 0xFF00);
}

FF_T_UINT32 FF_getLong(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT32) (pBuffer[offset] & 0x000000FF) | ((FF_T_UINT32) (pBuffer[offset+1] << 8) & 0x0000FF00) | ((FF_T_UINT32) (pBuffer[offset+2] << 16) & 0x00FF0000) | ((FF_T_UINT32) (pBuffer[offset+3] << 24) & 0xFF000000);
}

#endif

#ifdef FF_BIG_ENDIAN
/*
	TODO: These need converting to BIG ENDIAN!!!
*/
FF_T_UINT8 FF_getChar(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT8) (pBuffer[offset]);
}

FF_T_UINT16 FF_getShort(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT16) (pBuffer[offset] & 0x00FF) | ((FF_T_UINT16) (pBuffer[offset+1] << 8) & 0xFF00);
}

FF_T_UINT32 FF_getLong(FF_T_UINT8 *pBuffer, FF_T_UINT16 offset) {
	return (FF_T_UINT32) (pBuffer[offset] & 0x000000FF) | ((FF_T_UINT32) (pBuffer[offset+1] << 8) & 0x0000FF00) | ((FF_T_UINT32) (pBuffer[offset+2] << 16) & 0x00FF0000) | ((FF_T_UINT32) (pBuffer[offset+3] << 24) & 0xFF000000);
}

#endif

//strtok() is not threadsafe, here's one that is! (but it works differently!!)
// returns the iteration value that should passed into the iteration arg
// 0 means its finished!
// Token is a buffer, of the maxlength of a token!


char *FF_strtok(char *string, unsigned short *iteration, unsigned short *modded) {
	int i,x;
	/* Find working token!! (iteration number!!) */
	i = strlen(string);
	
	/*
		Strips Trailing / or \ from a path. 
		Otherwise a NULL pointer may not be returned.
	*/
	if(string[i-1] == '\\' || string[i-1] == '/') {
		string[i-1] = '\0';
	}
	
	if(*modded == 1) {
		return NULL;
	}

	i = 0;

	if(string[i] == '\\' || string[i] == '/') {
		i++;
	}

	for(x = *iteration; x > 0; i++) {
		if(string[i] == '\0') {
			x--;
		}
	}

	for(x = i; string[x] != '\0'; x++) {
		if(string[x] == '\\' || string[x] == '/') {
			*iteration += 1;
			string[x] = '\0';
			return &string[i];
		}
	}

	if(string[x] == '\0') {
		*modded = 1;
		return &string[i];
	}

	return NULL; // A default case!
}
