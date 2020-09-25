#ifndef FIXED_MATH_H
#define FIXED_MATH_H

#include<math.h>
#include <stdint.h>


#define BUFFSIZE 512

typedef int32_t FIXED_32;



typedef struct {
  FIXED_32 re;
  FIXED_32 im;
} COMPLEX_FIXED_32;

static FIXED_32 to_fixed32(float x){
  uint8_t shift = 16;
  return (FIXED_32)(x*(1<<shift));
}

static float from_fixed32(FIXED_32 x){
  uint8_t shift = 16;
    return (float)(x * (1.f / (1 << shift)));
}

static FIXED_32 fixed32_mult(FIXED_32 m1, FIXED_32 m2){
    uint8_t shift = 16;
    int64_t p1 = (m1 >> shift) * (int64_t)m2;
    int64_t p2 = ((m1 & 0xFFFF) * (int64_t)m2) >> shift;
    return p1 + p2;
}

static FIXED_32 fixed32_add(FIXED_32 m1, FIXED_32 m2){
    int64_t p1 = (int64_t)m1+(int64_t)m2;
    return p1;
}

static FIXED_32 fixed32_sub(FIXED_32 m1, FIXED_32 m2){
    int64_t p1 = (int64_t)m1-(int64_t)m2;
    return p1;
}



#endif

