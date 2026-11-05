#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define FONTSET_START_ADDRESS  0x50 // Адрес где начинаются шрифты
#define START_ADDRESS  0x200        // Адрес где начинаются программы


typedef struct 
{
    uint8_t registers[16];   // Регистры общего назначения V0 - VF
    uint8_t memory[4096];    // Оперативная память (4 КБ ОЗУ)
    uint16_t index;          // Индексный регистр I (для адресации в памяти)
    uint16_t pc;             // Счетчик команд (Program Counter — адрес текущей инструкции)
    uint16_t stack[16];      // Стек вызовов подпрограмм
    uint8_t sp;              // Указатель стека (Stack Pointer — индекс вершины стека)
    uint8_t delay_timer;     // Таймер задержки (уменьшается с частотой 60 Гц)
    uint8_t sound_timer;     // Звуковой таймер (пока > 0, звучит сигнал; 60 Гц)
    bool keypad[16];         // Состояние 16-клавишной клавиатуры (0-F, нажата/отпущена)
    uint8_t video[64 * 32];  // Видеопамять / буфер экрана (монохромная матрица 64x32 пикселя)
    uint16_t opcode;         // Текущий код операции (2-байтовая инструкция)
} Chip8;

// Функция работы ядра CHIP-8
void Chip8_Cycle(Chip8 *chip8);

#endif


