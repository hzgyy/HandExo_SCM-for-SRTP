#ifndef __USMART_STR_H
#define __USMART_STR_H	 
#include "stm32f4xx.h"

u8 usmart_get_parmpos(u8 num);								//Obtain the starting position of a parameter in the parameter column
u8 usmart_strcmp(u8*str1,u8 *str2);							//Compare whether two strings are equal
u32 usmart_pow(u8 m,u8 n);									//M^N
u8 usmart_str2num(u8*str,u32 *res);							//Convert string to number
u8 usmart_get_cmdname(u8*str,u8*cmdname,u8 *nlen,u8 maxlen);//Obtain the instruction name from str and return the instruction length
u8 usmart_get_fname(u8*str,u8*fname,u8 *pnum,u8 *rval);		//Get the function name from str
u8 usmart_get_aparm(u8 *str,u8 *fparm,u8 *ptype); 			//Obtain a function parameter from str
u8 usmart_get_fparam(u8*str,u8 *parn);  					//Obtain all function parameters in str
#endif











