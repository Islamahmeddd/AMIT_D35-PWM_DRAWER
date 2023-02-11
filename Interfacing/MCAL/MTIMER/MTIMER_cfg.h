/**
 * @file    MTIMER_cfg.h
 * @author  Islam Ahmed
 * @brief   This file contains configuration information related to Timer module
 * @version 1.0
 * @date    2022-10-08
 * 
 * 
 */

/*Header file guard*/
#ifndef __MTIMER_CFG_H__
#define __MTIMER_CFG_H__

/***************************************************************************************************/
/*                                         Configuration macros                                    */
/***************************************************************************************************/

/*Custom prescalers choice*/
#define TIMER_OPERATION_STOP            (0xF8)
#define TIMER1_OPERATION_STOP           (0xFFF8)

/*ICU frequency value*/
#define TIMER_ICU_FREQ_4US_DIVIDER      (250000)

/*Clear to compare mode 1 ms delay*/
#define TIMER_1_MS_DELAY                (250)

#endif /*__MTIMER_CFG_H__*/