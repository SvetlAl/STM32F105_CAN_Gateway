/**
  ******************************************************************************
  * @file		lcd16x2.h
  * @author	Yohanes Erwin Setiawan
  * @date		6 February 2016
	* @note		Re-write form Peter Fleury AVR LCD library
  ******************************************************************************
  */
	
#ifndef __LCD16X2_H
#define __LCD16X2_H

#ifdef __cplusplus
extern "C" {
#endif

/** Includes ---------------------------------------------------------------- */
#include "stm32f10x.h"	 

/** Imported definitions from STDLib ---------------------------------- */
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

/** Imported definitions from STDLib ---------------------------------- */


/** Port and pin definition for 4-bit mode ---------------------------------- */
// LCD control lines (must be on the same port) 
#define LCD16X2_RCC_GPIO_CONTROL	RCC_APB2Periph_GPIOA
#define LCD16X2_GPIO_CONTROL			GPIOA
#define LCD16X2_GPIO_RS						GPIOA
#define LCD16X2_GPIO_RW						GPIOA
#define LCD16X2_GPIO_EN						GPIOA
// LCD data lines (must be on the same port)
#define LCD16X2_RCC_GPIO_DATA			RCC_APB2Periph_GPIOA
#define LCD16X2_GPIO_DATA					GPIOA
#define LCD16X2_GPIO_D4						GPIOA
#define LCD16X2_GPIO_D5						GPIOA
#define LCD16X2_GPIO_D6						GPIOA
#define LCD16X2_GPIO_D7						GPIOA
// Pin definition
#define LCD16X2_PIN_RS	GPIO_Pin_11
#define LCD16X2_PIN_RW	GPIO_Pin_6
#define LCD16X2_PIN_EN	GPIO_Pin_7
#define LCD16X2_PIN_D4	GPIO_Pin_3		// 4-bit mode LSB
#define LCD16X2_PIN_D5	GPIO_Pin_4
#define LCD16X2_PIN_D6	GPIO_Pin_1
#define LCD16X2_PIN_D7	GPIO_Pin_2		// 4-bit mode MSB

/** Display size ------------------------------------------------------------ */
// Number of visible lines of the display (1 or 2)
#define LCD16X2_LINES					2
// Visible characters per line of the display
#define LCD16X2_DISP_LENGTH		16
// DDRAM address of first char of line 1
#define LCD16X2_START_LINE_1	0x00
// DDRAM address of first char of line 2
#define LCD16X2_START_LINE_2	0x40

/** Delay value ------------------------------------------------------------- */
// Delay power on
#define LCD16X2_DELAY_MULTIPLIER		1
#define LCD16X2_DELAY_POWER_ON   		16000*LCD16X2_DELAY_MULTIPLIER
// Delay after initialize 8-bit
#define LCD16X2_DELAY_INIT      		5000*LCD16X2_DELAY_MULTIPLIER
// Delay after initialize 8-bit repeated
#define LCD16X2_DELAY_INIT_REP    	64*LCD16X2_DELAY_MULTIPLIER
// Delay after set 4-bit mode
#define LCD16X2_DELAY_INIT_4BIT   	64*LCD16X2_DELAY_MULTIPLIER
// Delay until address counter updated after busy flag is cleared
#define LCD16X2_DELAY_BUSY_FLAG    	4*LCD16X2_DELAY_MULTIPLIER
// Enable pulse width high level
#define LCD16X2_DELAY_ENABLE_PULSE	2*LCD16X2_DELAY_MULTIPLIER

/** Instructions bit location ----------------------------------------------- */
#define LCD16X2_CLEAR_DISPLAY					0x01
#define LCD16X2_CURSOR_HOME						0x02
#define LCD16X2_CHARACTER_ENTRY_MODE	0x04
#define LCD16X2_DISPLAY_CURSOR_ON_OFF	0x08
#define LCD16X2_DISPLAY_CURSOR_SHIFT 	0x10
#define LCD16X2_FUNCTION_SET					0x20
#define LCD16X2_SET_CGRAM_ADDRESS	 		0x40
#define LCD16X2_SET_DDRAM_ADDRESS	 		0x80
/* Character entry mode instructions */
#define LCD16X2_INCREMENT							0x02	// Initialization setting
#define LCD16X2_DECREMENT							0x00
#define LCD16X2_DISPLAY_SHIFT_ON			0x01
#define LCD16X2_DISPLAY_SHIFT_OFF			0x00	// Initialization setting
/* Display cursor on off instructions */
#define LCD16X2_DISPLAY_ON	 					0x04
#define LCD16X2_DISPLAY_OFF	 					0x00	// Initialization setting
#define LCD16X2_CURSOR_UNDERLINE_ON	 	0x02
#define LCD16X2_CURSOR_UNDERLINE_OFF	0x00	// Initialization setting
#define LCD16X2_CURSOR_BLINK_ON	 			0x01
#define LCD16X2_CURSOR_BLINK_OFF	 		0x00	// Initialization setting
/* Display cursor shift instructions */
#define LCD16X2_DISPLAY_SHIFT					0x08
#define LCD16X2_CURSOR_MOVE						0x00
#define LCD16X2_RIGHT_SHIFT						0x04
#define LCD16X2_LEFT_SHIFT						0x00
/* Function set instructions */
#define LCD16X2_8BIT_INTERFACE				0x10	// Initialization setting
#define LCD16X2_4BIT_INTERFACE				0x00
#define LCD16X2_2LINE_MODE						0x08
#define LCD16X2_1LINE_MODE						0x00	// Initialization setting
#define LCD16X2_5X10DOT_FORMAT				0x04
#define LCD16X2_5X7DOT_FORMAT					0x00	// Initialization setting
/* Busy flag bit location */
#define LCD16X2_BUSY_FLAG							0x80

/** LCD display and cursor attributes --------------------------------------- */
#define LCD16X2_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF	(LCD16X2_DISPLAY_OFF | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_OFF)					
#define LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_ON		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_OFF | LCD16X2_CURSOR_BLINK_ON)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_OFF		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_OFF)
#define LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON		(LCD16X2_DISPLAY_ON | \
	LCD16X2_CURSOR_UNDERLINE_ON | LCD16X2_CURSOR_BLINK_ON)

/** Public function prototypes ---------------------------------------------- */
void lcd16x2_init(uint8_t disp_attr);
void lcd16x2_write_command(uint8_t cmd);
void lcd16x2_write_data(uint8_t data);
void lcd16x2_clrscr(void);
void lcd16x2_home(void);
void lcd16x2_gotoxy(uint8_t x, uint8_t y);
uint8_t lcd16x2_getxy(void);
void lcd16x2_entry_inc(void);
void lcd16x2_entry_dec(void);
void lcd16x2_entry_inc_shift(void);
void lcd16x2_entry_dec_shift(void);
void lcd16x2_display_on(void);
void lcd16x2_display_off(void);
void lcd16x2_cursor_on(void);
void lcd16x2_cursor_off(void);
void lcd16x2_blink_on(void);
void lcd16x2_blink_off(void);
void lcd16x2_display_shift_left(void);
void lcd16x2_display_shift_right(void);
void lcd16x2_cursor_shift_left(void);
void lcd16x2_cursor_shift_right(void);
void lcd16x2_putc(const char c);
void lcd16x2_puts(const char* s);
void lcd16x2_create_custom_char(uint8_t location, uint8_t* data_bytes);
void lcd16x2_put_custom_char(uint8_t x, uint8_t y, uint8_t location);







#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
