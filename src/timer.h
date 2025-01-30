#ifndef TIMER_H
#define TIMER_H

extern unsigned long timerDuration;
extern unsigned long breakTimerDuration;
extern unsigned long shortBreakDuration;
extern bool isShortBreak;
extern bool isLongBreak;
extern unsigned long startTime;

void updateTimers();

#endif