/**
  ******************************************************************************
  * 文件名程: bsp_AdvancedTIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 高级定时器TIM1 & TIM8底层驱动程序
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "AdvancedTIM/bsp_AdvancedTIM.h" 
#include "GeneralTIM/bsp_GeneralTIM.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef A_htimx;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 基本定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ADVANCED_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  
  A_htimx.Instance = ADVANCED_TIMx;
  A_htimx.Init.Prescaler = ADVANCED_TIM_PRESCALER;
  A_htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  A_htimx.Init.Period = ADVANCED_TIM_PERIOD;
  A_htimx.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  A_htimx.Init.RepetitionCounter = ADVANCED_TIM_REPETITIONCOUNTER; 
  HAL_TIM_Base_Init(&A_htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&A_htimx, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&A_htimx, &sMasterConfig);
  
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base){
  if(htim_base->Instance==ADVANCED_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    ADVANCED_TIM_RCC_CLK_ENABLE();

    /* 外设中断配置 */
    HAL_NVIC_SetPriority(ADVANCED_TIM_IRQ, 1, 0);
    HAL_NVIC_EnableIRQ(ADVANCED_TIM_IRQ);
  }
	else if(htim_base->Instance==GENERAL_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    GENERAL_TIM_RCC_CLK_ENABLE();
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ADVANCED_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    ADVANCED_TIM_RCC_CLK_DISABLE();

    /* 关闭外设中断 */
    HAL_NVIC_DisableIRQ(ADVANCED_TIM_IRQ);
  }
	else if(htim_base->Instance==GENERAL_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    GENERAL_TIM_RCC_CLK_DISABLE();
  }
} 

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
