#include "stm32f4xx.h"

// Настройка GPIO
void GPIO_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  // Тактируем порт B
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;   // Тактируем модуль I2C1

    // Чистим регистры пинов, 10 - старший бит, альтернативная функция
    GPIOB->MODER   &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);   
    GPIOB->MODER   |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1); 
    
    // Отключаем транзисторы подачи напряжение
    GPIOB->OTYPER  |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);       

    // Подаем напряжение через резистор, сначала очищаем
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);         // Очистка (00)
    GPIOB->PUPDR |=  (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);     // Запись (01 - Pull-Up)

    // Устанавливаем скорость работы транзисторов (по сути скорость их открытие и закрытие)
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7); // Очистка (00)
    GPIOB->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7); // Запись (11 - Very High)      
           
    // Назначаем пинам альтернативную функцию (I2C1), в начале очистка
    GPIOB->AFR[0] &= ~((15 << (6 * 4))  | (15 << (7 * 4)));
    GPIOB->AFR[0] |=  ((4 << (6 * 4))   | (4 << (7 * 4)));

    // Добавляем кнопки
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                            
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);         
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);       
    GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0);     
}