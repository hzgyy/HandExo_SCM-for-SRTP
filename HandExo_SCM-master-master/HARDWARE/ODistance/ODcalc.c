 //
// Created by 包洋斌 on 2024/3/2.
//modified by gyy on 2024/9/29
//
#include "ODcalc.h"
#include "math.h"

void TM_calc(struct TransMatrix* TM,float degree[])
{
		//angle5 changed
    //float t1 = -degree[0]-79 , t2 = -degree[1]+180, t3 = degree[2]+90, t4 = -degree[3], t5 = degree[4]-90, t6 = -degree[5]-70;
		//mine version - - - - + -
		float t1 = -degree[0]-79 , t2 = -degree[1]+180, t3 = -degree[2]+90, t4 = -degree[3], t5 = degree[4]-90, t6 = -degree[5]-70;
    float l1 = 74, l2 = 33, l3 = 44, l5 = 13, l6 = 5,beta = 60;
		float d2r = pi/180;
		//change to rad
		t1*= d2r;t2*= d2r;t3*= d2r;t4*= d2r;t5*= d2r;t6*= d2r;beta*=d2r;
		/*
    TM->R1_1 = cosf(t5)*cosf(t6) * (cosf(t1+t2)*cosf(t3)*cosf(t4)-0.5*cosf(t1+t2)*sinf(t3)*sinf(t4)+sqrtf(3)/2*sinf(t4)*sinf(t1+t2)) -\
               sinf(t6) * (sqrtf(3)/2*sinf(t1+t2)*cosf(t4)-cosf(t1+t2)*cosf(t3)*sinf(t4)-0.5*cosf(t1+t2)*sinf(t3)*cosf(t4)) -\
               sinf(t5)*cosf(t6) * (sqrtf(3)/2*cosf(t1+t2)*sinf(t3)+0.5*sinf(t1+t2));

    TM->R1_2 = -sinf(t5) * (cosf(t1+t2)*cosf(t3)*cosf(t4)-0.5*cosf(t1+t2)*sinf(t3)*sinf(t4)+ sqrtf(3)/2* sinf(t4)*sinf(t1+t2)) -\
               cosf(t5) * (sqrtf(3)/2*cosf(t1+t2)*sinf(t3)+0.5*sinf(t1+t2));

    TM->R1_3 = cosf(t5)*sinf(t6) * (cosf(t1+t2)*cosf(t3)*cosf(t4)-0.5*cosf(t1+t2)*sinf(t3)*sinf(t4)+ sqrtf(3)/2* sinf(t4)*sinf(t1+t2)) +\
               cosf(t6) * (sqrtf(3)/2*sinf(t1+t2)*cosf(t4)-cosf(t1+t2)*cosf(t3)*sinf(t4)-0.5*cosf(t1+t2)*sinf(t3)*cosf(t4)) -\
               sinf(t5)*sinf(t6) * (sqrtf(3)/2*cosf(t1+t2)*sinf(t3)+0.5*sinf(t1+t2));

    TM->P1_4 = (cosf(t5)*l5 - cosf(t5)*sinf(t6)*l6) * (cosf(t1+t2)*cosf(t3)*cosf(t4)-0.5*cosf(t1+t2)*sinf(t3)*sinf(t4)+sqrtf(3)/2*sinf(t4)*sinf(t1+t2)) -\
               cosf(t6)*l6 * (sqrtf(3)/2*sinf(t1+t2)*cosf(t4)-cosf(t1+t2)*cosf(t3)*sinf(t4)-0.5*cosf(t1+t2)*sinf(t3)*cosf(t4)) +\
               (sinf(t5)*sinf(t6)*l6-sinf(t5)*l5) * (sqrtf(3)/2*cosf(t1+t2)*sinf(t3)+0.5*sinf(t1+t2)) +\
               (sqrtf(3)/2*cosf(t1+t2)*sinf(t3)*l3 + 0.5*sinf(t1+t2)*l3 + l2*sinf(t1+t2) + cosf(t1)*l1);

    /////////////////////

    TM->R2_1 =  cosf(t5)*cosf(t6) * (sinf(t1+t2)*cosf(t3)*cosf(t4)-0.5*sinf(t1+t2)*sinf(t3)*sinf(t4)- sqrtf(3)/2*sinf(t4)*cosf(t1+t2)) +\
                sinf(t6) * (sinf(t1+t2)*cosf(t3)*sinf(t4)+0.5*sinf(t1+t2)*sinf(t3)*cosf(t4)+ sqrtf(3)/2*cosf(t1+t2)*cosf(t4)) -\
                sinf(t5)*cosf(t6) * (sqrtf(3)/2*sinf(t1+t2)*sinf(t3)-0.5*cosf(t1+t2));

    TM->R2_2 = -sinf(t5) * (sinf(t1+t2)*cosf(t3)*cosf(t4)-0.5*sinf(t1+t2)*sinf(t3)*sinf(t4)- sqrtf(3)/2*sinf(t4)*cosf(t1+t2)) -\
               cosf(t5) * (sqrtf(3)/2*sinf(t1+t2)*sinf(t3)-0.5*cosf(t1+t2));

    TM->R2_3 = cosf(t5)*sinf(t6) * (sinf(t1+t2)*cosf(t3)*cosf(t4)-0.5*sinf(t1+t2)*sinf(t3)*sinf(t4)- sqrtf(3)/2*sinf(t4)*cosf(t1+t2)) -\
               cosf(t6) * (sinf(t1+t2)*cosf(t3)*sinf(t4)+0.5*sinf(t1+t2)*sinf(t3)*cosf(t4)+ sqrtf(3)/2*cosf(t1+t2)*cosf(t4)) -\
               sinf(t5)*sinf(t6) * (sqrtf(3)/2*sinf(t1+t2)*sinf(t3)-0.5*cosf(t1+t2));

    TM->P2_4 = (cosf(t5)*l5 - cosf(t5)*sinf(t6)*l6) * (sinf(t1+t2)*cosf(t3)*cosf(t4)-0.5*sinf(t1+t2)*sinf(t3)*sinf(t4)- sqrtf(3)/2*sinf(t4)*cosf(t1+t2)) +\
                cosf(t6)*l6 * (sinf(t1+t2)*cosf(t3)*sinf(t4)+0.5*sinf(t1+t2)*sinf(t3)*cosf(t4)+ sqrtf(3)/2*cosf(t1+t2)*cosf(t4)) +\
                (sinf(t5)*sinf(t6)*l6-sinf(t5)*l5) * (sqrtf(3)/2*sinf(t1+t2)*sinf(t3)-0.5*cosf(t1+t2)) +\
                (sqrtf(3)/2*sinf(t1+t2)*sinf(t3)*l3 - 0.5*cosf(t1+t2)*l3 - l2*cosf(t1+t2) + sinf(t1)*l1);

    ///////////////////

    TM->R3_1 = cosf(t5)*cosf(t6) * (sinf(t3)*cosf(t4)+0.5*cosf(t3)*sinf(t4)) -\
               sinf(t6) * (0.5*cosf(t3)*cosf(t4)-sinf(t3)*sinf(t4)) +\
               (cosf(t6) *sinf(t5)) * (sqrtf(3)/2*cosf(t3)) ;

    TM->R3_2 = -sinf(t5) * (sinf(t3)*cosf(t4)+0.5*cosf(t3)*sinf(t4)) +\
               cosf(t5) * (sqrtf(3)/2*cosf(t3));

    TM->R3_3 = cosf(t5)*sinf(t6) * (sinf(t3)*cosf(t4)+0.5*cosf(t3)*sinf(t4)) +\
               cosf(t6) * (0.5*cosf(t3)*cosf(t4)-sinf(t3)*sinf(t4)) +\
               (sinf(t6) *sinf(t5)) * (sqrtf(3)/2*cosf(t3));

    TM->P3_4 = (cosf(t5)*l5 - cosf(t5)*sinf(t6)*l6)  * (sinf(t3)*cosf(t4)+0.5*cosf(t3)*sinf(t4))-\
                cosf(t6)*l6 * (0.5*cosf(t3)*cosf(t4)-sinf(t3)*sinf(t4)) -\
                (sinf(t5)*sinf(t6)*l6-sinf(t5)*l5) * (sqrtf(3)/2*cosf(t3)) -\
                sqrtf(3)/2*cosf(t3)*l5	;
		*/
		TM->R1_1 = sinf(t6)*(cosf(t3)*sinf(t4)*cosf(t1+t2) - sinf(beta)*cosf(t4)*sinf(t1+t2) +\
     cosf(beta)*cosf(t4)*sinf(t3)*cosf(t1+t2)) - cosf(t6)*(sinf(t5)*(cosf(beta)*sinf(t1+t2) \
     + sinf(beta)*sinf(t3)*cosf(t1+t2)) - cosf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) + \
     sinf(beta)*sinf(t4)*sinf(t1+t2) - cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2)));
     
    TM->R1_2=- cosf(t5)*(cosf(beta)*sinf(t1+t2) + sinf(beta)*sinf(t3)*cosf(t1+t2)) - \
    sinf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) + sinf(beta)*sinf(t4)*sinf(t1+t2) - \
    cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2));

    TM->R1_3= - sinf(t6)*(sinf(t5)*(cosf(beta)*sinf(t1+t2) + sinf(beta)*sinf(t3)*cosf(t1+t2)) - \
    cosf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) + sinf(beta)*sinf(t4)*sinf(t1+t2) - \
    cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2))) - cosf(t6)*(cosf(t3)*sinf(t4)*cosf(t1+t2) - \
    sinf(beta)*cosf(t4)*sinf(t1+t2) + cosf(beta)*cosf(t4)*sinf(t3)*cosf(t1+t2));

    TM->P1_4= l2*sinf(t1+t2) - 20*cosf(t6)*(sinf(t5)*(cosf(beta)*sinf(t1+t2) + sinf(beta)*sinf(t3)*cosf(t1+t2)) - cosf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) + \
    sinf(beta)*sinf(t4)*sinf(t1+t2) - cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2))) +\
    28*sinf(t6)*(sinf(t5)*(cosf(beta)*sinf(t1+t2) + sinf(beta)*sinf(t3)*cosf(t1+t2))\
     - cosf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) + sinf(beta)*sinf(t4)*sinf(t1+t2) - \
    cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2))) + 28*cosf(t6)*(cosf(t3)*sinf(t4)*cosf(t1+t2) -\
     sinf(beta)*cosf(t4)*sinf(t1+t2) + cosf(beta)*cosf(t4)*sinf(t3)*cosf(t1+t2)) + l1*cosf(t1) + \
    20*sinf(t6)*(cosf(t3)*sinf(t4)*cosf(t1+t2) - sinf(beta)*cosf(t4)*sinf(t1+t2) + \
    cosf(beta)*cosf(t4)*sinf(t3)*cosf(t1+t2)) - l5*(sinf(t5)*(cosf(beta)*sinf(t1+t2) + \
    sinf(beta)*sinf(t3)*cosf(t1+t2)) - cosf(t5)*(cosf(t3)*cosf(t4)*cosf(t1+t2) +\
     sinf(beta)*sinf(t4)*sinf(t1+t2) - cosf(beta)*sinf(t3)*sinf(t4)*cosf(t1+t2))) +\
     l3*cosf(beta)*sinf(t1+t2) + l3*sinf(beta)*sinf(t3)*cosf(t1+t2);
 
    TM->R2_1=  cosf(t6)*(sinf(t5)*(cosf(beta)*cosf(t1+t2) - sinf(beta)*sinf(t3)*sinf(t1+t2)) -\
     cosf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - cosf(t3)*cosf(t4)*sinf(t1+t2) + \
    cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2))) + sinf(t6)*(sinf(beta)*cosf(t4)*cosf(t1+t2) + \
    cosf(t3)*sinf(t4)*sinf(t1+t2) + cosf(beta)*cosf(t4)*sinf(t3)*sinf(t1+t2));

    TM->R2_2 =  cosf(t5)*(cosf(beta)*cosf(t1+t2) - sinf(beta)*sinf(t3)*sinf(t1+t2)) + \
    sinf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - cosf(t3)*cosf(t4)*sinf(t1+t2) + \
    cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2));

    TM->R2_3= sinf(t6)*(sinf(t5)*(cosf(beta)*cosf(t1+t2) - sinf(beta)*sinf(t3)*sinf(t1+t2)) -\
     cosf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - cosf(t3)*cosf(t4)*sinf(t1+t2) +\
     cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2))) - cosf(t6)*(sinf(beta)*cosf(t4)*cosf(t1+t2) + \
    cosf(t3)*sinf(t4)*sinf(t1+t2) + cosf(beta)*cosf(t4)*sinf(t3)*sinf(t1+t2));

    TM->P2_4= 20*cosf(t6)*(sinf(t5)*(cosf(beta)*cosf(t1+t2) - sinf(beta)*sinf(t3)*sinf(t1+t2)) - \
    cosf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - cosf(t3)*cosf(t4)*sinf(t1+t2) +cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2))) - l2*cosf(t1+t2) - \
    28*sinf(t6)*(sinf(t5)*(cosf(beta)*cosf(t1+t2) - sinf(beta)*sinf(t3)*sinf(t1+t2)) - \
    cosf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - cosf(t3)*cosf(t4)*sinf(t1+t2) + \
    cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2))) + 28*cosf(t6)*(sinf(beta)*cosf(t4)*cosf(t1+t2) + \
    cosf(t3)*sinf(t4)*sinf(t1+t2) + cosf(beta)*cosf(t4)*sinf(t3)*sinf(t1+t2)) + \
    20*sinf(t6)*(sinf(beta)*cosf(t4)*cosf(t1+t2) + cosf(t3)*sinf(t4)*sinf(t1+t2) + \
    cosf(beta)*cosf(t4)*sinf(t3)*sinf(t1+t2)) + l1*sinf(t1) + l5*(sinf(t5)*(cosf(beta)*cosf(t1+t2) - \
    sinf(beta)*sinf(t3)*sinf(t1+t2)) - cosf(t5)*(sinf(beta)*sinf(t4)*cosf(t1+t2) - \
    cosf(t3)*cosf(t4)*sinf(t1+t2) + cosf(beta)*sinf(t3)*sinf(t4)*sinf(t1+t2))) - \
    l3*cosf(beta)*cosf(t1+t2) + l3*sinf(beta)*sinf(t3)*sinf(t1+t2);

    TM->R3_1 = cosf(t6)*(cosf(t5)*(cosf(t4)*sinf(t3) + cosf(beta)*cosf(t3)*sinf(t4)) + sinf(beta)*cosf(t3)*sinf(t5)) + sinf(t6)*(sinf(t3)*sinf(t4) -\
     cosf(beta)*cosf(t3)*cosf(t4));

    TM->R3_2=  sinf(beta)*cosf(t3)*cosf(t5) - sinf(t5)*(cosf(t4)*sinf(t3) + cosf(beta)*cosf(t3)*sinf(t4));

    TM->R3_3= sinf(t6)*(cosf(t5)*(cosf(t4)*sinf(t3) + cosf(beta)*cosf(t3)*sinf(t4)) + sinf(beta)*cosf(t3)*sinf(t5)) - cosf(t6)*(sinf(t3)*sinf(t4) \
    - cosf(beta)*cosf(t3)*cosf(t4));

    TM->P3_4=  20*cosf(t6)*(cosf(t5)*(cosf(t4)*sinf(t3) + cosf(beta)*cosf(t3)*sinf(t4)) + sinf(beta)*cosf(t3)*sinf(t5)) - \
    28*sinf(t6)*(cosf(t5)*(cosf(t4)*sinf(t3) + cosf(beta)*cosf(t3)*sinf(t4)) + sinf(beta)*cosf(t3)*sinf(t5)) + l5*(cosf(t5)*(cosf(t4)*sinf(t3) + \
    cosf(beta)*cosf(t3)*sinf(t4)) + sinf(beta)*cosf(t3)*sinf(t5)) + 28*cosf(t6)*(sinf(t3)*sinf(t4) - cosf(beta)*cosf(t3)*cosf(t4)) + \
    20*sinf(t6)*(sinf(t3)*sinf(t4) - cosf(beta)*cosf(t3)*cosf(t4)) - l3*sinf(beta)*cosf(t3);
}


float simplified_getT14(const float angle[6])
{
    float t1 = -angle[0]-79 , t2 = -angle[1]+180, t3 = -angle[2]+90, t4 = -angle[3], t5 = angle[4]-90, t6 = -angle[5]-70;
    float l1 = 74, l2 = 33, l3 = 44, l5 = 13, l6 = 5;
	float d2r = pi/180;
	float T14;
	
	t1 *= d2r; t2 *= d2r; t3 *= d2r; t4 *= d2r; t5 *= d2r; t6 *= d2r;
    

    T14 = cosf(t3)*cosf(t1+t2) * (cosf(t4)*cosf(t5)*l5 + (sinf(t4)*cosf(t6)-cosf(t4)*cosf(t5)*cosf(t6))*l6) +
            (-sinf(t3)*cosf(t1+t2)) * ((float)0.5*sinf(t4)*cosf(t5)*l5 + sqrtf(3)/2*l5*sinf(5) - sqrtf(3)/2*l3 + l6*(-(float)0.5*sinf(t4)*cosf(t5)*sinf(t6)-(float)0.5*cosf(t4)*cosf(t6)-sqrtf(3)/2*sinf(t5)*sinf(t6))) +
            sinf(t1+t2)*(sqrtf(3)/2*sinf(t4)*cosf(t5)*l5-(float)0.5*l5*sinf(t5)+(float)0.5*l3 + l6*(-sqrtf(3)/2*sinf(t4)*cosf(t5)*sinf(t6)-sqrtf(3)/2*cosf(t4)*cosf(t6)-(float)0.5*sinf(t5)*sinf(t6))) +
            (cosf(t1)*l1+sinf(t1+t2)*l2);


    return T14;
}
Jacobe GetJacobe(TransMatrix * TM)
{
	Jacobe J;
	J.d1 = -TM->P2_4;
	J.d2 = TM->P1_4;
	J.d3 = 0;
	J.r1 = 0;
	J.r2 = 0;
	J.r3 = 1;
	return J;
}
FT TransFT(TransMatrix TM, FT oriFT)
{
	FT TransFt;
	TransFt.fx = TM.R1_1 * oriFT.fx + TM.R1_2 * oriFT.fy + TM.R1_3 * oriFT.fz;
	TransFt.fy = TM.R2_1 * oriFT.fx + TM.R2_2 * oriFT.fy + TM.R2_3 * oriFT.fz;
	TransFt.fz = TM.R3_1 * oriFT.fx + TM.R3_2 * oriFT.fy + TM.R3_3 * oriFT.fz;
	TransFt.tx = 0;
	TransFt.ty = 0;
	TransFt.tz = 0;
	return TransFt;
}
float GetTorch(Jacobe J, FT transFT)
{
	float torch;
	torch = J.d1*transFT.fx + J.d2*transFT.fy + J.d3*transFT.fz + J.r1*transFT.tx + J.r2*transFT.ty + J.r3*transFT.tz;
	return torch;
}
