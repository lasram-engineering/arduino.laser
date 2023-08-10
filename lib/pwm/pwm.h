#ifndef PWM_H
#define PWM_H

#include "config.h"

/// Sets the power output of the laser
/// \param power the desired power as a fraction
void setPower(double power);

/// Sets the duty cycle
/// \param duty the duty cycle as a fraction
void setDuty(double duty);

void startPWM();

void stopPWM();

/// @brief use this method to check if the PWM is on
/// @return true if it is on, false otherwise
bool isPWMOn();

#endif