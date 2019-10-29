#ifndef __ADVANCED_TIM_H__
#define __ADVANCED_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/

#define ADVANCED_TIMx                        TIM1
#define ADVANCED_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCED_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define ADVANCED_TIM_GPIO_RCC_CLK_ENABLE()   {__HAL_RCC_GPIOA_CLK_ENABLE();__HAL_RCC_GPIOB_CLK_ENABLE();}
#define ADVANCED_TIM_CH1_PORT                GPIOA
#define ADVANCED_TIM_CH1_PIN                 GPIO_PIN_8
#define ADVANCED_TIM_CH2_PORT                GPIOA
#define ADVANCED_TIM_CH2_PIN                 GPIO_PIN_9
#define ADVANCED_TIM_CH3_PORT                GPIOA
#define ADVANCED_TIM_CH3_PIN                 GPIO_PIN_10
#define ADVANCED_TIM_CH4_PORT                GPIOA
#define ADVANCED_TIM_CH4_PIN                 GPIO_PIN_11

#define ADVANCED_TIM_CH1N_PORT               GPIOB
#define ADVANCED_TIM_CH1N_PIN                GPIO_PIN_13
#define ADVANCED_TIM_CH2N_PORT               GPIOB
#define ADVANCED_TIM_CH2N_PIN                GPIO_PIN_14
#define ADVANCED_TIM_CH3N_PORT               GPIOB
#define ADVANCED_TIM_CH3N_PIN                GPIO_PIN_15

/* 72M��1000= */
// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��ADVANCED_TIMx_PRESCALER+1��
#define ADVANCED_TIM_PRESCALER            71  // ʵ��ʱ��Ƶ��Ϊ��72MHz/71+1=1M

// ���嶨ʱ�����ڣ�����ʱ����ʼ������ADVANCED_TIMx_PERIODֵ�����ظ������Ĵ���Ϊ0ʱ���¶�ʱ�������ɶ�Ӧ�¼����ж�
#define ADVANCED_TIM_PERIOD               200  // ��ʱ�������ж�Ƶ��Ϊ��1MHz/1000=1KHz����1ms��ʱ����
// ����߼���ʱ���ظ������Ĵ���ֵ��
#define ADVANCED_TIM_REPETITIONCOUNTER    0

// ���ն�ʱ��Ƶ�ʼ���Ϊ�� 72MHz/��ADVANCED_TIMx_PRESCALER+1/��ADVANCED_TIM_REPETITIONCOUNTER+1��/ADVANCED_TIMx_PERIOD
// ������Ҫ����20ms���ڶ�ʱ����������Ϊ�� 72MHz/��359+1��/��9+1��/400=50Hz����20ms����
// �������� ADVANCED_TIMx_PRESCALER=359��ADVANCED_TIM_REPETITIONCOUNTER=9��ADVANCED_TIMx_PERIOD=400��

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;
/* �������� ------------------------------------------------------------------*/

void ADVANCED_TIMx_Init(void);

#endif	/* __ADVANCED_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
