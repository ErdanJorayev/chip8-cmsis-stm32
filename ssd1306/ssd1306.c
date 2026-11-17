#include "ssd1306.h"
#include "stm32f4xx.h"


// Буфер кадра: 128x64 пикселей / 8 бит = 1024 байта
static uint8_t frame_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Команды инициализации экрана
static const uint8_t INIT_CMDS[] = 
{
    0xAE,       // Включение экрана
    0x20, 0x00, // Horizontal Addressing Mode
    0x8D, 0x14, // Charge Pump Enable (7.5V)
    
    // --- Поворот экрана на 180 градусов ---
    0xA1,       // Set Segment Re-map (X: справа налево)
    0xC8,       // Set COM Output Scan Direction (Y: снизу вверх)
    // ------------------------------------
    
    0xAF        // Display ON
};
// Команды сброса указателя адреса (на верхний левый угол 0,0)
static const uint8_t RESET_ADDR_CMDS[] = 
{
    0x21, 0, 127, // Set Column Range (0..127)
    0x22, 0, 7    // Set Page Range (0..7)
};

// Отправка команд и данных(универсальная функция)
static void SSD1306_WriteBuffer(uint8_t control_byte, const uint8_t *data, uint16_t count) 
{
    // Генерация сигнала START
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Адрес экрана и указание на запись
    I2C1->DR = 0x78; 

    // Сброс ADDR
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; 

    // Отправка управляющего байта (0x00 для команд, 0x40 для данных)
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = control_byte; 

    // Заталкиваем весь массив данных в цикле
    for (uint16_t i = 0; i < count; i++) 
    {
        while (!(I2C1->SR1 & I2C_SR1_TXE)); 
        I2C1->DR = data[i]; 
    }

    // Ожидание окончания передачи последнего байта и STOP
    while (!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

// Команды
static void SSD1306_WriteCmds(const uint8_t *cmds, uint16_t count) 
{
    SSD1306_WriteBuffer(0x00, cmds, count);
}

// Данные, сброс курсора и отправка буфера кадра
static void SSD1306_WriteData(void) 
{ 
    SSD1306_WriteCmds(RESET_ADDR_CMDS, sizeof(RESET_ADDR_CMDS));
    SSD1306_WriteBuffer(0x40, frame_buffer, 1024);
}

void SSD1306_Init(void) 
{
    I2C1->CR2   &= ~I2C_CR2_FREQ; // Указываем скорость тактирование шины APB1, чтобы потом поделить
    I2C1->CR2   |= 16;           
    I2C1->CCR = I2C_CCR_FS | 13;  // Ставим флаг FS и указываем скорость по формуле для fast mode
    I2C1->TRISE = 6;              // Указываем время нарастание фронта  
    I2C1->CR1   |= I2C_CR1_PE;    // Включаем I2C1

    // Отправка команд инициализации экрана
    SSD1306_WriteCmds(INIT_CMDS, sizeof(INIT_CMDS));
}

// Очищаем буфер кадра
void SSD1306_Clear() 
{
    for (uint16_t i = 0; i < sizeof(frame_buffer); i++) 
        frame_buffer[i] = OFF; // Выключаем все пиксели
}

// Обновление кадра
void SSD1306_Update() 
{
    SSD1306_WriteData();
}

// Рисуем пиксель
void SSD1306_DrawPixel(uint8_t x, uint8_t y, PixelState state) 
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
        return;

    if (state == ON) 
        frame_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    else 
        frame_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

// Рисуем символ
void SSD1306_WriteChar(uint8_t x, uint8_t y, char ch, PixelState state) 
{
    // Проверка на печатаемые символы ASCII
    if (ch < 32 || ch > 126) 
        ch = '?';

    // Проходим по 5 столбцам шрифта
    for (uint8_t i = 0; i < 5; i++) 
    {
        // Берем байт столбца из таблицы шрифта
        uint8_t line = Font5x7[ch - 32][i];

        // Проверяем каждый бит из 8 по вертикали
        for (uint8_t j = 0; j < 8; j++) 
        {
            if (line & (1 << j)) 
                SSD1306_DrawPixel(x + i, y + j, state);
            else
                SSD1306_DrawPixel(x + i, y + j, (state == ON) ? OFF : ON);
        }
    }
}

// Отрисовка строку
void SSD1306_WriteString(uint8_t x, uint8_t y, const char *str, PixelState state) 
{
    while (*str) 
    {
        // Проверка: помещается ли следующий символ по ширине экрана (128px)
        if (x + 6 >= 128) 
        {
            x = 0;              // Перенос на следующую строку по X
            y += 8;             // Сдвиг вниз по Y на высоту символа
            if (y >= 64) break; // Экран закончился
        }
        SSD1306_WriteChar(x, y, *str, state);
        x += 6; // Сдвиг на 5 пикселей символа + 1 пиксель межсимвольный интервал
        str++;
    }
}