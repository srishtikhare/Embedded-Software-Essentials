/*
 * memory.h
 *
 *  Created on: Nov 28, 2016
 *      Author: shva9978
 */

#ifndef SOURCES_MEMORY_H_
#define SOURCES_MEMORY_H_


	#include<stdio.h>
	#include<stdint.h>

	void my_memmove(uint8_t *source, uint8_t *destination, uint32_t length);
	void my_memzero(uint8_t *src, uint32_t length);


#endif /* SOURCES_MEMORY_H_ */
