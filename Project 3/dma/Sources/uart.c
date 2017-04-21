/*
 * uart.c
 *
 *  Created on: Nov 28, 2016
 *      Author: shva9978
 */


/*

#include "MKL25Z4.h"
#include<stdio.h>
#include <math.h>
#include <string.h>
//#include <log.h>
#include"msgcmd.h"
#include "rgb.h"


int flag=0;
int flag1=0;
uint8_t arr[10];
uint8_t array1[15]="Checksum error";

 int counter=0;
int counter1=0;
CI_Msg new;

int Bright=500;

void delay()
{
	int i;
	for(i=0;i<65535;i++);
	for(i=0;i<65535;i++);
}

void LED_Initialize()
{
    SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK;    //initialize TPM2 for red and green and TPM0 for blue
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM0_MASK;
    SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(1);


    TPM0_BASE_PTR->SC = TPM_SC_CMOD(1) | TPM_SC_PS(7);
    TPM0_BASE_PTR->MOD = 1875;

    TPM2_BASE_PTR->SC = TPM_SC_CMOD(1) | TPM_SC_PS(7);  // initialize the mod data to 1875
    TPM2_BASE_PTR->MOD = 1875;

    SIM_BASE_PTR->SCGC5 |=  0x400;     //define clock sources for the TPM mode
    SIM_BASE_PTR->SCGC5 |= 0x1000;

    PORTB_BASE_PTR->PCR[18] = PORTB_BASE_PTR->PCR[19] = PORT_PCR_MUX(3);
    PORTD_BASE_PTR->PCR[1]=PORT_PCR_MUX(4);

      TPM2_BASE_PTR->CONTROLS[0].CnSC =  0x20 | 0x4;
      TPM2_BASE_PTR->CONTROLS[1].CnSC =  0x20 | 0x4; ;
      TPM0_BASE_PTR->CONTROLS[1].CnSC =  0x20 | 0x4; ;
}

void uart_init()//uint32_t baudRate)
{
	uint16_t baud_divisor;

    __disable_irq();
	//Sets gate clock  for PORTA
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Enables clock for UART0; source as MCGCLKFLL
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Sets for UART0 mode
	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);

	// Disables tx and rx before editing registers
	UART0_C2=0x00;
	UART0_C1=0x00;


	UART0_BDH |= 0x00;//(baud_divisor >> 8) & UART0_BDH_SBR_MASK;
	UART0_BDL |= 0x17;//(baud_divisor & UART0_BDL_SBR_MASK);


	//UART_C2_REG(UART0_BASE_PTR) |= UART_C2_TIE_MASK;//Setup receiver interrupt
	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;
	UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_RE_MASK );

	__enable_irq();

	//Enabling NVIC and CPU interrupts
	NVIC_EnableIRQ(UART0_IRQn);
	//Switch on transmitter and receiver
}

void structure_init(CI_Msg *msg){

	msg->command=arr[0]-48;
	msg->length = arr[1]-48;
	msg->data = arr[2]-48;
	msg->checksum =arr[3]-48;
	if(((msg->command)+(msg->length)+(msg->data))!=msg->checksum)
	{
		flag1=1;
		counter1=0;
		counter=0;
		UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK );
		UART_C2_REG(UART0_BASE_PTR) |= UART_C2_TIE_MASK;


	}
	else{
	Decode_CI_Msg(&new);
	counter=0;
	}
	UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;

	//NVIC_EnableIRQ(UART0_IRQn);
}

void Decode_CI_Msg(CI_Msg * msg)
{
	switch(msg->data)
	{
	case LED_RED: Set_red_LED();
		break;
	case LED_BLUE:Set_blue_LED();
		break;
	case LED_GREEN:Set_green_LED();
		break;
	case LED_DANCE:Set_LED_Dance();
		break;
	}
}
void Set_red_LED()
{
	 	    TPM2_BASE_PTR->CONTROLS[1].CnV = 0;
	 	    TPM0_BASE_PTR->CONTROLS[1].CnV = 0;
	 	    TPM2_BASE_PTR->CONTROLS[0].CnV = Bright;
}


void Set_blue_LED()
{

			TPM2_BASE_PTR->CONTROLS[1].CnV = 0;
			TPM2_BASE_PTR->CONTROLS[0].CnV = 0;
			TPM0_BASE_PTR->CONTROLS[1].CnV = Bright;

}

void Set_green_LED()
{
				TPM2_BASE_PTR->CONTROLS[1].CnV = Bright;
			 	TPM2_BASE_PTR->CONTROLS[0].CnV = 0;
			 	TPM0_BASE_PTR->CONTROLS[1].CnV = 0;
}
void Set_LED_Dance()
{
	//UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;
	//while(!(UART0_S1 & UART_S1_RDRF_MASK));
	//int received
		while(UART0_D!='s')
		{
			Set_red_LED();
			delay();
			Set_blue_LED();
			delay();
			Set_green_LED();
			delay();
		}
//		if(UART0_D=='s')
//		{
//			Bright=0;
//		Set_red_LED();
//		}
}


void UART0_IRQHandler()
{
	__disable_irq();
	uint8_t status;
	char received;
	status = UART0_S1;
//	if(status & UART_S1_RDRF_MASK)
//	{
////		if(UART0_D!='\r')
////		{
////			*(arr+counter)= UART0_D;
////			counter++;
////		}
//		if(UART0_D=='\r')
//			UART0_S1=status;
//	}
//	//if(status & UART_S1_TDRE_MASK)
	if(status & UART_S1_RDRF_MASK)
	{
		received= UART0_D;
		if(received!='\r')
		{
			*(arr+counter)= received;
			counter++;
		}
		else
		{
			UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_RIE_MASK);
			//NVIC_DisableIRQ(UART0_IRQn);
			flag=1;
			received=UART0_D;
		}
	}
	if((status & UART_S1_TDRE_MASK) && (flag1==1))
	{
		if((*(array1+counter1))!='\0')
		{
			UART0_D = *(array1 + counter1);
			counter1++;
		}
		else
		{
			UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK);
			UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TIE_MASK);
			//counter1 = 0;
			//counter=0;
				//UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;
			flag1=0;
		}
	}
	__enable_irq();
}

int main()
{
	LED_Initialize();
	uart_init();
	int m;
	while(1)
	{
		if(flag==1)
		{
			structure_init(&new);
			flag=0;
		}
	}
	return 0;
}

*/
