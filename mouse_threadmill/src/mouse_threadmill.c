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

#define TICKRATE_HZ1 (10000)
#define MEMORY_CAPACITY 100
#define regulation_step 1
static ADC_CLOCK_SETUP_T ADCSetup;
static uint32_t rotation_counter=0;
static uint16_t sensor_threshold=0x4B0;
static uint32_t rotation_phase=0;
static uint32_t rotation_time=0;
static int rotation_debug_holder[MEMORY_CAPACITY];
static int rotation_debug_holder2[MEMORY_CAPACITY];
static uint32_t desired_rotation_time=150;

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
	int cast;
	cast=(int) data;
	//DEBUGOUT("0x%04x\r\n", data);
	DEBUGOUT("%d\n",cast);
}


void SysTick_Handler(void)
{
	uint16_t dataADC;
	//int i;
	uint32_t currentPWM;
	currentPWM=LPC_PWM1->MR1;

	//Board_LED_Toggle(0);
	Board_LED_Set(0,1);


	//Chip_ADC_SetStartMode(_LPC_ADC_ID, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

	//while (Chip_ADC_ReadStatus(_LPC_ADC_ID, _ADC_CHANNLE, ADC_DR_DONE_STAT) != SET) {}
	/* Read ADC value */
	Chip_ADC_ReadValue(_LPC_ADC_ID, _ADC_CHANNLE, &dataADC);
	/* Print ADC value */
	//App_print_ADC_value(dataADC);
	/*
	if(rotation_time<MEMORY_CAPACITY){
		rotation_debug_holder[rotation_time]=(int) dataADC;
		rotation_time++;
	}
	*/

	rotation_time++;
	//if(rotation_counter<MEMORY_CAPACITY){	//debug line
    if(dataADC>sensor_threshold){
        if(rotation_phase==0){
            	//rotation_debug_holder[rotation_counter]=rotation_time;		//debug line
            	//rotation_debug_holder2[rotation_counter]=dataADC;				//debug line

            	if(currentPWM < (1000-regulation_step) && currentPWM > (regulation_step)){	//debug needed here
            		if(rotation_time>desired_rotation_time){
            			PWM_SetCycle(currentPWM+regulation_step,1000);
            		}else{
            			PWM_SetCycle(currentPWM-regulation_step,1000);
            		}
            	}
            rotation_counter++;
            rotation_time=0;
            rotation_phase=1;
        }

    }else{
    	rotation_phase = 0;
    }

	//}				//debug line

    if(rotation_time==1000){
    	if(currentPWM < (1000-regulation_step)){
    		PWM_SetCycle(currentPWM+10*regulation_step,1000);
    	}
    	rotation_time=0;
    }


	Board_LED_Set(0,0);
}


void PWM_SetCycle(uint32_t Rate, uint32_t CycleLength){
	LPC_PWM1->MR0=CycleLength;	//100 as 100%
	LPC_PWM1->MR1=Rate;	//30% rate

	//writing to the PWM LER register so that the values are transferred from the shadow registers

	LPC_PWM1->LER |= (1 << 1) | (1);
}

void PWM_Init(void){

	//assigning bit powering PWM (the entire peripheral)

	LPC_SYSCTL->PCONP |= (1<<6);

	//assigning bit clocking PWM (the entire peripheral)

	LPC_SYSCTL->PCLKSEL[0] |= ((1<<13) | (1<<12));

	//assigning a bit to select PWM1 on pin P2.0

	LPC_IOCON->PINSEL[4]|=1;
	LPC_IOCON->PINSEL[4] &= ~(1<<1);

	//setting the initial cycle resulting in LOW output

	PWM_SetCycle(0,1000);

	//setting proper bits of PWM1TCR register
	LPC_PWM1->TCR |= (1 << 3) | (1);
	LPC_PWM1->TCR &= ~ (1 << 1);

	//enabling PWM1 output
	LPC_PWM1->PCR |= ( 1<<9 );

}

int main(void) {



	//preparing the chip & board
	SystemCoreClockUpdate();
	Board_Init();
	PWM_Init();
	PWM_SetCycle(400,1000);

	//the actual code
	Chip_ADC_Init(_LPC_ADC_ID, &ADCSetup);
	Chip_ADC_EnableChannel(_LPC_ADC_ID, _ADC_CHANNLE, ENABLE);
	Chip_ADC_SetBurstCmd(_LPC_ADC_ID, ENABLE);
	//int i;

	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

    while(1) {
    	__WFI();
    	/*
    	if(rotation_counter==MEMORY_CAPACITY){			//not rotation_cycle?
    		for(i=0;i<MEMORY_CAPACITY;i++){
    		DEBUGOUT("%d \n",rotation_debug_holder[i]);//,rotation_debug_holder2[i]);
    		}
    		rotation_counter++;
    	}
    	*/
    }
    return 0 ;
}


