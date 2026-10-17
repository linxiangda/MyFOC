#include "filter.h"
#include <stdbool.h>

float y_prev1;
float y_prev2;
float y_prev3;

float LowPassFilter1(float x,float ts,float alpha){
  static bool first = true;

  if(first == true){
    first = false;
    y_prev1 = x;
    return x;
  }

  y_prev1 = alpha * y_prev1 + (1.0f - alpha) * x;
  return y_prev1;
}

float LowPassFilter2(float x,float ts,float alpha){
  static bool first = true;

  if(first == true){
    first = false;
    y_prev2 = x;
    return x;
  }

  y_prev2 = alpha * y_prev2 + (1.0f - alpha) * x;
  return y_prev2;
}

float LowPassFilter3(float x,float ts,float alpha){
  static bool first = true;

  if(first == true){
    first = false;
    y_prev3 = x;
    return x;
  }

  y_prev3 = alpha * y_prev3 + (1.0f - alpha) * x;
  return y_prev3;
}
