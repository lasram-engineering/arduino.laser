#include "pwm.h"
#include <Arduino.h>

// 16 MHz clock
// 8x prescaler
// PWM frequency = CLK / (256 * PRE) =~ 7,8 kHz

/// Initializes the counter of the arduino
void initPWM()
{

    DDRD |= (1 << PD6);                    // set the PORTD6 to output (OC06 register, PWM)
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // set the timer mode to fast pwm
    TCCR0A |= (1 << COM0A1);               // non-inverting mode
}

void startPWM()
{
    TCCR0B |= (1 << CS01); // set the prescaler to 8
}

void stopPWM()
{
    TCCR0B = 0; // stops the clock
}

/// Sets the duty cycle
/// \param duty the duty cycle as a fraction
void setDuty(double duty)
{
    // 37.5% is the max duty cycle
    // 255 * 37.5 / 100 = 95,625
    OCR0A = (int)(duty * 95.625);
}
