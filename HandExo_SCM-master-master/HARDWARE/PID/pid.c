#include "pid.h"

void pid_Init(PID* pid, float kp, float kd, float ki) {
    pid->kp = kp;
    pid->kd = kd;
    pid->ki = ki;
    pid->maxOutput = 1e9;
    pid->maxInterval = 1e9;
    pid->output = 0;
    pid->interval = 0;
    printf("pid construction function called\n");
}
void pid_InitWithMax(PID* pid, float kp, float kd, float ki, float maxInterval, float maxOutput) {
    pid->kp = kp;
    pid->kd = kd;
    pid->ki = ki;
    pid->maxOutput = maxOutput;
    pid->maxInterval = maxInterval;
    pid->output = 0;
    pid->interval = 4000;
    printf("pid construction function called\n");
}
void pid_calc(PID* pid, float reference, float feedback) {
	
	
	float DOut, POut;
    pid->lastError = pid->error;
    pid->error = reference - feedback;

    DOut = (pid->error - pid->lastError) * pid->kd;
    POut = (pid->error) * pid->kp;

    pid->interval += pid->error * pid->ki;
    if (pid->interval > pid->maxInterval) {
        pid->interval = pid->maxInterval;
    } else if (pid->interval < -pid->maxInterval) {
        pid->interval = -pid->maxInterval;
    }

    pid->output = DOut + POut + pid->interval;

    if (pid->output > pid->maxOutput) {
        pid->output = pid->maxOutput;
    } else if (pid->output < - pid->maxOutput) {
        pid->output = -pid->maxOutput;
    }

}


float pid_getPIDOutput(PID* pid) {
    return pid->output;
}
