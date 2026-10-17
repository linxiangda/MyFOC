#include "Flash.h"

/* 校准零位存储地址：256KB Flash 的最后一页起始(GD32F303CCT6) */
#define CAL_FLASH_ADDR   0x0803FC00UL
/* 校验魔数：用于判断该位置是否已写入过有效校准数据 */
#define CAL_MAGIC        0x5A5AC1B0UL

/* float与uint32共用的联合体，用于把float的位模式当作32位整数写入Flash */
typedef union {
    float    f;   /* float视角 */
    uint32_t u;   /* 32位整数视角 */
} FloatU32;

/**
 * @brief 把校准零位写入Flash
 * @param zero_angle 校准得到的电角度零位
 * @return none
 * @attention 会擦除整页(1KB)，只在校准时调用一次
 */
void Flash_Cal_Write(float zero_angle)
{
    FloatU32 v;
    v.f = zero_angle;

    fmc_unlock();                                        /* 解锁FMC */
    if (fmc_page_erase(CAL_FLASH_ADDR) == FMC_READY) {   /* 擦除整页(1KB) */
        fmc_word_program(CAL_FLASH_ADDR + 0, CAL_MAGIC); /* 写魔数 */
        fmc_word_program(CAL_FLASH_ADDR + 4, v.u);       /* 写零位(float位模式) */
    }
    fmc_lock();                                          /* 上锁FMC */
}

/**
 * @brief 从Flash读取校准零位
 * @param zero_angle 读出零位的指针
 * @return 1:读取成功 0:未校准或数据损坏
 * @attention 通过魔数判断数据是否有效
 */
uint8_t Flash_Cal_Read(float *zero_angle)
{
    if (*(volatile uint32_t *)CAL_FLASH_ADDR != CAL_MAGIC)   /* 没写过，全为0xFFFFFFFF */
        return 0;

    FloatU32 v;
    v.u = *(volatile uint32_t *)(CAL_FLASH_ADDR + 4);
    *zero_angle = v.f;
    return 1;
}
