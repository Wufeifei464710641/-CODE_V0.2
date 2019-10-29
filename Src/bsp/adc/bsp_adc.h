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

#define ADCx2_RCC_CLK_ENABLE()            __HAL_RCC_ADC2_CLK_ENABLE()
#define ADCx2_RCC_CLK_DISABLE()           __HAL_RCC_ADC2_CLK_DISABLE()
#define ADCx2                             ADC2

#define ADC_GPIO_ClK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC_GPIO                          GPIOA
#define ADC_GPIO_PIN                      GPIO_PIN_0        
#define ADC_CHANNEL                       ADC_CHANNEL_0    
              

/* 扩展变量 ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx1;
extern ADC_HandleTypeDef hadcx2;
extern DMA_HandleTypeDef hdma_adcx;
/* 函数声明 ------------------------------------------------------------------*/
void MX_ADCx_Init(void);

#endif /* __ADC_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
