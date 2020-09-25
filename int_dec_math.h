#ifndef INT_DEC_MATH_H
#define INT_DEC_MATH_H


#include<math.h>
#include <stdint.h>
#include "fixed_math.h"

typedef int16_t FRACT16;
#define FRACT16_BIT     32
#define FRACT16_MAX     ((FRACT16)0x7FFF)
#define FRACT16_MIN     ((FRACT16)0x8000)
#define PI 3.1415926535897932384626433832795f
#define PI_ON_2 (PI/2)

typedef struct {
   int16_t integral;
   FRACT16 fractional;
}INT_FRACT32;

typedef struct{
  int16_t integral_re;
  FRACT16 fractional_re;
  int16_t integral_im;
  FRACT16 fractional_im;
}CMPLX_INT_FRACT32;

typedef int16_t FRACT32;

#ifndef DSP_ARCH_FLOAT_TO_FRACT32
/**
 * Converts a float to a FRACT16
 */
static FRACT16 float_to_fract16 (float _x) {
  FRACT32 res = 0;
  if (_x >= 1.0) return FRACT16_MAX;
  if (_x <= -1.0) return FRACT16_MIN;
  if (_x > 0) {
    double d = _x * FRACT16_MAX;
    res = (FRACT16)floor(d+0.5);
  }
  if (_x < 0) {
    double d = _x * 0x8000;
    res = (FRACT16)ceil(d-0.5);
  }
  return res;
}
#endif // DSP_ARCH_FLOAT_TO_FRACT32

static FRACT16 fract16_multr(FRACT16 a, FRACT16 b){

int32_t res = (int32_t)a* (int32_t)b;

int32_t round = (res>>14) & 1;
res=res>>15;
return ((FRACT16)(res + round));

}

static FRACT16 fract16_add(FRACT16 a, FRACT16 b){
  int32_t res = (int32_t)a + (int32_t)b;
  if(res > FRACT16_MAX) return FRACT16_MAX;
  if (res < FRACT16_MIN) return FRACT16_MIN;
  return (FRACT16)res;
}



#ifndef DSP_ARCH_FRACT16_TO_FLOAT
/**
 * Converts a FRACT16 to a float
 */
static float fract16_to_float (FRACT16 _x) {
  double res = 0.0;
  double v = _x;
  if (_x > 0) res =  v / FRACT16_MAX;
  if (_x < 0) res =  v / 0x8000;
  return (float)res;
}
#endif // DSP_ARCH_FRACT16_TO_FLOAT

#ifndef DSP_ARCH_FLOAT_TO_INT_FRACT32
/**
 * Converts a float to INT_FRACT32
 */
static INT_FRACT32 float_to_int_fract32(float x) {
  INT_FRACT32 converted_output;
  double fract_part,int_part;
  fract_part=modf((double)x,&int_part);
  if(int_part < -0x8000)int_part = 0x8000;
  if(int_part > 0x7FFF)int_part = 0x7FFF;
  converted_output.integral = (uint16_t)int_part;
  converted_output.fractional = float_to_fract16((float)fract_part);
  return converted_output;
}
#endif // DSP_ARCH_FLOAT_TO_INT_FRACT32

#ifndef DSP_INT_FRACT32_TO_FLOAT
/**
 * Converts a INT_FRACT32 to FLOAT
 */
static float int_fract32_to_float(INT_FRACT32 x) {

float fract_part = fract16_to_float(x.fractional);

float output_value = ((float)x.integral+fract_part);
  return output_value;
}
#endif // DSP_INT_FRACT32_TO_FLOAT


#ifndef DSP_ARCH_INT_FRACT32_ADD
/**
 * Converts a INT_FRACT32 to FLOAT
 */
static INT_FRACT32 int_fract32_add(INT_FRACT32 a ,INT_FRACT32 b) {
  INT_FRACT32 output;
  int32_t integral_result = a.integral+b.integral;
  int32_t fractional_result = (int32_t)a.fractional + (int32_t)b.fractional;

  if(fractional_result > FRACT16_MAX){
    output.fractional = (FRACT16)(fractional_result-FRACT16_MAX);
    integral_result++;
    output.integral = integral_result;
    return output;
  }
  if(fractional_result < FRACT16_MIN){
    output.fractional = (FRACT16)(fractional_result+FRACT16_MAX);
    integral_result--;
    output.integral = integral_result;
    return output;
  }
  output.integral = (int16_t)integral_result;
  output.fractional = (FRACT16)fractional_result;
return output;
}
#endif // DSP_ARCH_INT_FRACT32_ADD

#ifndef DSP_ARCH_INT_FRACT32_ADD

static INT_FRACT32 int_fract32_sub(INT_FRACT32 a ,INT_FRACT32 b) {
  INT_FRACT32 output;
  int32_t integral_result = a.integral-b.integral;
  int32_t fractional_result = (int32_t)a.fractional - (int32_t)b.fractional;

  if(fractional_result > FRACT16_MAX){
    output.fractional = (FRACT16)(fractional_result-FRACT16_MAX);
    integral_result++;
    output.integral = integral_result;
    return output;
  }
  if(fractional_result < FRACT16_MIN){
    output.fractional = (FRACT16)(fractional_result+FRACT16_MAX);
    integral_result--;
    output.integral = integral_result;
    return output;
  }
  output.integral = (int16_t)integral_result;
  output.fractional = (FRACT16)fractional_result;
return output;
}
#endif // DSP_ARCH_INT_FRACT32_ADD

#ifndef DSP_ARCH_INT_FRACT32_ADD

static INT_FRACT32 int_fract32_mult(INT_FRACT32 a ,INT_FRACT32 b) {
  INT_FRACT32 output;
  output.fractional=0;
  output.integral=0;
  int32_t integral_multi = (int32_t)a.integral * (int32_t)b.integral; // fine
  int32_t fractional_multi = (int32_t)a.fractional * (int32_t)b.fractional; // needscheck
  int32_t fract16_int_multi = (int32_t)a.fractional * (int32_t)b.integral; // needscheck
  int32_t integral_fract16_multi = (int32_t)a.integral * (int32_t)b.fractional; // neeedscheck
  int32_t integral_tmp=0;
  int32_t fractional_tmp=0;
  // Integer
  output.integral =(int16_t)integral_multi;
  integral_tmp=fract16_int_multi>>15;
  output.integral = output.integral+(int16_t)integral_tmp;
  integral_tmp=integral_fract16_multi>>15;
  output.integral = output.integral+(int16_t)integral_tmp;
  //Float
  //fractional_tmp=integral_fract16_multi+fract16_int_multi+fractional_multi;
  //integral_tmp=fractional_tmp>>15;
  //output.integral = output.integral+(int16_t)integral_tmp;
return output;
}
#endif // DSP_ARCH_INT_FRACT32_ADD
#endif

