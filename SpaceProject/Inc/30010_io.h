#ifndef _30010_IO_H_
#define _30010_IO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define UART_BUFFER_LENGTH 256

/* Exported functions ------------------------------------------------------- */
/****************************/
/*** USB Serial Functions ***/
/****************************/
void uart_init(uint32_t baud);
void uart_put_char(uint8_t c);
uint8_t uart_get_char();
uint8_t uart_get_count();
void uart_clear();

/*****************************/
/*** LCD Control Functions ***/
/*****************************/
void lcd_init();
void lcd_transmit_byte(uint8_t data);
void lcd_push_buffer(uint8_t* buffer);
void lcd_reset();

#endif /* _30010_IO_H_ */

void lcd_write_string(uint8_t buffer[512], char *slice, uint8_t line);
void lcd_write_string2(uint8_t buffer[512], char *slice, uint8_t line);
void turnOn(GPIO_TypeDef *pin, uint32_t pinnum);
void turnOff(GPIO_TypeDef *pin, uint32_t pinnum);
