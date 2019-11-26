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
#include "math.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
ADC_HandleTypeDef hadcx3;
ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;
DMA_HandleTypeDef hdma_adcx;

// ���ڱ���ת�������ĵ�ѹֵ	 
float ADC_ConvertedValueLocal[2];
// ADת�����ֵ
uint32_t ADC_ConvertedValue,ADC_NtcValue;
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
  hadcx1.Init.ContinuousConvMode = ENABLE;
  hadcx1.Init.DiscontinuousConvMode = DISABLE;
  hadcx1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx1);  
  
    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_DUALMODE_INTERLFAST;
  HAL_ADCEx_MultiModeConfigChannel(&hadcx1, &multimode);
  
  // ���ò���ͨ��
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx1, &sConfig);

  // ADC��������
  hadcx2.Instance = ADCx2;
  hadcx2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx2.Init.ContinuousConvMode = ENABLE;
  hadcx2.Init.DiscontinuousConvMode = DISABLE;
  hadcx2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx2, &sConfig);
}
/**
  * ��������: ADת����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void NTC_ADCx_Init(void){
	ADC_ChannelConfTypeDef sConfig;

  // ADC��������
  hadcx3.Instance = ADCx3;
  hadcx3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx3.Init.ContinuousConvMode = ENABLE;
  hadcx3.Init.DiscontinuousConvMode = DISABLE;
  hadcx3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx3.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx3);

  // ���ò���ͨ��
  sConfig.Channel = ADC_CHANNEL3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx3, &sConfig);
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
  }else  if(hadc->Instance==ADCx3)
  {
    /* ����ʱ��ʹ�� */
    ADCx3_RCC_CLK_ENABLE();
    
    /* ADת��ͨ������ʱ��ʹ�� */
    ADC_GPIO_ClK_ENABLE();
    
    /* ADת��ͨ�����ų�ʼ�� */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);

    /* �����ж����ȼ����ú�ʹ���ж� */
    HAL_NVIC_SetPriority(ADCx3_IRQ, 1, 0);
    HAL_NVIC_EnableIRQ(ADCx3_IRQ);
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
  }else  if(hadc->Instance==ADCx3)
  {
    /* ����ADC����ʱ�� */
    ADCx3_RCC_CLK_DISABLE();
  
    /* ADת��ͨ�����ŷ���ʼ�� */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN3);

    /* ���������ж� */
    HAL_NVIC_DisableIRQ(ADCx3_IRQ);
  }
} 

/**
  * ��������: ADת����ʼ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����������Դ�Ͳο���Դ�ɼ�
  */
void NTCStart_Collecting(void){	
	HAL_ADCEx_Calibration_Start(&hadcx3);
  /* ����ADת����ʹ��AD�ж� */
  HAL_ADC_Start_IT(&hadcx3);	
}
/**
  * ��������: ADת����ͣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����������Դ�Ͳο���Դ�ɼ�
  */
void NTCStop_Collecting(void){
  /* ����ADת����ʹ��AD�ж� */
  HAL_ADC_Stop_IT(&hadcx3);	
}
/**
  * ��������: ADת����ʼ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����������Դ�Ͳο���Դ�ɼ�
  */
void Start_Collecting(void){
	HAL_ADCEx_Calibration_Start(&hadcx1);
	HAL_ADCEx_Calibration_Start(&hadcx2);
	/* ����ADת����ʹ��DMA������ж� */
//	HAL_ADC_Start(&hadcx1);
	HAL_ADC_Start(&hadcx2);
	HAL_ADCEx_MultiModeStart_DMA(&hadcx1,&ADC_ConvertedValue,sizeof(ADC_ConvertedValue));	
}
/**
  * ��������: ADת��ֹͣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����������Դ�Ͳο���Դ�ɼ�
  */
void Stop_Collecting(void){
	/* ֹͣADת����ʹ��DMA������ж� */
//	HAL_ADC_Stop(&hadcx1);
	HAL_ADC_Stop(&hadcx2);
	HAL_ADCEx_MultiModeStop_DMA(&hadcx1);
	HAL_ADCEx_MultiModeStop_DMA(&hadcx2);		
}	 

/**
  * ��������: �¶�סADת��
  * �������: ADC_Value  ADC�жϱ���Ĳɼ�ֵ ADC_NtcValue
  * �� �� ֵ: TR_Tem   ����֮����¶�ֵ
  * ˵    ����10 K NTC��������
  */
int16_t GetTem(uint16_t ADC_Value){
	int TR_Tem;
	double output;
	
	uint32_t temp=0,lstTR_Tem;
	/********��ʼ�ɼ�**********/
	NTCStart_Collecting();
	printf("\r\n ADC_NtcValue = %d \r\n", ADC_NtcValue); 
	temp=ADC_Value*825/1024;
	NTCStop_Collecting();	
	/********ֹͣ�ɼ�**********/
	temp=temp*10000/(3300-temp);	
	output=(3435.0/(log(temp/10000.0)+11.521))-273.15;	
	TR_Tem=output*10;
	if(TR_Tem>lstTR_Tem+3||TR_Tem<lstTR_Tem-3){
		lstTR_Tem=TR_Tem;}
	else{
		TR_Tem=lstTR_Tem;}

	if(TR_Tem>=0){
		printf("\r\n Temperature above zero: %d ��",TR_Tem);
	}else{
		printf("\r\n Temperature above zero: %d ��",TR_Tem);	
	}
	return TR_Tem;
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
