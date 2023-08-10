#include "pwm.h"
#include <Arduino.h>

void startPWM()
{

    // 16 MHz clock
    // 8x prescaler
    // PWM frequency = CLK / (256 * PRE) =~ 7,8 kHz
    DDRD |= (1 << PD6);                                    // set the PORTD6 to output (OC06 register, PWM)  D6
    TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1); // set the timer mode to fast pwm
    TCCR0B |= (1 << CS01);                                 // set the prescaler to 8
}

void stopPWM()
{
    TCCR0B = 0;
    TCCR0A = 0; // resets the timer configuration and disconnects OC0A
    TCNT0 = 0;  // reset the counter
    PORTD &= ~(1 << PORTD6);
}

bool isPWMOn()
{
    return TCCR0B != 0;
}

void setPower(double power)
{
    if (power > 1 || power < 0)
    {
#ifdef SERIAL_ENABLED
        if (Serial)
        {
            Serial.print("Invalid power value: ");
            Serial.println(power);
        }
#endif
        return;
    }
#ifdef SERIAL_ENABLED
    else if (Serial)
    {
        Serial.print("Laser power set to: ");
        Serial.println(power);
    }
#endif

    // 0.375 duty -> 100 % laser power
    // 255 * 37.5 / 100 = 95,625
    setDuty(power * 0.375);
}

void setDuty(double duty)
{
    OCR0A = (int)(duty * 255);
}
