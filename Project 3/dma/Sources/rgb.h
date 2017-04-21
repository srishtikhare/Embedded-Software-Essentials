/*
 * rgb.h
 *
 *  Created on: Nov 28, 2016
 *      Author: shva9978
 */

#ifndef SOURCES_RGB_H_
#define SOURCES_RGB_H_
typedef enum
{
	OFF,
	CYAN,
    MAGENTA,
	YELLOW,
	RED,
    GREEN,
	BLUE,
	WHITE
}LED;
void LED_Initialize();
//void LED_Modify(uint8_t input);
//void Brightness_Control(LED Color_Number, uint16_t Bright);

#endif
