#ifndef __ADC_H__
#define	__ADC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC输入通道（引脚）配置**************************/
#define ADCx1_RCC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx1_RCC_CLK_DISABLE()           __HAL_RCC_ADC1_CLK_DISABLE()

#define DMAx1_RCC_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx1                             ADC1
#define ADC_DMAx_CHANNELn                 DMA1_Channel1
#define ADC_DMAx_CHANNELn_IRQn            DMA1_Channel1_IRQn
#define ADC_DMAx_CHANNELn_IRQHANDLER      DMA1_Channel1_IRQHandler
/*------------------------------------------------------------------*/
#define ADCx2_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
#define ADCx2_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()

#define ADCx2                             ADC2
#define ADC_GPIO_ClK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO                          GPIOA
#define ADC_GPIO_PIN                      GPIO_PIN_0        
#define ADC_CHANNEL                       ADC_CHANNEL_0                 
/*------------------------------------------------------------------*/
#define ADCx3_RCC_CLK_ENABLE()            __HAL_RCC_ADC3_CLK_ENABLE()
#define ADCx3_RCC_CLK_DISABLE()           __HAL_RCC_ADC3_CLK_DISABLE()
#define ADCx3                             ADC3
#define ADCx3_IRQ                         ADC3_IRQn
#define ADCx3_IRQHandler                  ADC3_IRQHandler

#define ADC_GPIO_PIN3                   	GPIO_PIN_1       //热敏电阻  
#define ADC_CHANNEL3                    	ADC_CHANNEL_1   

/* 扩展变量 ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx3;
extern ADC_HandleTypeDef hadcx1;
extern ADC_HandleTypeDef hadcx2;
extern DMA_HandleTypeDef hdma_adcx;

// 用于保存转换计算后的电压值	 
extern float ADC_ConvertedValueLocal[2];
// AD转换结果值
extern uint32_t ADC_ConvertedValue,ADC_NtcValue;
/* 函数声明 ------------------------------------------------------------------*/
void MX_ADCx_Init(void);
void NTC_ADCx_Init(void);

void NTCStart_Collecting(void);
void NTCStop_Collecting(void);

void Start_Collecting(void);
void Stop_Collecting(void);

int16_t GetTem(uint16_t ADC_Value);
#endif /* __ADC_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
