#ifndef __PID_H_
#define __PID_H_

#include "stdio.h"

typedef struct _PID {
    float kp, kd, ki;
    float error, lastError;
    float interval, maxInterval;
    float output , maxOutput;
} PID;

void pid_Init(PID* pid, float kp, float kd, float ki);

void pid_InitWithMax(PID* pid, float kp, float kd, float ki, float maxInterval, float maxOutput);

void pid_calc(PID* pid, float reference, float feedback);

float pid_getPIDOutput(PID* pid);
#endif
