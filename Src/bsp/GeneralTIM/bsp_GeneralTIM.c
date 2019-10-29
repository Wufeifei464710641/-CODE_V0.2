/**
  ******************************************************************************
  * �ļ�����: bsp_GeneralTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ͨ�ö�ʱ���ײ���������
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
#include "GeneralTIM/bsp_GeneralTIM.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef G_htimx;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʱ��Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����GENERAL_TIMx_Init��������
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==GENERAL_TIMx)
  {  
    /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
    GENERAL_TIM_GPIO_RCC_CLK_ENABLE();

    /* ��ʱ��ͨ��3��������IO��ʼ�� */
    GPIO_InitStruct.Pin = GENERAL_TIM_CH3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStruct);

    /* ��ʱ��ͨ��4��������IO��ʼ�� */
    GPIO_InitStruct.Pin = GENERAL_TIM_CH4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStruct);
  }
}

/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void GENERAL_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  G_htimx.Instance = GENERAL_TIMx;
  G_htimx.Init.Prescaler = GENERAL_TIM_PRESCALER;
  G_htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  G_htimx.Init.Period = GENERAL_TIM_PERIOD;
  G_htimx.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&G_htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&G_htimx, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&G_htimx, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  sConfigOC.Pulse = GENERAL_TIM_CH3_PULSE;
  HAL_TIM_PWM_ConfigChannel(&G_htimx, &sConfigOC, TIM_CHANNEL_3);

  sConfigOC.Pulse = GENERAL_TIM_CH4_PULSE;
  HAL_TIM_PWM_ConfigChannel(&G_htimx, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&G_htimx);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==GENERAL_TIMx)
		{
			/* ������ʱ������ʱ��ʹ�� */
			GENERAL_TIM_RCC_CLK_ENABLE();
		}
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==GENERAL_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    GENERAL_TIM_RCC_CLK_DISABLE();
  }
} 
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
