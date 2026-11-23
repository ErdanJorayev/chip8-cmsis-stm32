#include "stm32f4xx.h"

void EXTI_init(void)
{
    // Тактирование SYSCFG для привязки линий EXTI к портам GPIO
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Линии EXTI5..7 и отдельно для EXTI8
    SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI5 | SYSCFG_EXTICR2_EXTI6 |
                           SYSCFG_EXTICR2_EXTI7);
    SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI8;

    const uint32_t btn_mask = EXTI_FTSR_TR5 | EXTI_FTSR_TR6 |
                              EXTI_FTSR_TR7 | EXTI_FTSR_TR8;

    EXTI->FTSR |= btn_mask; // Спадающий фронт (нажатие на GND)
    EXTI->RTSR |= btn_mask; // Возрастающий фронт (отпускание)
    EXTI->IMR  |= btn_mask; // Разрешаем прерывания линий 5..8
    EXTI->PR    = btn_mask; // Сбрасываем возможные висячие флаги (W1C)

    NVIC_EnableIRQ(EXTI9_5_IRQn);
}