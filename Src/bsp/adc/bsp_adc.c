/**
  ******************************************************************************
  * �ļ�����: bsp_adc.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ����ADC��ѹ�ɼ��ײ���������
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
#include "adc/bsp_adc.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;
DMA_HandleTypeDef hdma_adcx;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ADת����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_ADCx_Init(void)
{
  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

  // ADC��������
  hadcx1.Instance = ADCx1;
  hadcx1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx1.Init.ContinuousConvMode = DISABLE;//ע�ⲻҪʹ������ת�����������ͻ�һֱת����ֱ��count��0��
  hadcx1.Init.DiscontinuousConvMode = DISABLE;
  hadcx1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1; //�ⲿ�������� tim1_cc1��Ϊ����Դ
  hadcx1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx1);  
  
    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_DUALMODE_INTERLSLOW;
  HAL_ADCEx_MultiModeConfigChannel(&hadcx1, &multimode);
  
  // ���ò���ͨ��
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx1, &sConfig);
  
  // ADC��������
  hadcx2.Instance = ADCx2;
  hadcx2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx2.Init.ContinuousConvMode = DISABLE;//ע�ⲻҪʹ������ת�����������ͻ�һֱת����ֱ��count��0��
  hadcx2.Init.DiscontinuousConvMode = DISABLE;
  hadcx2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;  //�ⲿ�������� tim1_cc1��Ϊ����Դ
	
  hadcx2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx2.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx2);  
  
    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_DUALMODE_INTERLFAST;
  HAL_ADCEx_MultiModeConfigChannel(&hadcx2, &multimode);
  
  // ���ò���ͨ��
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx2, &sConfig);
	
//	ADC_SampleRate
}

/**
  * ��������: ADC�����ʼ������
  * �������: hadc��AD����������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADCx1)
  {
    /* ����ʱ��ʹ�� */
    ADCx1_RCC_CLK_ENABLE();
    
    /* ADת��ͨ������ʱ��ʹ�� */
    ADC_GPIO_ClK_ENABLE();
    
    /* DMAʱ��ʹ�� */
    DMAx1_RCC_CLK_ENABLE();
      
    /* ADת��ͨ�����ų�ʼ�� */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);

    /* DMA�����ʼ������ */  
    hdma_adcx.Instance = ADC_DMAx_CHANNELn;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adcx);
    /* ����DMA */
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
    
    /* �����ж����ȼ����ú�ʹ���ж� */
    HAL_NVIC_SetPriority(ADC_DMAx_CHANNELn_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_DMAx_CHANNELn_IRQn);
  }
  else if(hadc->Instance==ADCx2)
  {
    /* ����ʱ��ʹ�� */
    ADCx2_RCC_CLK_ENABLE();
    
    /* ADת��ͨ������ʱ��ʹ�� */
    ADC_GPIO_ClK_ENABLE();
      
    /* ADת��ͨ�����ų�ʼ�� */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ADC���跴��ʼ������
  * �������: hadc��AD����������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADCx1)
  {
    /* ����ADC����ʱ�� */
    ADCx1_RCC_CLK_DISABLE();
  
    /* ADת��ͨ�����ŷ���ʼ�� */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN);

    /* DMA���跴��ʼ��*/
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }
  else if(hadc->Instance==ADCx2)
  {
    /* ����ADC����ʱ�� */
    ADCx2_RCC_CLK_DISABLE();
  
    /* ADת��ͨ�����ŷ���ʼ�� */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN);
  }
} 

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
