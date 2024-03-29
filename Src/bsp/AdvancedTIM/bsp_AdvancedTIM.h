#ifndef __ADVANCED_TIM_H__
#define __ADVANCED_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

#define ADVANCED_TIMx                     TIM1
#define ADVANCED_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCED_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM1_CLK_DISABLE()
#define ADVANCED_TIM_IRQ                  TIM1_UP_IRQn
#define ADVANCED_TIM_UP_IRQ_FUN           TIM1_UP_IRQHandler


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ADVANCED_TIMx_PRESCALER+1）
#define ADVANCED_TIM_PRESCALER            36  // 实际时钟频率为：1MHz

// 定义定时器周期，当定时器开始计数到ADVANCED_TIMx_PERIOD值并且重复计数寄存器为0时更新定时器并生成对应事件和中断
#define ADVANCED_TIM_PERIOD               999  // 定时器产生中断频率为：1MHz/1000=1KHz，即1ms定时周期
// 定义高级定时器重复计数寄存器值，
#define ADVANCED_TIM_REPETITIONCOUNTER    299     // 1/1000= 1s

// 最终定时器频率计算为： 72MHz/（ADVANCED_TIMx_PRESCALER+1/（ADVANCED_TIM_REPETITIONCOUNTER+1）/ADVANCED_TIMx_PERIOD
// 比如需要产生20ms周期定时，可以设置为： 72MHz/（359+1）/（9+1）/400=50Hz，即20ms周期
// 这里设置 ADVANCED_TIMx_PRESCALER=359；ADVANCED_TIM_REPETITIONCOUNTER=9；ADVANCED_TIMx_PERIOD=400；

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef A_htimx;
/* 函数声明 ------------------------------------------------------------------*/

void ADVANCED_TIMx_Init(void);

#endif	/* __ADVANCED_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
