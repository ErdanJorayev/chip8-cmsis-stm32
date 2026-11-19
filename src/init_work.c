#include "core.h"
#include "stm32f4xx.h"
#include <string.h>


volatile uint8_t key1_state = 0;
volatile uint8_t key2_state = 0;


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
    chip8->keypad[0x1] = key1_state;   
    chip8->keypad[0x4] = key2_state;     
}


void EXTI2_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR2)
    {
        EXTI->PR |= EXTI_PR_PR2; // Сбрасываем флаг прерывания
        
        // Читаем реальное состояние пина PA2 (при подтяжке Pull-Up: 0 — нажато, 1 — отпущено)
        if (!(GPIOA->IDR & GPIO_IDR_ID2)) 
            key1_state = 1; // Кнопка нажата
        else 
            key1_state = 0; // Кнопка отпущена
    }
}

void EXTI3_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR3)
    {
        EXTI->PR |= EXTI_PR_PR3; // Сбрасываем флаг прерывания
        
        // Читаем реальное состояние пина PA3
        if (!(GPIOA->IDR & GPIO_IDR_ID3)) 
            key2_state = 1; // Кнопка нажата
        else 
            key2_state = 0; // Кнопка отпущена
    }
}