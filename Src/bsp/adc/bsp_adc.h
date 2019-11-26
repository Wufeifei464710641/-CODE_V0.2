#ifndef __ADC_H__
#define	__ADC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC����ͨ�������ţ�����**************************/
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

#define ADC_GPIO_PIN3                   	GPIO_PIN_1       //��������  
#define ADC_CHANNEL3                    	ADC_CHANNEL_1   

/* ��չ���� ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx3;
extern ADC_HandleTypeDef hadcx1;
extern ADC_HandleTypeDef hadcx2;
extern DMA_HandleTypeDef hdma_adcx;

// ���ڱ���ת�������ĵ�ѹֵ	 
extern float ADC_ConvertedValueLocal[2];
// ADת�����ֵ
extern uint32_t ADC_ConvertedValue,ADC_NtcValue;
/* �������� ------------------------------------------------------------------*/
void MX_ADCx_Init(void);
void NTC_ADCx_Init(void);

void NTCStart_Collecting(void);
void NTCStop_Collecting(void);

void Start_Collecting(void);
void Stop_Collecting(void);

int16_t GetTem(uint16_t ADC_Value);
#endif /* __ADC_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
