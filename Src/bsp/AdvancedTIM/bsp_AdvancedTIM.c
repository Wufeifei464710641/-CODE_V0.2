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
TIM_HandleTypeDef A_htimx;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

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
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base){
  if(htim_base->Instance==ADVANCED_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    ADVANCED_TIM_RCC_CLK_ENABLE();

    /* �����ж����� */
    HAL_NVIC_SetPriority(ADVANCED_TIM_IRQ, 1, 0);
    HAL_NVIC_EnableIRQ(ADVANCED_TIM_IRQ);
  }
	else if(htim_base->Instance==GENERAL_TIMx)
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
  if(htim_base->Instance==ADVANCED_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    ADVANCED_TIM_RCC_CLK_DISABLE();

    /* �ر������ж� */
    HAL_NVIC_DisableIRQ(ADVANCED_TIM_IRQ);
  }
	else if(htim_base->Instance==GENERAL_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    GENERAL_TIM_RCC_CLK_DISABLE();
  }
} 

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
