#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f4xx.h"   
#include "pwm.h"

// calculate the electrical angle
static float _electricalAngle(float shaft_angle, int pole_pairs);
//normalize the angle to [0,2pi]
static float _normalizeAngle(float angle);
// set PWM
static void setPwm(float Ua, float Ub, float Uc);

void setPhaseVoltage(float Uq,float angle_el);

#endif