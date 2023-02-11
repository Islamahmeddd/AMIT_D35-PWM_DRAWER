/**
 * @file    HLCD_program.c
 * @author  Islam Ahmed
 * @brief   This file contains logical implementation related to LCD module
 * @version 1.0
 * @date    2022-09-23
 * 
 * 
 */

/***************************************************************************************************/
/*                                             Includes                                            */
/***************************************************************************************************/

#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "MDIO_interface.h"
#include "HLCD_cfg.h"
#include "HLCD_interface.h"
#include "util/delay.h"

/***************************************************************************************************/
/*                                          Important macros                                       */
/***************************************************************************************************/

/*Data byte bits*/
#define BIT_0                  (0)
#define BIT_1                  (1)
#define BIT_2                  (2)
#define BIT_3                  (3)
#define BIT_4                  (4)
#define BIT_5                  (5)
#define BIT_6                  (6)
#define BIT_7                  (7)

/*LCD write operation triggering time delay in Microseconds*/
#define TRIGGER_DELAY_US       (50)

/*LCD delay times in Milliseconds*/
#define POWER_ON_DELAY_MS      (50)
#define INSTRUCTION_DELAY_MS   (1)
#define CLEAR_DELAY_MS         (2)

/*DDRAM Address control header*/
#define SET_DDRAM_ADD_HEADER   (0x80)

/***************************************************************************************************/
/*                                       Functions' definitions                                    */
/***************************************************************************************************/

static void hlcd_sendData(u8_t au8_data)
{
    /*Setting RS Pin to HIGH to send data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_RS_PIN, HIGH);

    /*Setting R/W Pin to LOW to write data over LCD*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_RW_PIN, LOW);

    /*Sending the high nibble 4-bits*/
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D4_PIN, GET_BIT(au8_data, BIT_4));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D5_PIN, GET_BIT(au8_data, BIT_5));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D6_PIN, GET_BIT(au8_data, BIT_6));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D7_PIN, GET_BIT(au8_data, BIT_7));

    /*Triggering the LCD MCU to read the written data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, LOW);
    _delay_us(TRIGGER_DELAY_US);
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);

    /*Sending the low nibble 4-bits*/
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D4_PIN, GET_BIT(au8_data, BIT_0));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D5_PIN, GET_BIT(au8_data, BIT_1));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D6_PIN, GET_BIT(au8_data, BIT_2));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D7_PIN, GET_BIT(au8_data, BIT_3));

    /*Triggering the LCD MCU to read the written data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, LOW);
    _delay_us(TRIGGER_DELAY_US);
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);
    
    /*Return from this function*/
    return;
}

static void hlcd_sendCommand(u8_t au8_command)
{
    /*Setting RS Pin to HIGH to send data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_RS_PIN, LOW);

    /*Setting R/W Pin to LOW to write data over LCD*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_RW_PIN, LOW);

    /*Sending the high nibble 4-bits*/
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D4_PIN, GET_BIT(au8_command, BIT_4));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D5_PIN, GET_BIT(au8_command, BIT_5));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D6_PIN, GET_BIT(au8_command, BIT_6));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D7_PIN, GET_BIT(au8_command, BIT_7));

    /*Triggering the LCD MCU to read the written data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, LOW);
    _delay_us(TRIGGER_DELAY_US);
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);

    /*Sending the low nibble 4-bits*/
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D4_PIN, GET_BIT(au8_command, BIT_0));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D5_PIN, GET_BIT(au8_command, BIT_1));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D6_PIN, GET_BIT(au8_command, BIT_2));
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D7_PIN, GET_BIT(au8_command, BIT_3));

    /*Triggering the LCD MCU to read the written data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, LOW);
    _delay_us(TRIGGER_DELAY_US);
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);

    /*Return from this function*/
    return;
}

void hlcd_init(void)
{
    /*Setting the LCD control lines to output*/
    mdio_setPinStatus(LCD_CONTROL_LINES_PORT, (LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN), OUTPUT);

    /*Setting the LCD data lines to output*/
    mdio_setPinStatus(LCD_DATA_LINES_PORT, (LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN), OUTPUT);

    /*Setting the enable pin to high*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);

    /*Waiting for VDD to rise up to 4.5v*/
    _delay_ms(POWER_ON_DELAY_MS);

    /*Sending the high nibble 4-bits*/
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D4_PIN, LOW);
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D5_PIN, HIGH);
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D6_PIN, LOW);
    mdio_setPinValue(LCD_DATA_LINES_PORT, LCD_D7_PIN, LOW);

    /*Triggering the LCD MCU to read the written data*/
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, LOW);
    _delay_us(TRIGGER_DELAY_US);
    mdio_setPinValue(LCD_CONTROL_LINES_PORT, LCD_EN_PIN, HIGH);

    /*Sending function set command*/
    hlcd_sendCommand(LCD_FUNCTION_SET);

    /*Waiting for instruction to be executed*/
    _delay_ms(INSTRUCTION_DELAY_MS);

    /*Sending the display on/off control command*/
    hlcd_sendCommand(LCD_DISP_ON_OFF_CTRL);

    /*Waiting for instruction to be executed*/
    _delay_ms(INSTRUCTION_DELAY_MS);

    /*Sending the clear command*/
    hlcd_sendCommand(LCD_CLEAR);

    /*Waiting for instruction to be executed*/
    _delay_ms(CLEAR_DELAY_MS);

    /*Sending the entry mode set command*/
    hlcd_sendCommand(LCD_ENTRY_MODE_SET);

    /*Waiting for instruction to be executed*/
    _delay_ms(INSTRUCTION_DELAY_MS);

    /*Return from this function*/
    return;
}

void hlcd_displayCharacter(u8_t au8_characterData)
{
    /*Sending character data to LCD*/
    hlcd_sendData(au8_characterData);

    /*Return from this function*/
    return;
}

void hlcd_displayString(u8_t* pu8_stringData)
{
    /*Looping over string characters*/
    while (*pu8_stringData != '\0')
    {
        /*Sending character data*/
        hlcd_sendData(*pu8_stringData);

        /*Increasing the pointer to get the next character*/
        pu8_stringData++;
    }
    
    /*Return from this function*/
    return;
}

void hlcd_displayPosition(u8_t au8_row, u8_t au8_col)
{
     switch(au8_row)
	{
	case 0:
		hlcd_sendCommand(0x80+au8_col);
		break;
	case 1:
		hlcd_sendCommand(0xC0+au8_col);
		break;
	}

    /*Return from this function*/
    return;
}

void Lcd_DisplayNum(u16_t num)
{
	hlcd_displayCharacter(num/1000 + '0');
	hlcd_displayCharacter(num%1000/100 + '0');
	hlcd_displayCharacter(num%1000%100/10 + '0');
	hlcd_displayCharacter(num%1000%100%10 + '0');
}
void Lcd_Display3Num(u16_t num)
{
	hlcd_displayCharacter(num/100 + '0');
	hlcd_displayCharacter(num%100/10 + '0');
	hlcd_displayCharacter(num%100%10 + '0');
}

void Lcd_Display2Num(u8_t num)
{
	hlcd_displayCharacter(num/10 + '0');
	hlcd_displayCharacter(num%10+ '0');
}
