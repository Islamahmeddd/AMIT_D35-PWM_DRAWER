/**
 * @file    main.c
 * @author  Islam Ahmed
 * @brief   This is main.c file contain Application of the SW
 * @version 1.0
 * @date    25/Jan/2023
 * 
 * 
 */


#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "avr/io.h"
#include "MDIO_interface.h"
#include "MTIMER_interface.h"
#include "HLCD_interface.h"
#include <util/delay.h>
void displayInfo(u8_t dutyCycle,u8_t time,u32_t freq);

int main(void)
{
    u8_t duty;
    u8_t time;
    u32_t freq;
    mdio_setPinStatus(PORTD, PIN7, OUTPUT);//OC2 Timer 2
    mdio_setPinStatus(PORTD, PIN6, INPUT_FLOAT);//Input Capture pin ICP1
    mtimer_init(TIMER_CHANNEL_2, TIMER_PWM_MODE, TIMER_FREQ_1KHZ); //Initialize timer2 with mode PWm and freq 
    mtimer_init(TIMER_CHANNEL_1, TIMER_ICU_MODE, TIMER_ICU_PRESCALER);//Initialize timer1 with input capture Unit Mode and prescaler
    hlcd_init();//Initialize LCD
    mtimer_startPWM_signal(TIMER_CHANNEL_2);//Start PWM
    mtimer_runPWM_signal(TIMER_CHANNEL_2, 80);//Generate PWM with Duty Cycle

    mtimer_getSignal_duty_and_freq(&duty, &freq,&time);//This function calculate Duty and Freq and time of the PWM Signal
 
    while(1)
    {
        displayInfo(duty,time,freq);//Display The Duty and freq and time on LCD

         _delay_ms(100);
    }

    return 0;
}


void displayInfo(u8_t dutyCycle,u8_t time,u32_t freq)
 {

  hlcd_displayPosition(0, 0);		// Start From Row 0 Col 0
	hlcd_displayString((u8_t*)"F="); //Write Frequency
  Lcd_DisplayNum(freq);
	hlcd_displayPosition(0, 6);//start from row 0 and col 6
	hlcd_displayString((u8_t*)"T=");//Write Time
  Lcd_DisplayNum(time);
	hlcd_displayPosition(0, 12);//Start From Row 0 col 12
	hlcd_displayString((u8_t*)"D="); //Write Duty Cycle
  Lcd_Display2Num(dutyCycle);//Display the Duty Cycle
  //Display the Wave
  hlcd_displayPosition(1, 0);		// Start From Row 1 Col 0 
  int bars = (int) (dutyCycle / 10.0);// Calculate the number of bars to display

  // Display the bars
  for (int i = 0; i < bars; i++) {
    hlcd_displayString((u8_t*)"-");
  }

  // Display the rest of the line
  for (int i = bars; i < 10; i++) {
    hlcd_displayString((u8_t*)"_");
  }
//Display the rest of LCD
for (int i = 10; i < 10+bars; i++) {
    hlcd_displayString((u8_t*)"-");
  }

  for (int i = 10+bars; i < 10+bars+bars; i++) {
    hlcd_displayString((u8_t*)"_");
  }
}


