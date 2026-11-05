#include "core.h"
#include "stm32f4xx.h"
#include <string.h>


volatile uint8_t key1_state = 0;
volatile uint8_t key2_state = 0;

// Это массив шрифтов для CHIP-8. Каждая строчка это 1 символ
const uint8_t fontset[80] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


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
    
    /*
    chip8->keypad[0x2] = IsKeyDown(KEY_TWO);
    chip8->keypad[0x3] = IsKeyDown(KEY_THREE); 
    chip8->keypad[0xC] = IsKeyDown(KEY_FOUR);
    
    chip8->keypad[0x4] = IsKeyDown(KEY_Q);     
    chip8->keypad[0x5] = IsKeyDown(KEY_W);
    chip8->keypad[0x6] = IsKeyDown(KEY_E);     
    chip8->keypad[0xD] = IsKeyDown(KEY_R); 
    
    chip8->keypad[0x7] = IsKeyDown(KEY_A);     
    chip8->keypad[0x8] = IsKeyDown(KEY_S);
    chip8->keypad[0x9] = IsKeyDown(KEY_D);     
    chip8->keypad[0xF] = IsKeyDown(KEY_F);
    
    chip8->keypad[0xA] = IsKeyDown(KEY_Z);     
    chip8->keypad[0x0] = IsKeyDown(KEY_X);
    chip8->keypad[0xB] = IsKeyDown(KEY_C);     
    chip8->keypad[0xE] = IsKeyDown(KEY_V);
    */
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