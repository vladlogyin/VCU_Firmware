#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
//#include <BMP180/BMP180.h>
#include <lm75/lm75.h>
//#include <htu21d/htu21d.h>
//#include <ds3231/ds3231.h>
#include <max7219/max7219.h>
#include <bme280/bme280.h>
//#include <ili9341/ili9341.h>
//#include <rc522/rc522.h>
#include <systems.h>
#include <stdlib.h>
//#include <stdio.h>
#include <printf/printf.h>
#include <cerrno>
void i2csetup(void);
void rgb(uint8_t,uint8_t,uint8_t);
void clocksetup(void);
void gpiosetup(void);
void usartsetup(void);
void spisetup(void);


int print(const char*);
int _write(int file, char *ptr, int len);
void _putchar(char c);



