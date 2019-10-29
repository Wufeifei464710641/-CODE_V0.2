/**
  ******************************************************************************
  * �ļ�����: bsp_AdvancedTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: �߼���ʱ��TIM1 & TIM8�ײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "AdvancedTIM/bsp_AdvancedTIM.h" 
#include "GeneralTIM/bsp_GeneralTIM.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʱ��Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����ADVANCED_TIMx_Init��������
  */
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//  if(htim->Instance==ADVANCED_TIMx)
//  {  
//    /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
//    ADVANCED_TIM_GPIO_RCC_CLK_ENABLE();
//    
//    /* ��ʱ��ͨ��1��������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH1_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(ADVANCED_TIM_CH1_PORT, &GPIO_InitStruct);
//    /* ��ʱ��ͨ��1����ͨ����������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH1N_PIN;
//    HAL_GPIO_Init(ADVANCED_TIM_CH1N_PORT, &GPIO_InitStruct);

//    /* ��ʱ��ͨ��2��������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH2_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(ADVANCED_TIM_CH2_PORT, &GPIO_InitStruct);
//    /* ��ʱ��ͨ��2����ͨ����������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH2N_PIN;
//    HAL_GPIO_Init(ADVANCED_TIM_CH2N_PORT, &GPIO_InitStruct);
//    
//    /* ��ʱ��ͨ��3��������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH3_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(ADVANCED_TIM_CH3_PORT, &GPIO_InitStruct);
//    /* ��ʱ��ͨ��3����ͨ����������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH3N_PIN;
//    HAL_GPIO_Init(ADVANCED_TIM_CH3N_PORT, &GPIO_InitStruct);
//    
//    /* ��ʱ��ͨ��4��������IO��ʼ�� */
//    GPIO_InitStruct.Pin = ADVANCED_TIM_CH4_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(ADVANCED_TIM_CH4_PORT, &GPIO_InitStruct);
//  }
//}

/**
  * ��������: ������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ADVANCED_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  htimx.Instance = ADVANCED_TIMx;
  htimx.Init.Prescaler = ADVANCED_TIM_PRESCALER;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = ADVANCED_TIM_PERIOD;
  htimx.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  htimx.Init.RepetitionCounter = ADVANCED_TIM_REPETITIONCOUNTER;
  HAL_TIM_Base_Init(&htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimx, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htimx);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig);
  
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 900;//
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_1);

//  sConfigOC.Pulse = 600;
//  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_2);

//  sConfigOC.Pulse = 0;
//  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_3);

//  sConfigOC.Pulse = 100;
//  HAL_TIM_PWM_ConfigChannel(&htimx, &sConfigOC, TIM_CHANNEL_4);

//  HAL_TIM_MspPostInit(&htimx);

}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
//{
//  if(htim_base->Instance==ADVANCED_TIMx)
//  {
//    /* ������ʱ������ʱ��ʹ�� */
//    ADVANCED_TIM_RCC_CLK_ENABLE();
//  }
//  else if(htim_base->Instance==GENERAL_TIMx)
//  {
//    /* ������ʱ������ʱ��ʹ�� */
//    GENERAL_TIM_RCC_CLK_ENABLE();
//  }
//}

///**
//  * ��������: ������ʱ��Ӳ������ʼ������
//  * �������: htim_base��������ʱ���������ָ��
//  * �� �� ֵ: ��
//  * ˵    ��: �ú�����HAL���ڲ�����
//  */
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
//{

//  if(htim_base->Instance==ADVANCED_TIMx)
//  {
//    /* ������ʱ������ʱ�ӽ��� */
//    ADVANCED_TIM_RCC_CLK_DISABLE();
//  }else   if(htim_base->Instance==GENERAL_TIMx)
//  {
//    /* ������ʱ������ʱ�ӽ��� */
//    GENERAL_TIM_RCC_CLK_DISABLE();
//  }
//} 

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
