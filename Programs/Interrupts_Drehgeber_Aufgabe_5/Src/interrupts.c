/**
 *******************************************************************
  * @file interrupts.c
  * @author Ozan
  * @date January 2026
  * @brief This module implements interrupts for the AUX pins GPIOG 0 and 1
 *******************************************************************
*/

#include "interrupts.h"
#include "ledOutputs.h"
#include "timer.h"
#include "stm32f4xx.h"

static volatile Direction* isr_dir = 0;
static volatile int32_t* isr_pos = 0;
static volatile uint32_t* isr_iTimr = 0;

static const Direction dirField[4][4]=
{   {IDLE, REV, FWD, ERR},
    {FWD, IDLE, ERR, REV},
    {REV, ERR, IDLE, FWD},
    {ERR, FWD, REV, IDLE}};

static Phase oldPhase = 0;

void sendAdresses(volatile Direction* pdir, volatile int32_t* ppos, volatile uint32_t* piTmr){
    isr_dir = pdir;
    isr_pos = ppos;
    isr_iTimr = piTmr;
}

static inline Direction getDirection(uint8_t phase){
    phase &= 0x03U;
    Direction dir = dirField[oldPhase][phase];
    oldPhase = phase;
    return dir;
}

void setOldPhase(uint8_t phase){
    oldPhase = phase & 0x03U;
}

uint8_t getOldPhase(){
    return oldPhase;
}

static inline void myISR(void){
    if(!isr_dir || !isr_pos || !isr_iTimr){//wenn min 1 null pointer -> return
        return;
    }
    *isr_iTimr = getTimeStamp();
    *isr_dir = getDirection(GPIOG->IDR & 0x03U);
    
    if(*isr_dir == FWD){
        (*isr_pos)++;

    } else if(*isr_dir == REV){
         (*isr_pos)--;

    }
}

void initInterrupts(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; //Clcok for GPIO Port G
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //System conf. Clock enable

    //G0 und G1 auf MODER input setzen
    GPIOG->MODER = (GPIOG->MODER & ~(0x03U << 2*0));//Pin 0 input
    GPIOG->MODER = (GPIOG->MODER & ~(0x03U << 2*1));//Pin 1 input

    //Routing Pin 0 of Port G -> EXTI0
    SYSCFG->EXTICR[0] &= ~(0x0f << (4*0));//Pin 0, Remove old selection
    SYSCFG->EXTICR[0] |= 0x06 << (4*0); //0x00 Selecht Port A, 0x01: Select port B..., 0x06: Select Port G, ...

    //Routing Pin 1 of Port G -> EXTI1
    SYSCFG->EXTICR[0] &= ~(0x0f << (4*1));//Pin 1, Remove old selection
    SYSCFG->EXTICR[0] |= 0x06 << (4*1); //0x00 Selecht Port A, 0x01: Select port B..., 0x06: Select Port G, ...

    //Definiere Events für INT0 und unmask IRQ0
    EXTI->RTSR |= (1U<<0); //select rising trigger for INT0
    EXTI->FTSR |= (1U<<0); //select falling trigger for INT0
    EXTI->IMR |= (1U<<0); //Unmask INT0

    //Definiere Event für INT1 und unmask IRQ1
    EXTI->RTSR |= (1U<<1); //select rising trigger for INT1
    EXTI->FTSR |= (1U<<1); //select falling trigger for INT1
    EXTI->IMR |= (1U<<1); //Unmask INT1

    //NVIC Prioritäten EXTI0 und EXTI 1
    NVIC_SetPriority(EXTI0_IRQn, 0); //Setup EXTI0 auf höchste Prio.
    NVIC_SetPriority(EXTI1_IRQn, 0); //Setup EXTI1 auf höchste Prio.

    NVIC_EnableIRQ(EXTI0_IRQn); //Enable EXTI0
    NVIC_EnableIRQ(EXTI1_IRQn); //Enable EXTI1
}

void EXTI0_IRQHandler(void){
    EXTI->PR = (1U<<0);
    myISR();
}

void EXTI1_IRQHandler(void){
    EXTI->PR = (1U<<1);
    myISR();
}