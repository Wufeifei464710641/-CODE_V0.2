/**
  ******************************************************************************
  * 文件名程: bsp_GeneralTIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 通用定时器底层驱动程序
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
#include "GeneralTIM/bsp_GeneralTIM.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 定时器硬件初始化配置
  * 输入参数: htim：定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被GENERAL_TIMx_Init函数调用
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==GENERAL_TIMx)
  {  
    /* 定时器通道功能引脚端口时钟使能 */
    GENERAL_TIM_GPIO_RCC_CLK_ENABLE();
    
    /* 定时器通道3功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GENERAL_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStruct);

    /* 定时器通道4功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GENERAL_TIM_CH4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void GENERAL_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  htimx.Instance = GENERAL_TIMx;
  htimx.Init.Prescaler = GENERAL_TIM_PRESCALER;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = GENERAL_TIM_PERIOD;
  htimx.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimx, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = GENERAL_TIM_CH3_PULSE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_3);
	
  sConfigOC.Pulse = GENERAL_TIM_CH4_PULSE;
  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&htimx);
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
