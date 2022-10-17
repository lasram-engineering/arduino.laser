#include "Arduino.h"

#include "pwm.h"
#include "serial.h"
#include "timer.h"

#define BAUDRATE 9600

/// Initializes the interrupts
void setupInterrupts()
{
    EIMSK |= (1 << INT0);                 // enable interrupt on INT0
    EICRA |= (1 << ISC00) | (1 << ISC01); // rising edge interrupt to INT0
}

void setup()
{
    cli(); // disable interrupts
    pinMode(SHUTTER_PIN, OUTPUT);

    initSerial(BAUDRATE);

    setupInterrupts();

    initPWM();
    setDuty(0.0);
    stopPWM();
    sei(); // enable interrupts
}

String message = "";

void loop()
{
    // check for incoming bytes
    while (Serial.available() > 0)
    {
        // read the incoming byte
        char incoming = (char)Serial.read();

        // check if the incoming byte is the terminator character
        if (incoming != '\n')
        {
            // add the incoming char to the message
            message += incoming;
        }
        else
        {
            // process the message
            bool success = processMessage(message);
            // reset the message buffer
            message = "";
        }
    }
}

/// interrupt service routine for the int0 pin (D2)
ISR(INT0_vect)
{
    /// Laser error pin
    // Serial.println("Laser error");
    laserError = true;
}