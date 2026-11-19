CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

MCU     = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
DEFINES = -DSTM32F411xE

# Добавляем инклюды (теперь все слэши на месте)
INCLUDES = -Iinc \
           -Issd1306 \
		   -Ichip8 \
           -Icmsis \
           -I.

CFLAGS  = $(MCU) $(DEFINES) $(INCLUDES) -O1 -g -Wall -fdata-sections -ffunction-sections
LDFLAGS = $(MCU) -Tlinker_script.ld -nostartfiles -nodefaultlibs -nolibc -Wl,--gc-sections

# Исходники проекта
SRCS    = src/main.c      \
		  src/init_work.c \
		  src/pong.c      \
		  src/exti_init.c \
		  src/gpio_init.c \
		  chip8/core.c    \
		  chip8/fonts.c   \
          ssd1306/fonts.c \
          ssd1306/ssd1306.c \
          startup_stm32f411xe.s

# Преобразуем исходники в список .o файлов
OBJS    = $(addsuffix .o, $(basename $(SRCS)))

TARGET  = diod

# Главная цель по умолчанию (выполняется при вызове просто `make`)
all: build

# Правило для сбора .c файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для ассемблерных файлов .s
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка ELF и выгрузка размера
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# Создание чистокровного BIN бинарника
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Сборка + подчистка объектников, остается только готовый bin
build: $(TARGET).bin
	@echo "Удаляем промежуточные файлы..."
	rm -f $(OBJS) $(TARGET).elf
	@echo "Готово! Остался только $(TARGET).bin"

# Полная очистка всего
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

.PHONY: all build clean