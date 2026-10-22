#include "math_util.h"
#include <math.h>

#define _PI 3.14159265359f
#define _2PI 6.28318530718f

// 归一化角度到 [0,2PI]
float normalizeAngle(float angle){
  float a = fmodf(angle, _2PI);
  if(a < 0){
    a +=2*_PI;
  }
  return a;
}

float constrain(float val, float min, float max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}
