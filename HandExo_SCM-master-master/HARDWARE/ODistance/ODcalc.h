//
// Created by 包洋斌 on 2024/3/2.
//

#ifndef ODISTANCE_ODCALC_H
#define ODISTANCE_ODCALC_H

#define pi 3.1415926535
typedef struct TransMatrix {
    double R1_1,R1_2,R1_3,P1_4;
    double R2_1,R2_2,R2_3,P2_4;
    double R3_1,R3_2,R3_3,P3_4;

}TransMatrix;

typedef struct Jacobe {
	double d1, d2, d3;
	double r1, r2, r3;
} Jacobe;

typedef struct FT{
	double fx, fy, fz;
	double tx, ty, tz;
} FT;

void TM_calc(struct TransMatrix* TM,float degree[]);
float simplified_getT14(const float angle[6]);
Jacobe GetJacobe(TransMatrix * TM);
FT TransFT(TransMatrix TM, FT oriFT);
float GetTorch(Jacobe J, FT transFT);

#endif //ODISTANCE_ODCALC_H
