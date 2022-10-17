#include <Arduino.h>
#include <TimerInterrupt.h>

#include "pwm.h"
#include "serial.h"
#include "timer.h"

#define KEY_PWM "pwm"
#define KEY_DUTY "duty"
#define KEY_SHUTTER "shutter"

#define LASER_ACTIVE_PIN 8

#define ON "on"
#define OFF "off"

bool laserActive = false;
bool laserError = false;

void timerHandler()
{
    if (digitalRead(LASER_ACTIVE_PIN) != HIGH)
    {
        Serial.println("Error turning on laser");
    }

    stopTimer(); // stop the timer
}

void initSerial(int baud)
{
    Serial.begin(baud);
    initTimer();
    registerCallback(&timerHandler); // register the handler callback
}

bool processMessage(String message)
{
    bool commandIsValid = false;
    while (true)
    {
        // get the key
        int delimiterAt = message.indexOf("|");
        if (delimiterAt == -1)
            break;
        String key = message.substring(0, delimiterAt);
        message = message.substring(delimiterAt + 1); // remove the substring

        // get the value
        delimiterAt = message.indexOf("|");
        if (delimiterAt == -1)
            break;
        String value = message.substring(0, delimiterAt);

        commandIsValid = processCommand(key, value);
    }

    if (!commandIsValid)
    {
        Serial.println("invalid");
        return false;
    }

    return true;
}

bool processCommand(String key, String value)
{
    if (key == KEY_PWM)
    {
        if (value == ON)
        {
            Serial.println("PWM started");
            digitalWrite(BUILTIN_LED_PIN, HIGH);
            startPWM();
            startTimer(1);
        }
        else if (value == OFF)
        {
            Serial.println("PWM stopped");
            digitalWrite(BUILTIN_LED_PIN, LOW);
            stopPWM();
        }
        else
        {
            return false;
        }
    }
    else if (key == KEY_DUTY)
    {
        double duty = value.toDouble();
        Serial.println("Duty cycle set to " + value);
        setDuty(duty);
    }
    else if (key == KEY_SHUTTER)
    {
        if (value == ON)
        {
            Serial.println("Shutter on");
            digitalWrite(SHUTTER_PIN, HIGH);
        }
        else if (value == OFF)
        {
            Serial.println("Shutter off");
            digitalWrite(SHUTTER_PIN, LOW);
        }
        else
        {
            return false;
        }
    }
    return true;
}