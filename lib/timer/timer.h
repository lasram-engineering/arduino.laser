#ifndef TIMER_H
#define TIMER_H

void initTimer();
void startTimer(int seconds);
void stopTimer();

void registerCallback(void (*callback)());

#endif