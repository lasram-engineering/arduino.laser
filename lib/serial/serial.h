#ifndef SERIAL_H
#define SERIAL_H
#include <Arduino.h>

#define SHUTTER_PIN 4
#define BUILTIN_LED_PIN 13

// the pin number of the laser active signal
#define LASER_ACTIVE_PIN 2

extern bool laserActive;
extern bool laserError;

void initSerial(int baud);

/// Processes the command given in a key-value pair
bool processCommand(String key, String value);

/// Processes the incoming message
///
/// Message syntax: (UltraLight 2.0):
///     <key>|<value>|
///
/// The available commands for the PWM signals are:
///     duty|<value (float)>|
///     shutter|<'on' or 'off'>|
///     pwm|<'on' or 'off'>|
///
/// \returns true if the processing was successful else false
bool processMessage(String message);

#endif