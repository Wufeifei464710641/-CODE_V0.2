/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ʹ��˫ADC���ٽ���ģʽ�ɼ���ѹ
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
#include "stm32f1xx_hal.h"
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"
#include "adc/bsp_adc.h"
#include "GeneralTIM/bsp_GeneralTIM.h"
#include "AdvancedTIM/bsp_AdvancedTIM.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
// ���ڱ���ת�������ĵ�ѹֵ	 
float ADC_ConvertedValueLocal[2];
// ADת�����ֵ
uint32_t ADC_ConvertedValue;
uint32_t DMA_Transfer_Complete_Count=0;
__IO uint16_t timer_count=0;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9��Ƶ���õ�72MHz��ʱ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ�72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1ʱ�ӣ�36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2ʱ�ӣ�72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  /* ADCʱ������ */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6; // 6��Ƶ��12MHz
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{  
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
//  SystemClock_Config();
	LED_GPIO_Init();
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();  
  printf("\n----����һ��ADC��ͨ����ѹ�ɼ�ʵ��(DMA����)-----\n"); 
  /* ͨ�ö�ʱ����ʼ��������PWM������� */
  GENERAL_TIMx_Init();
	
  /* ������ʱ����ʼ����10ms�ж�һ�� */
//  ADVANCED_TIMx_Init();
  /* ���ж�ģʽ��������ʱ�� */
//  HAL_TIM_Base_Start_IT(&htimx);
	
  /* ADC ��ʼ�� */
  MX_ADCx_Init();
  HAL_ADCEx_Calibration_Start(&hadcx1);
  HAL_ADCEx_Calibration_Start(&hadcx2);
  /* ����ѭ�� */
  while (1)
  { 
//		LEDx_StateSet(LED1,LED_TOGGLE);
//		HAL_Delay(300);	//0.2s
		//STEP1�����������
		HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_4);
		/* ����ADת����ʹ��DMA������ж� */
		HAL_ADC_Start(&hadcx1);
		HAL_ADCEx_MultiModeStart_DMA(&hadcx1,&ADC_ConvertedValue,sizeof(ADC_ConvertedValue));
		//STEP2����ʱ200ms		
		HAL_Delay(200);	//0.2s
		//STEP3���ر����������Դ���ر�ADC1�ɼ�
		HAL_TIM_PWM_Stop(&htimx,TIM_CHANNEL_4); 
		HAL_ADC_Stop(&hadcx1);	
		//STEP4������ο���Դ
		HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_3);
		/* ����ADת����ʹ��DMA������ж� */
		HAL_ADC_Start(&hadcx2);
		HAL_Delay(200);	//0.2s		
		HAL_ADCEx_MultiModeStart_DMA(&hadcx2,&ADC_ConvertedValue,sizeof(ADC_ConvertedValue));
		//STEP2���ر����������Դ���ر�ADC1�ɼ�
		HAL_TIM_PWM_Stop(&htimx,TIM_CHANNEL_3); 
		HAL_ADC_Stop(&hadcx2);			
//		HAL_Delay(1000);	//0.2s		
    /* 3.3ΪADת���Ĳο���ѹֵ��stm32��ADת��Ϊ12bit��2^12=4096��
       ��������Ϊ3.3Vʱ��ADת�����Ϊ4096 */
    ADC_ConvertedValueLocal[0] =(float)(ADC_ConvertedValue&0xFFF)*3.3/4096; 
	  ADC_ConvertedValueLocal[1] =(float)((ADC_ConvertedValue>>16)&0xFFF)*3.3/4096;         
		printf("\nADC1ת��ԭʼֵ = %04d --> ��ѹֵ = %f V ", ADC_ConvertedValue&0xFFFF,ADC_ConvertedValueLocal[0]);    
    printf("\nADC2ת��ԭʼֵ = %04d --> ��ѹֵ = %f V ", (ADC_ConvertedValue>>16)&0xFFFF,ADC_ConvertedValueLocal[1]);      
    printf("\n�Ѿ����ADת��������%d",DMA_Transfer_Complete_Count);
    printf("\r\n");    
    DMA_Transfer_Complete_Count=0;
  }
}

/**
  * ��������: ADCת����ɻص�����
  * �������: hadc��ADC�����豸���
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  DMA_Transfer_Complete_Count++; 
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
