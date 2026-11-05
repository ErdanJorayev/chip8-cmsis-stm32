#ifndef INITS_H
#define INITS_H

void GPIO_init(void);
void EXTI_init(void);
void Chip8_Init(Chip8 * chip8);

void HandleInput(Chip8 * chip8);


#endif