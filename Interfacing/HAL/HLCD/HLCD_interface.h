/**
 * @file    HLCD_interface.h
 * @author  Islam Ahmed
 * @brief   This file contains interfacing information related to LCD module
 * @version 1.0
 * @date    2022-09-23
 * 
 * 
 */

/*Header file guard*/
#ifndef __HLCD_INTERFACE_H__
#define __HLCD_INTERFACE_H__

/***************************************************************************************************/
/*                                         Interfacing macros                                      */
/***************************************************************************************************/

/*LCD Rows*/
#define LCD_ROW_0     (0x00)
#define LCD_ROW_1     (0x40)

/*LCD Columns*/
#define LCD_COL_0     (0x00)
#define LCD_COL_1     (0x01)
#define LCD_COL_2     (0x02)
#define LCD_COL_3     (0x03)
#define LCD_COL_4     (0x04)
#define LCD_COL_5     (0x05)
#define LCD_COL_6     (0x06)
#define LCD_COL_7     (0x07)
#define LCD_COL_8     (0x08)
#define LCD_COL_9     (0x09)
#define LCD_COL_10    (0x0A)
#define LCD_COL_11    (0x0B)
#define LCD_COL_12    (0x0C)
#define LCD_COL_13    (0x0D)
#define LCD_COL_14    (0x0E)
#define LCD_COL_15    (0x0F)


/***************************************************************************************************/
/*                                        Functions' prototypes                                    */
/***************************************************************************************************/

/**
 * @brief This function is used to initialize the LCD module
 * 
 */
void hlcd_init(void);

/**
 * @brief This function is used to display single character over LCD
 * 
 * @param au8_characterData 
 */
void hlcd_displayCharacter(u8_t au8_characterData);

/**
 * @brief This function is responsible for displaying a string over LCD
 * 
 * @param pu8_stringData 
 */
void hlcd_displayString(u8_t* pu8_stringData);


/**
 * @brief This function is responsible for choosing the display position
 * 
 * @param au8_row 
 * @param au8_col 
 */
void hlcd_displayPosition(u8_t au8_row, u8_t au8_col);
void Lcd_DisplayNum(u16_t num);
void Lcd_Display3Num(u16_t num);
void Lcd_Display2Num(u8_t num);

#endif /*__HLCD_INTERFACE_H__*/