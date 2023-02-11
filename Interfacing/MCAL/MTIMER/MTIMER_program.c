/**
 * @file    MTIMER_program.c
 * @author  Islam Ahmed
 * @brief   This file contains logical implementation related to Timer module
 * @version 1.0
 * @date    2022-10-08
 * 
 * 
 */

/***************************************************************************************************/
/*                                             Includes                                            */
/***************************************************************************************************/

#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "avr/interrupt.h"
#include "MTIMER_private.h"
#include "MTIMER_cfg.h"
#include "MTIMER_interface.h"

/***************************************************************************************************/
/*                                        Important macros                                         */
/***************************************************************************************************/

#define NULL                     ((void*)0)
#define TIMER_PH_CORRET_MODE     (0x60)
#define CAPTURE_SIZE             (3)
#define FIRST_CAPTURE            (0)
#define SECOND_CAPTURE           (1)
#define THIRD_CAPTURE            (2)
#define PERCENTAGE_CONVERSION    (100)

/***************************************************************************************************/
/*                                        Important variables                                      */
/***************************************************************************************************/

/*Global variable used to set the timers prescaler*/
static u8_t gu8_timer0_prescaler;
static u8_t gu8_timer1_prescaler;
static u8_t gu8_timer2_prescaler;

/*Global variable used to set the timers asynchronous operation mode*/
static u8_t gu8_timer0_operation;
static u8_t gu8_timer1_operation;
static u8_t gu8_timer2_operation;

/*Global variables used to store the delay time in ms*/
static u32_t gu32_delayTimeMs_timer0;
static u32_t gu32_delayTimeMs_timer1;
static u32_t gu32_delayTimeMs_timer2;

/*Global pointer to function variables used to store the timers callback functions*/
static void (*gfptr_timer0ISR)(void) = NULL;
static void (*gfptr_timer1ISR)(void) = NULL;
static void (*gfptr_timer2ISR)(void) = NULL;

/***************************************************************************************************/
/*                                       Functions' definitions                                    */
/***************************************************************************************************/

static void mtimer_start(u8_t au8_timerChannel)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:
            
            /*Reset the timer counter0*/
            MTIMER_TCNT0 = 0;

            /*Setting the timer0 prescaler and start its operation*/
            MTIMER_TCCR0 |= gu8_timer0_prescaler;

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:
            
            /*Reset the timer counter 1*/
            MTIMER_TCNT1 = 0;

            /*Setting the timer prescaler and start its operation*/
            MTIMER_TCCR1 |= (u16_t)gu8_timer1_prescaler;

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Reset the timer counter0*/
            MTIMER_TCNT2 = 0;

            /*Setting the timer0 prescaler and start its operation*/
            MTIMER_TCCR2 |= gu8_timer2_prescaler;

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

static void mtimer_stop(u8_t au8_timerChannel)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Stop the timer0 operation*/
            MTIMER_TCCR0 &= TIMER_OPERATION_STOP;

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Stop the timer1 operation*/
            MTIMER_TCCR1 &= TIMER1_OPERATION_STOP;

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_init(u8_t au8_timerChannel, u16_t au16_timerMode, u8_t au8_timerPrescaler)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Checking if the timer mode chosen is PWM mode or not*/
            if(au16_timerMode == (u16_t)TIMER_PWM_MODE)
            {
                /*Checking if the chosen PWM mode is phase correct or fast PWM*/
                if(au8_timerPrescaler > TIMER_PH_CORRECT_OFFSET)
                {
                    /*Setting phase correct mode*/
                    au16_timerMode = (u16_t)TIMER_PH_CORRET_MODE;

                    /*Setting the new timer prescaler*/
                    au8_timerPrescaler -= TIMER_PH_CORRECT_OFFSET;
                }
                else
                {
                    /*Do nohing*/
                }
            }
            else
            {
                /*Do nothing*/
            }

            /*Setting the timer0 mode*/
            MTIMER_TCCR0 = (u8_t)au16_timerMode;

            /*Settint the selected timer prescaler*/
            gu8_timer0_prescaler = au8_timerPrescaler;

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

        /*Checking if the timer mode chosen is PWM mode or not*/
            if(au16_timerMode == (u16_t)TIMER_PWM_MODE)
            {
                /*Checking if the chosen PWM mode is phase correct or fast PWM*/
                if(au8_timerPrescaler > TIMER_PH_CORRECT_OFFSET)
                {
                    /*Setting phase correct mode*/
                    au16_timerMode = (u16_t)TIMER_PH_CORRET_MODE;

                    /*Setting the new timer prescaler*/
                    au8_timerPrescaler -= TIMER_PH_CORRECT_OFFSET;
                }
                else
                {
                    /*Do nohing*/
                }
            }
            else
            {
                /*Do nothing*/
            }
            
            /*Setting the selected timer mode*/            
            MTIMER_TCCR1 = au16_timerMode;

            /*Settint the selected timer prescaler*/
            gu8_timer1_prescaler = au8_timerPrescaler;

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

           /*Checking if the timer mode chosen is PWM mode or not*/
            if(au16_timerMode == (u16_t)TIMER_PWM_MODE)
            {
                /*Checking if the chosen PWM mode is phase correct or fast PWM*/
                if(au8_timerPrescaler > TIMER_PH_CORRECT_OFFSET)
                {
                    /*Setting phase correct mode*/
                    au16_timerMode = (u16_t)TIMER_PH_CORRET_MODE;

                    /*Setting the new timer prescaler*/
                    au8_timerPrescaler -= TIMER_PH_CORRECT_OFFSET;
                }
                else
                {
                    /*Do nohing*/
                }
            }
            else
            {
                /*Do nothing*/
            }

            /*Setting the timer0 mode*/
            MTIMER_TCCR2 = (u8_t)au16_timerMode;

            /*Settint the selected timer prescaler*/
            gu8_timer2_prescaler = au8_timerPrescaler;

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_delayMs_synchronous(u8_t au8_timerChannel, u32_t au32_delayTime_ms)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Setting the OCR0 value to make a compare match after 1ms*/
            MTIMER_OCR0 = TIMER_1_MS_DELAY;

            /*Start the timer operation*/
            mtimer_start(TIMER_CHANNEL_0);
            
            /*Looping over delay value*/
            while(au32_delayTime_ms--)
            {
                /*Waiting until the 1ms delay passes*/
                while(!GET_BIT(MTIMER_TIFR, MTIMER_TIFR_OCF0_BIT));
                
                /*Clearing the flag bit*/
                SET_BIT(MTIMER_TIFR, MTIMER_TIFR_OCF0_BIT);
            }

            /*Stopping the timer operation*/
            mtimer_stop(TIMER_CHANNEL_0);

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_delayMs_asynchronous(u8_t au8_timerChannel, u32_t au32_delayTime_ms, void (*fptr_operationPointer)(void), u8_t au8_operationType)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Setting the OCR0 value to make a compare match after 1ms*/
            MTIMER_OCR0 = TIMER_1_MS_DELAY;

            /*Setting the timer0 operation parameters*/
            gu32_delayTimeMs_timer0 = au32_delayTime_ms;
            gfptr_timer0ISR = fptr_operationPointer;
            gu8_timer0_operation = au8_operationType;

            /*Enable output compare match interrupt*/
            SET_BIT(MTIMER_TIMSK, MTIMER_TIMSK_OCIE0_BIT);

            /*Starting timer0 operations*/
            mtimer_start(TIMER_CHANNEL_0);

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Setting the timer1 operation parameters*/
            gu32_delayTimeMs_timer1 = au32_delayTime_ms;
            gfptr_timer1ISR = fptr_operationPointer;
            gu8_timer1_operation = au8_operationType;

            /*Starting timer1 operations*/
            mtimer_start(TIMER_CHANNEL_1);

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Setting the timer2 operation parameters*/
            gu32_delayTimeMs_timer2 = au32_delayTime_ms;
            gfptr_timer2ISR = fptr_operationPointer;
            gu8_timer2_operation = au8_operationType;

            /*Starting timer2 operations*/
            mtimer_start(TIMER_CHANNEL_2);

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_runPWM_signal(u8_t au8_timerChannel, u8_t au8_dutyCycle)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Setting the duty cycle*/
            MTIMER_OCR0 = au8_dutyCycle;

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Setting the duty cycle*/
            MTIMER_OCR1A = (u16_t)au8_dutyCycle;
            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Setting the duty cycle*/
            MTIMER_OCR2 = au8_dutyCycle;
            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }
    
    /*Return from this function*/
    return;    
}

void mtimer_startPWM_signal(u8_t au8_timerChannel)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Starting timer0 operations*/
            mtimer_start(TIMER_CHANNEL_0);

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Starting timer1 operations*/
            mtimer_start(TIMER_CHANNEL_1);

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Starting timer2 operations*/
            mtimer_start(TIMER_CHANNEL_2);

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_stopPWM_signal(u8_t au8_timerChannel)
{
    /*Switching over timer channels*/
    switch(au8_timerChannel)
    {
        /*In case of channel 0*/
        case TIMER_CHANNEL_0:

            /*Stopping timer0 operations*/
            mtimer_stop(TIMER_CHANNEL_0);

            /*Breaking from this case*/
            break;

        /*In case of channel 1*/
        case TIMER_CHANNEL_1:

            /*Stopping timer1 operations*/
            mtimer_stop(TIMER_CHANNEL_1);

            /*Breaking from this case*/
            break;

        /*In case of channel 2*/
        case TIMER_CHANNEL_2:

            /*Stopping timer2 operations*/
            mtimer_stop(TIMER_CHANNEL_2);

            /*Breaking from this case*/
            break;

        /*In case of default*/
        default:

            /*Breaking from this case*/
            break;
    }

    /*Return from this function*/
    return;
}

void mtimer_getSignal_duty_and_freq(u8_t* pu8_duty, u32_t* pu32_freq,u8_t* pu8_Time)
{
    /*Local variable used in looping operations*/
    u8_t au8_loopingVar = 0; 
    
    /*Local variables used in ICU operations*/
    u16_t au16_captureValue[CAPTURE_SIZE] = {0};

    /*Local variables used to store the ON period and total period*/
    u16_t au16_Ton = 0;
    u16_t au16_periodicTime = 0;

    /*Setting the ICU to be triggered on rising edge*/
    SET_BIT(MTIMER_TCCR1, MTIMER_TCCR1_ICES1_BIT);        

    /*Running timer channel 1*/
    mtimer_start(TIMER_CHANNEL_1);

    /*Looping over ICU captures*/
    for(au8_loopingVar = 0; au8_loopingVar < CAPTURE_SIZE; au8_loopingVar++)
    {
        /*Waiting until the rising/falling edge happens*/
        while(!GET_BIT(MTIMER_TIFR, MTIMER_TIFR_ICF1_BIT));

        /*Getting the capture value*/
        au16_captureValue[au8_loopingVar] = MTIMER_ICR1;

        /*Clearing the ICF1 flag*/
        SET_BIT(MTIMER_TIFR, MTIMER_TIFR_ICF1_BIT);

        /*Setting the ICU to be triggered on rising/falling edge*/
        TOGGLE_BIT(MTIMER_TCCR1, MTIMER_TCCR1_ICES1_BIT);        
    }

    /*Stopping the timer operations*/
    mtimer_stop(TIMER_CHANNEL_1);

    /*Getting the Ton value*/
    au16_Ton = au16_captureValue[SECOND_CAPTURE] - au16_captureValue[FIRST_CAPTURE];

    /*Getting the periodic time value*/
    au16_periodicTime = au16_captureValue[THIRD_CAPTURE] - au16_captureValue[FIRST_CAPTURE];

    *pu8_Time=au16_periodicTime;
    /*Getting the duty cycle*/
    *pu8_duty = (u8_t)( (((u32_t)au16_Ton * PERCENTAGE_CONVERSION) / (u32_t)au16_periodicTime) );

    /*Getting the frequency value in Hz*/
    *pu32_freq = TIMER_ICU_FREQ_4US_DIVIDER / (u32_t)au16_periodicTime;

    /*Return from this function*/
    return;
}

/***************************************************************************************************/
/*                                              ISRs                                               */
/***************************************************************************************************/

/*Timer0 ISR*/
ISR(TIMER0_COMP_vect)
{
    /*Static global variable used to track the delay time*/
    static u32_t au32_delayTemp = 0;

    /*Increment the delay variable*/
    au32_delayTemp++;

    /*Checking if the delay time has passed or not*/
    if(au32_delayTemp == gu32_delayTimeMs_timer0)
    {
        /*Checking if the callback function is valid or not*/
        if(gfptr_timer0ISR != NULL)
        {                    
            /*Executing the timer0 ISR callback function*/
            gfptr_timer0ISR();

            /*Reset the delay variable*/
            au32_delayTemp = 0;

            /*Checking if the timer is in single mode operation or not*/
            if(gu8_timer0_operation == TIMER_SINGLE_OPERATION)
            {
                /*Stopping the timer0*/
                mtimer_stop(TIMER_CHANNEL_0);
            }
            else
            {
                /*Do nothing*/
            }
        }
        else
        {
            /*Do nothing*/
        }
    }
    else
    {
        /*Do nothing*/
    }
}

/*Timer0 ISR*/
ISR(TIMER2_COMP_vect)
{
    /*Static global variable used to track the delay time*/
    static u32_t au32_delayTemp = 0;

    /*Increment the delay variable*/
    au32_delayTemp++;

    /*Checking if the delay time has passed or not*/
    if(au32_delayTemp == gu32_delayTimeMs_timer2)
    {
        /*Checking if the callback function is valid or not*/
        if(gfptr_timer2ISR != NULL)
        {                    
            /*Executing the timer0 ISR callback function*/
            gfptr_timer2ISR();

            /*Reset the delay variable*/
            au32_delayTemp = 0;

            /*Checking if the timer is in single mode operation or not*/
            if(gu8_timer2_operation == TIMER_SINGLE_OPERATION)
            {
                /*Stopping the timer0*/
                mtimer_stop(TIMER_CHANNEL_2);
            }
            else
            {
                /*Do nothing*/
            }
        }
        else
        {
            /*Do nothing*/
        }
    }
    else
    {
        /*Do nothing*/
    }
}