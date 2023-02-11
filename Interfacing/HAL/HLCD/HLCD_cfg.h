/**
 * @file    HLCD_cfg.h
 * @author  Islam Ahmed
 * @brief   This file contains configuration information related to LCD module
 * @version 1.0
 * @date    2022-09-23
 * 
 * 
 */

/*Header file guard*/
#ifndef __HLCD_CFG_H__
#define __HLCD_CFG_H__

/***************************************************************************************************/
/*                                         Configuration macros                                    */
/***************************************************************************************************/

/*LCD Control lines configurations*/
#define LCD_CONTROL_LINES_PORT      (PORTB)
#define LCD_RS_PIN                  (PIN1)
#define LCD_RW_PIN                  (PIN2)
#define LCD_EN_PIN                  (PIN3)

/*LCD Data lines configurations*/
#define LCD_DATA_LINES_PORT         (PORTA)
#define LCD_D4_PIN                  (PIN4)
#define LCD_D5_PIN                  (PIN5)
#define LCD_D6_PIN                  (PIN6)
#define LCD_D7_PIN                  (PIN7)

/*LCD Instructions*/
#define LCD_FUNCTION_SET            (0x2C)
#define LCD_DISP_ON_OFF_CTRL        (0x0F)
#define LCD_CLEAR                   (0x01)
#define LCD_ENTRY_MODE_SET          (0x06)

#endif /*__HLCD_CFG_H__*/