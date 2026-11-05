#include <string.h>
#include "stm32f4xx.h"
#include "ssd1306.h"
#include "core.h"
#include "pong.h"
#include "inits.h"

static void WorkChip8(Chip8 * chip8);


int main(void)
{
    // Включаем светодиод на PC13 (для проверки, что МК вообще ожил)
    // Плюс в целом удобно когда переводишь в режим прошивки (светодиод выключится)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER13_0; // Output
    GPIOC->BSRR = GPIO_BSRR_BR13;         // Зажечь светодиод
    //___________________________________________________________________________


    // Инициализация железо
    GPIO_init();
    EXTI_init();
    SSD1306_Init();    // Инициализация модуля экрана по интерфейсу I2C1

    Chip8 core;

    Chip8_Init(&core); // Инициализация ядра

    // Кидаем нашу игру в память CHIP-8
    memcpy(core.memory + START_ADDRESS, pong, PONG_SIZE);

    while (1)
    {
        SSD1306_Clear(); // Очистка кадра буфера
        WorkChip8(&core);
        SSD1306_Update();
    }
}

// Work CHIP-8
void WorkChip8(Chip8 * chip8)
{
    HandleInput(chip8);

    // Выполняем 8 команд чипа
    for (size_t i = 0; i < 8; i++) 
        Chip8_Cycle(chip8);
        
    // Уменьшаем таймер ожидание
    if (chip8->delay_timer > 0) 
        chip8->delay_timer--;

    // Уменьшаем таймер звука
    if (chip8->sound_timer > 0) 
        chip8->sound_timer--;

    // Рисуем в экране
    for (size_t y = 0; y < 32; y++) 
    {
        for (size_t x = 0; x < 64; x++)
        { 
            if (chip8->video[y * 64 + x] != 0) 
            {
                uint8_t screen_x = x * 2;
                uint8_t screen_y = y * 2;

            // Рисуем квадрат 2x2 пикселя
                SSD1306_DrawPixel(screen_x,     screen_y,     WHITE);
                SSD1306_DrawPixel(screen_x + 1, screen_y,     WHITE);
                SSD1306_DrawPixel(screen_x,     screen_y + 1, WHITE);
                SSD1306_DrawPixel(screen_x + 1, screen_y + 1, WHITE);
            }
        }
    }    
}


