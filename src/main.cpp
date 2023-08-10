#include "Arduino.h"
#include "config.h"
#include "pwm.h"

#define BAUDRATE 9600
#define IN_SHUTTER_OPEN 2
#define IN_LASER_ENABLE 3

#define OUT_SHUTTER_OPEN 5
#define OUT_LASER_PWM 6

#define IN_POWER 7
#define POWER_DEPTH 6
#define POWER_RESOLUTION (pow(2, POWER_DEPTH) - 1)

bool laserOn = false;
bool shutterOn = false;

void initPins()
{
    // inputs
    pinMode(IN_LASER_ENABLE, INPUT_PULLUP);
    pinMode(IN_SHUTTER_OPEN, INPUT_PULLUP);
    for (int i = IN_POWER; i < IN_POWER + POWER_DEPTH; i++)
    {
        pinMode(i, INPUT_PULLUP);
    }

    // outputs
    pinMode(OUT_LASER_PWM, OUTPUT);
    pinMode(OUT_SHUTTER_OPEN, OUTPUT);
}

int getPowerFromPins()
{
    int power = 0b00000000;

    for (int i = IN_POWER + POWER_DEPTH - 1; i >= IN_POWER; i--)
    {
        int value = digitalRead(i) == LOGICAL_HIGH;
        power = power << 1;
        power |= value;

        // #ifdef SERIAL_ENABLED
        //         if (Serial)
        //         {
        //             Serial.print("Pin ");
        //             Serial.print(i);
        //             Serial.print(" is ");
        //             Serial.println(value);
        //         }
        // #endif
    }

    return power;
}

void handleLaser()
{
    laserOn = digitalRead(IN_LASER_ENABLE) == LOGICAL_HIGH;
    shutterOn = digitalRead(IN_SHUTTER_OPEN) == LOGICAL_HIGH;

    digitalWrite(OUT_SHUTTER_OPEN, shutterOn);

    int power = getPowerFromPins();

#ifdef SERIAL_ENABLED
    if (Serial)
    {
        Serial.print("Laser: ");
        Serial.println(laserOn);
        Serial.print("Shutter: ");
        Serial.println(shutterOn);
        Serial.print("Power: ");
        Serial.println(power / POWER_RESOLUTION);
    }
#endif

    // start the pwm
    if (laserOn && shutterOn)
    {

        if (power > 0)
        {
#ifdef SERIAL_ENABLED
            if (Serial)
            {
                Serial.print("Power is set to: ");
                Serial.println(power);
            }
#endif
            setPower(power / POWER_RESOLUTION);
            startPWM();
        }
    }

    // stop the pwm
    else if ((!laserOn || !shutterOn))
    {
#ifdef SERIAL_ENABLED
        if (Serial)
            Serial.println("Stopping PWM");
#endif
        // laserOn = false;
        // analogWrite(LASER_OUT_PIN, 0);

        stopPWM();
    }
}

void setup()
{
    cli(); // disable interrupts
#ifdef SERIAL_ENABLED
    Serial.begin(BAUDRATE);
#endif
    initPins();

    cli();
    attachInterrupt(digitalPinToInterrupt(IN_LASER_ENABLE), handleLaser, CHANGE);
    attachInterrupt(digitalPinToInterrupt(IN_SHUTTER_OPEN), handleLaser, CHANGE);

    sei(); // enable interrupts
    Serial.println("Device online");
}

void loop()
{
}
