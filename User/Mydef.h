#ifndef _MYDEF_H
#define _MYDEF_H

/* GIM6010-8 */
// #define DIR -1 //电机方向/编码器方向
// #define PP 14  //电机极对数
// #define G 8.0f //减速比

/* M0603A_111 */
#define DIR 1 //电机方向/编码器方向
#define PP 10  //电机极对数
#define G 1.0f //减速比

#define _PI 3.14159265359f
#define _2PI 6.28318530718f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _3PI_2 4.71238898038f

#define _SQRT3 1.73205080757f
#define _SQRT3_2 0.86602540378f
#define _1_SQRT3 0.57735026919f
#define _2_SQRT3 1.15470053838f

//right
// #define POS_LIMIT_MAX -0.49
// #define POS_LIMIT_MIN -4.97
//left
#define POS_LIMIT_MAX -1.33
#define POS_LIMIT_MIN -5.00

#define Ts_currentLoop 5e-5f
#define Ts_velocityLoop 5e-4f
#define Ts_positionLoop 5e-3f
#define Ts_T0 5e-5f
#endif
