#ifndef _MYDEF_H
#define _MYDEF_H

/* 浮点数值定义 */
#define _PI 3.14159265359f
#define _2PI 6.28318530718f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _3PI_2 4.71238898038f
#define _SQRT3 1.73205080757f
#define _SQRT3_2 0.86602540378f
#define _1_SQRT3 0.57735026919f
#define _2_SQRT3 1.15470053838f

/* 弧度/角度 */
#define DEG_TO_RAD (_PI / 180.0f)
#define RAD_TO_DEG (180.0f / _PI)

/* -----------------------------电机宏定义---------------------------------- */
#define MOTOR_LEG_WHEEL 0 //0为足，1为轮
#define MOTOR_LEFT_RIGHT 1 //0为左，1为右
#if MOTOR_LEG_WHEEL == 0
/* GIM6010-8 */
#if MOTOR_LEFT_RIGHT == 0
//left
#define DIR -1 //电机方向/编码器方向
#define PP 14  //电机极对数
#define G 8.0f //减速比
#define POS_LIMIT_MAX (-0.16388642f) // -9.39°，收腿端
#define POS_LIMIT_MIN (-0.72588244f) // -41.59°，伸腿端
#define ControlID 0x101
#else
//right
#define DIR -1 //电机方向/编码器方向
#define PP 14  //电机极对数
#define G 8.0f //减速比
#define POS_LIMIT_MAX (-0.05550147f) // -3.18°，伸腿端
#define POS_LIMIT_MIN (-0.61749749f) // -35.38°，收腿端
#define ControlID 0x103
#endif

#else
/* M0603A_111 */
#if MOTOR_LEFT_RIGHT == 0
//left
#define DIR 1 //电机方向/编码器方向
#define PP 10  //电机极对数
#define G 1.0f //减速比
#define ControlID 0x102
#else
//right
#define DIR -1 //电机方向/编码器方向
#define PP 10  //电机极对数
#define G 1.0f //减速比
#define ControlID 0x104
#endif
#endif

#if MOTOR_LEG_WHEEL == 0
#define LEG_TRAVEL_RAD (POS_LIMIT_MAX - POS_LIMIT_MIN)/* 总行程32.2° ≈ 0.56199602 rad */
#if MOTOR_LEFT_RIGHT == 0
/* 左腿：原坐标减小为伸腿 */
#define LEG_RETRACT_POS_RAD POS_LIMIT_MAX
#define LEG_EXTEND_SIGN     (-1.0f)
#else
/* 右腿：原坐标增加为伸腿 */
#define LEG_RETRACT_POS_RAD POS_LIMIT_MIN
#define LEG_EXTEND_SIGN     (1.0f)
#endif
#endif

/* -----------------------------周期---------------------------------- */
#define Ts_currentLoop 5e-5f
#define Ts_velocityLoop 5e-4f
#define Ts_positionLoop 5e-3f
#define Ts_T0 5e-5f

#endif
