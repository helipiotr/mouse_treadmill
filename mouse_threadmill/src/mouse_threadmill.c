/*
===============================================================================
 Name        : mouse_threadmill.c
 Author      : Piotr Wierzba
 Version     : 0.1
 Copyright   :
 Description : This is a prototype for the future threadmill controller
===============================================================================
*/



#include "board.h"


#ifdef BOARD_NXP_LPCXPRESSO_1769
#define _ADC_CHANNLE ADC_CH0
#else
#define _ADC_CHANNLE ADC_CH2
#endif
#define _LPC_ADC_ID LPC_ADC

#define TICKRATE_HZ1 (10)
static ADC_CLOCK_SETUP_T ADCSetup;
//static volatile uint8_t Burst_Mode_Flag = 0;
/*
 * #include "adc_17xx_40xx.h"
* #include "sysctl_17xx_40xx.h"
* #include "iocon_17xx_40xx.h"
* #include "pwm_17xx_40xx.h"
* #include "cr_section_macros.h"
* #include "mcpwm_17xx_40xx.h"
* #include "LPC17xx.h"	add "${workspace_loc:/CMSISv2p00_LPC17xx/inc}" to link it properly,
* however, you need to use it consistently (remove unnecessary includes) since it might cause
* double definitions
*/

static void App_print_ADC_value(uint16_t data)
{
	DEBUGOUT("ADC value is : 0x%04x\r\n", data);
}

void SysTick_Handler(void)
{
	uint16_t dataADC;

	Board_LED_Toggle(0);

	Chip_ADC_SetStartMode(_LPC_ADC_ID, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

	while (Chip_ADC_ReadStatus(_LPC_ADC_ID, _ADC_CHANNLE, ADC_DR_DONE_STAT) != SET) {}
	/* Read ADC value */
	Chip_ADC_ReadValue(_LPC_ADC_ID, _ADC_CHANNLE, &dataADC);
	/* Print ADC value */
	App_print_ADC_value(dataADC);
}

void PWM_Init(void){

	//assigning bit powering PWM1

	LPC_SYSCTL->PCONP |= (1<<6);

	//assigning bit clocking PWM1 (the entire peripheral)

	LPC_SYSCTL->PCLKSEL[0] |= ((1<<13) | (1<<12));

	//assigning a bit to select PWM1 on pin P2.0

	LPC_IOCON->PINSEL[4]|=1;
	LPC_IOCON->PINSEL[4] &= ~(1<<1);

	//setting the match registers to generate proper PWM waveforms

	LPC_PWM1->MR0=1000;	//100 as 100%
	LPC_PWM1->MR1=300;	//30% rate

	//setting proper bits of PWM1TCR register
	LPC_PWM1->TCR |= (1 << 3) | (1);
	LPC_PWM1->TCR &= ~ (1 << 1);

	//enabling PWM1 output
	LPC_PWM1->PCR |= ( 1<<9 );

	//writing to the PWM LER register so that the values are transferred from the shadow registers

	LPC_PWM1->LER |= (1 << 1) | (1);

}

int main(void) {

	//preparing the chip & board
	SystemCoreClockUpdate();
	Board_Init();
	PWM_Init();

	//the actual code




	Chip_ADC_Init(_LPC_ADC_ID, &ADCSetup);
	Chip_ADC_EnableChannel(_LPC_ADC_ID, _ADC_CHANNLE, ENABLE);
	Chip_ADC_SetBurstCmd(_LPC_ADC_ID, DISABLE);

	/*
	uint32_t initial_val,new_val;
	uint32_t mask= (1<<25);
	initial_val = LPC_SYSCTL->PCONP;
	LPC_SYSCTL->PCONP |= mask;
	//LPC_SYSCTL->PCONP |= (1<<3);
	//int cast = 3;
	//cast= (int ) test_var;
	new_val= LPC_SYSCTL->PCONP;
	DEBUGOUT("The old value is %d, and the new one %d \n",initial_val, new_val);
	 */



	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);


    while(1) {
    	__WFI();
    }
    return 0 ;
}
