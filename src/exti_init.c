#include "stm32f4xx.h"

void EXTI_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;                                     
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI2 | SYSCFG_EXTICR1_EXTI3);      
    SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PA | SYSCFG_EXTICR1_EXTI3_PA); 
    
    // Настройка EXTI для пина PA2 (Кнопка 1)
    EXTI->FTSR |= EXTI_FTSR_TR2; // Спадающий фронт (нажатие)
    EXTI->RTSR |= EXTI_RTSR_TR2; // Возрастающий фронт (отпускание)
    EXTI->IMR  |= EXTI_IMR_MR2;   
    EXTI->PR   =  EXTI_PR_PR2;      
    NVIC_EnableIRQ(EXTI2_IRQn);  

    
    // Настройка EXTI для пина PA3 (Кнопка 2)
    EXTI->FTSR |= EXTI_FTSR_TR3; // Спадающий фронт (нажатие)
    EXTI->RTSR |= EXTI_RTSR_TR3; // Возрастающий фронт (отпускание)
    EXTI->IMR  |= EXTI_IMR_MR3;  
    EXTI->PR   |= EXTI_PR_PR3;   
    NVIC_EnableIRQ(EXTI3_IRQn);  
}