/**
  ******************************************************************************
  * 文件名程: bsp_adc.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 板载ADC电压采集底层驱动程序
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
#include "adc/bsp_adc.h"
#include "math.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
ADC_HandleTypeDef hadcx3;
ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;
DMA_HandleTypeDef hdma_adcx;

// 用于保存转换计算后的电压值	 
float ADC_ConvertedValueLocal[2];
// AD转换结果值
uint32_t ADC_ConvertedValue,ADC_NtcValue;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: AD转换初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MX_ADCx_Init(void)
{
  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;	
  // ADC功能配置
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
  
  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx1, &sConfig);

  // ADC功能配置
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
  
  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx2, &sConfig);
}
/**
  * 函数功能: AD转换初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void NTC_ADCx_Init(void){
	ADC_ChannelConfTypeDef sConfig;

  // ADC功能配置
  hadcx3.Instance = ADCx3;
  hadcx3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadcx3.Init.ContinuousConvMode = ENABLE;
  hadcx3.Init.DiscontinuousConvMode = DISABLE;
  hadcx3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadcx3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadcx3.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadcx3);

  // 配置采样通道
  sConfig.Channel = ADC_CHANNEL3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  HAL_ADC_ConfigChannel(&hadcx3, &sConfig);
}	

/**
  * 函数功能: ADC外设初始化配置
  * 输入参数: hadc：AD外设句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	if(hadc->Instance==ADCx1)
  {
    /* 外设时钟使能 */
    ADCx1_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
    
    /* DMA时钟使能 */
    DMAx1_RCC_CLK_ENABLE();
      
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);
		
    /* DMA外设初始化配置 */  
    hdma_adcx.Instance = ADC_DMAx_CHANNELn;
    hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcx.Init.Mode = DMA_CIRCULAR;
    hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adcx);
    /* 连接DMA */
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
		
    /* 外设中断优先级配置和使能中断 */
    HAL_NVIC_SetPriority(ADC_DMAx_CHANNELn_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_DMAx_CHANNELn_IRQn);
  }
  else if(hadc->Instance==ADCx2)
  {
    /* 外设时钟使能 */
    ADCx2_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
      
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);
  }else  if(hadc->Instance==ADCx3)
  {
    /* 外设时钟使能 */
    ADCx3_RCC_CLK_ENABLE();
    
    /* AD转换通道引脚时钟使能 */
    ADC_GPIO_ClK_ENABLE();
    
    /* AD转换通道引脚初始化 */
    GPIO_InitStruct.Pin = ADC_GPIO_PIN3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_GPIO, &GPIO_InitStruct);

    /* 外设中断优先级配置和使能中断 */
    HAL_NVIC_SetPriority(ADCx3_IRQ, 1, 0);
    HAL_NVIC_EnableIRQ(ADCx3_IRQ);
  }
}

/**
  * 函数功能: ADC外设反初始化配置
  * 输入参数: hadc：AD外设句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADCx1)
  {
    /* 禁用ADC外设时钟 */
    ADCx1_RCC_CLK_DISABLE();
  
    /* AD转换通道引脚反初始化 */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN);
    /* DMA外设反初始化*/
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }
  else if(hadc->Instance==ADCx2)
  {
    /* 禁用ADC外设时钟 */
    ADCx2_RCC_CLK_DISABLE();
  
    /* AD转换通道引脚反初始化 */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN);
  }else  if(hadc->Instance==ADCx3)
  {
    /* 禁用ADC外设时钟 */
    ADCx3_RCC_CLK_DISABLE();
  
    /* AD转换通道引脚反初始化 */
    HAL_GPIO_DeInit(ADC_GPIO, ADC_GPIO_PIN3);

    /* 禁用外设中断 */
    HAL_NVIC_DisableIRQ(ADCx3_IRQ);
  }
} 

/**
  * 函数功能: AD转换开始
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：激发光源和参考光源采集
  */
void NTCStart_Collecting(void){	
	HAL_ADCEx_Calibration_Start(&hadcx3);
  /* 启动AD转换并使能AD中断 */
  HAL_ADC_Start_IT(&hadcx3);	
}
/**
  * 函数功能: AD转换暂停
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：激发光源和参考光源采集
  */
void NTCStop_Collecting(void){
  /* 启动AD转换并使能AD中断 */
  HAL_ADC_Stop_IT(&hadcx3);	
}
/**
  * 函数功能: AD转换开始
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：激发光源和参考光源采集
  */
void Start_Collecting(void){
	HAL_ADCEx_Calibration_Start(&hadcx1);
	HAL_ADCEx_Calibration_Start(&hadcx2);
	/* 启动AD转换并使能DMA传输和中断 */
//	HAL_ADC_Start(&hadcx1);
	HAL_ADC_Start(&hadcx2);
	HAL_ADCEx_MultiModeStart_DMA(&hadcx1,&ADC_ConvertedValue,sizeof(ADC_ConvertedValue));	
}
/**
  * 函数功能: AD转换停止
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：激发光源和参考光源采集
  */
void Stop_Collecting(void){
	/* 停止AD转换并使能DMA传输和中断 */
//	HAL_ADC_Stop(&hadcx1);
	HAL_ADC_Stop(&hadcx2);
	HAL_ADCEx_MultiModeStop_DMA(&hadcx1);
	HAL_ADCEx_MultiModeStop_DMA(&hadcx2);		
}	 

/**
  * 函数功能: 温度住AD转换
  * 输入参数: ADC_Value  ADC中断保存的采集值 ADC_NtcValue
  * 返 回 值: TR_Tem   计算之后的温度值
  * 说    明：10 K NTC热敏电阻
  */
int16_t GetTem(uint16_t ADC_Value){
	int TR_Tem;
	double output;
	
	uint32_t temp=0,lstTR_Tem;
	/********开始采集**********/
	NTCStart_Collecting();
	printf("\r\n ADC_NtcValue = %d \r\n", ADC_NtcValue); 
	temp=ADC_Value*825/1024;
	NTCStop_Collecting();	
	/********停止采集**********/
	temp=temp*10000/(3300-temp);	
	output=(3435.0/(log(temp/10000.0)+11.521))-273.15;	
	TR_Tem=output*10;
	if(TR_Tem>lstTR_Tem+3||TR_Tem<lstTR_Tem-3){
		lstTR_Tem=TR_Tem;}
	else{
		TR_Tem=lstTR_Tem;}

	if(TR_Tem>=0){
		printf("\r\n Temperature above zero: %d ℃",TR_Tem);
	}else{
		printf("\r\n Temperature above zero: %d ℃",TR_Tem);	
	}
	return TR_Tem;
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
