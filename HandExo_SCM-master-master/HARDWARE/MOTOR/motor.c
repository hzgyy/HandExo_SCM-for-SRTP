#include "motor.h"
#include <math.h>
#define PI 3.1415926535897932384626433832795

//?????????
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
float voltage_power_supply=12;
float shaft_angle=0,open_loop_timestamp=0;
float zero_electric_angle=0,Ualpha,Ubeta=0,Ua=0,Ub=0,Uc=0,dc_a=0,dc_b=0,dc_c=0;

// calculate the electrical angle
static float _electricalAngle(float shaft_angle, int pole_pairs) {
  return (shaft_angle * pole_pairs);
}

//normalize the angle to [0,2pi]
static float _normalizeAngle(float angle){
  float a = fmod(angle, 2*PI);   //???????????,??????????
  return a >= 0 ? a : (a + 2*PI);  
}

// set PWM
static void setPwm(float Ua, float Ub, float Uc) {
  dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f );
  dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

	Set_TIM1_PWM_DutyCycle(dc_a*1000,dc_b*1000,dc_c*1000);
}

void setPhaseVoltage(float Uq,float angle_el) {
  angle_el = _normalizeAngle(angle_el + zero_electric_angle);
  // Parker Transform
  Ualpha =  -Uq*sin(angle_el); 
  Ubeta =   Uq*cos(angle_el); 

  // Clark Transform
  Ua = Ualpha + voltage_power_supply/2;
  Ub = (sqrt(3)*Ubeta-Ualpha)/2 + voltage_power_supply/2;
  Uc = (-Ualpha-sqrt(3)*Ubeta)/2 + voltage_power_supply/2;
  setPwm(Ua,Ub,Uc);
}

