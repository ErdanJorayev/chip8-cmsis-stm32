#include "stm32f4xx.h"

void GPIO_init(void)
{
    // Тактирование портов B, A и модуля I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB6 (SCL) и PB7 (SDA) Alternate Function 
    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |=  (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);

    // Open-Drain для шины I2C
    GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

    // Внутренняя подтяжка Pull-Up 
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);
    GPIOB->PUPDR |=  (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);

    // Very High speed 
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7);
    GPIOB->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7);

    // AF4 (0100) для I2C1 на PB6 и PB7
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);
    GPIOB->AFR[0] |=  (GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL7_2);


    // Кнопки PA5...PA8_____________________________________________
    GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 |
                      GPIO_MODER_MODER7 | GPIO_MODER_MODER8);
    
    // Подятжки для кнопок
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 |
                      GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8);
    GPIOA->PUPDR |=  (GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 |
                      GPIO_PUPDR_PUPDR7_0 | GPIO_PUPDR_PUPDR8_0);
}