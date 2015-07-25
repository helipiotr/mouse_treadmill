/*
 * pwm_17xx_40xx.h
 *
 *  Created on: 24 lip 2015
 *      Author: Piotr
 */

#ifndef __PWM_17XX_40XX_H_
#define __PWM_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
  __I  uint32_t CR1;
  __I  uint32_t CR2;
  __I  uint32_t CR3;
       uint32_t RESERVED0;
  __IO uint32_t MR4;
  __IO uint32_t MR5;
  __IO uint32_t MR6;
  __IO uint32_t PCR;
  __IO uint32_t LER;
       uint32_t RESERVED1[7];
  __IO uint32_t CTCR;
} LPC_PWM_T;

#ifdef __cplusplus
}
#endif

#endif /* __PWM_17XX_40XX_H_ */
