#include "core.h"
#include "stm32f4xx.h"
#include <string.h>
#include "pong.h"

static volatile bool key_state[4] = {false, false, false, false}; // Массив для состояние кнопок

void Chip8_Init(Chip8 * chip8) 
{
    // Зануляем все поля структуры CHIP-8
    memset(chip8, 0, sizeof(*chip8)); 

    // С этого адреса начинается программа для CHIP-8
    chip8->pc = START_ADDRESS;        

    // Копируем шрифтовой набор в ОЗУ
    memcpy(&chip8->memory[FONTSET_START_ADDRESS], fontset, sizeof(fontset));
}


void HandleInput(Chip8 * chip8) 
{
    chip8->keypad[0x4] = key_state[0];   
    chip8->keypad[0x5] = key_state[1];
    chip8->keypad[0x6] = key_state[1];

}

void EXTI9_5_IRQHandler(void)
{
   
    if (EXTI->PR & EXTI_PR_PR5) 
    {
        EXTI->PR = EXTI_PR_PR5;
        key_state[0] = !(GPIOA->IDR & GPIO_IDR_ID5);
    }
    if (EXTI->PR & EXTI_PR_PR6) 
    {
        EXTI->PR = EXTI_PR_PR6;
        key_state[1] = !(GPIOA->IDR & GPIO_IDR_ID6);
    }
    if (EXTI->PR & EXTI_PR_PR7) 
    {
        EXTI->PR = EXTI_PR_PR7;
        key_state[2] = !(GPIOA->IDR & GPIO_IDR_ID7);
    }
    if (EXTI->PR & EXTI_PR_PR8) 
    {
        EXTI->PR = EXTI_PR_PR8;
        key_state[3] = !(GPIOA->IDR & GPIO_IDR_ID8);
    }
}

