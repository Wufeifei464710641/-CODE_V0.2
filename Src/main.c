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
#include "core_cm3.h"
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"
#include "adc/bsp_adc.h"
#include "GeneralTIM/bsp_GeneralTIM.h"
#include "AdvancedTIM/bsp_AdvancedTIM.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "arm_math.h"
#include "math.h"
void SystemClock_Config(void);
void GetPowerMag(void);
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
#define NPT 256

uint32_t R_ADCValue[NPT]; //ADC�ɼ���������
long lBufInArray[NPT];       //FFT�������������
long lBufOutArray[NPT/2];    //����FFT��������������ǶԳƵģ����ͨ�������������ȡһ�������
long lBufMagArray[NPT/2];    //г����ֵ

uint32_t DMA_Transfer_Complete_Count=0;
uint16_t timer_count=0;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: г����ֵ����
  * �������: lBufOutArray[]
  * �� �� ֵ: lBufMagArray[]
  * ˵    ��: ��
  */
void GetPowerMag(void)
{
	signed short lX,lY;
	float X,Y,Mag;
	unsigned short i;
	for(i=0; i<NPT/2; i++)
	{
		lX  = (lBufOutArray[i] << 16) >> 16;
		lY  = (lBufOutArray[i] >> 16);
		
		//����32768�ٳ�65536��Ϊ�˷��ϸ������������
		X = NPT * ((float)lX) / 32768;
		Y = NPT * ((float)lY) / 32768;
		Mag = sqrt(X * X + Y * Y) / NPT;
		if(i == 0)
				lBufMagArray[i] = (unsigned long)(Mag * 32768);
		else
				lBufMagArray[i] = (unsigned long)(Mag * 65536);
		
		printf("\r\n lBufMagArray[%d] %ld \r\n",i,lBufMagArray[i]);
	}
}
/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{  
//  arm_cfft_radix4_instance_f32 scfft;	
	
	uint16_t i=0,j=0;
	int Temp=0;//,adc1,adc2
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
	LED_GPIO_Init();
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();  
  /* ADC ��ʼ�� */
  MX_ADCx_Init();				
	/*NTC���������ʼ��**�����жϲɼ�*/	
	NTC_ADCx_Init();
  /* ͨ�ö�ʱ����ʼ��������PWM������� */
  GENERAL_TIMx_Init();
	HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_4);	
  /* ������ʱ����ʼ����1 ms�ж�һ�� */
  ADVANCED_TIMx_Init();
	/* ���ж�ģʽ��������ʱ�� */
//	HAL_TIM_Base_Start_IT(&A_htimx);	
	
  printf("\n----This is an voltage acquisition experiment (DMA transfer)-----\n"); 	
  /* ����ѭ�� */
  while (1){
//		__HAL_TIM_SET_COUNTER(&A_htimx,0);//����TIM1��ʱ���ļ�����ֵ
//		timer_count=0;
//		HAL_Delay(1000);
//		time = __HAL_TIM_GET_COUNTER(&A_htimx);//��������ʱ�� 

//		LEDx_StateSet(LED1,LED_TOGGLE);
//		printf("\r\n ����ִ��ʱ�䣺 %0.3fms   %d \r\n",time,timer_count);
		
//		if(j==0){     
//			//STEP1�����������
//			HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_4);	
//			HAL_Delay(2000);

//			Start_Collecting();		

// 			for(i=0;i<NPT;i+=2){
//				R_ADCValue[i] = ADC_ConvertedValue&0xFFF;
//				R_ADCValue[i+1] = (ADC_ConvertedValue>>16)&0xFFF;
//				printf("ADC1ת��ԭʼֵR_ADCValue[][%d] = %04d \r\n",i, ADC_ConvertedValue&0xFFFF);    
//				printf("ADC2ת��ԭʼֵR_ADCValue[][%d] = %04d \r\n",i+1,(ADC_ConvertedValue>>16)&0xFFF);  					
//			}
//			HAL_TIM_PWM_Stop(&htimx,TIM_CHANNEL_4); 
//			Stop_Collecting();			
//			//*******��¼ʱ��*****************/
////			__HAL_TIM_SET_COUNTER(&A_htimx,0);//����TIM1��ʱ���ļ�����ֵ
////			timer_count=0;				
//			//�������
//			for(i=0;i<NPT;i++){
//				//������Ϊ��Ƭ����ADCֻ�ܲ����ĵ�ѹ ������Ҫǰ����ֱ��ƫִ
//				//����ֱ��ƫִ����Ҫ������ϼ�ȥ2048��һ�룬�ﵽ�������ڲ�����Ŀ�ģ���Ҫ���ݾ���������������ã�
//				lBufInArray[i] = (signed short)R_ADCValue[i];
////				lBufInArray[i] = ((signed short)(R_ADCValue[i]-2048)) << 16;
//				/* NPT ��ʵ���п���FFT */ 
//				cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);

//				printf("\r\nlBufOutArray[%d] %ld ; lBufInArray[%d] = %ld \r\n",i,lBufOutArray[i],i,lBufInArray[i]);
//			}					
//			GetPowerMag(); //г����ֵ����				
//			HAL_Delay(3000);
//			j++;
//		}
//		else if(j==1){
//			//STEP3������ο���
//			HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_3);
//			HAL_Delay(3000);
//			//�ɼ���ʼ
//			Start_Collecting();	
//			for(i=0;i<=NPT;i+=2){
//				R_ADCValue[i] = ADC_ConvertedValue&0xFFF;
//				R_ADCValue[i+1] = (ADC_ConvertedValue>>16)&0xFFF;
//				printf("\nADC1ת��ԭʼֵR_ADCValue[][%d] = %04d ",i, ADC_ConvertedValue&0xFFFF);    
//				printf("\r\nADC2ת��ԭʼֵR_ADCValue[][%d] = %04d ",i+1,(ADC_ConvertedValue>>16)&0xFFF);  		
////				adc2+=(R_ADCValue[j][i]+R_ADCValue[j][i+1]);			
//			}	
////			printf("\r\n�ο���Դת��ԭʼֵ�ܺ�adc2 = %d \r\n",adc2);  
//			/**********************************************************/
//			Stop_Collecting();	
//			HAL_TIM_PWM_Stop(&htimx,TIM_CHANNEL_3);

//			
//			j++;
//		}
			//else 
//				if(j == 1){
//				HAL_Delay(1000);
//				NTCStart_Collecting();				
//				Temp = GetTem(ADC_NtcValue);	
//				printf("\r\nADת���¶�ֵTemp = %d \r\n", Temp);
//				NTCStop_Collecting();				
//				j++;
//			}
		 
//		for(i=0;i<FFT_LENGTH;i++)//�����ź�����
//		{
//			 fft_inputbuf[2*i]=100+
//												 10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
//								 30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
//												 50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//���������ź�ʵ��
//			 fft_inputbuf[2*i+1]=0;//�鲿ȫ��Ϊ0
//		}
//		__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//����TIM3��ʱ���ļ�����ֵ
//		timeout=0;
//		arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT���㣨��4��
//					time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//��������ʱ��
//		sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
//		
//		LCD_ShowString(30+12*8,160,100,16,16,buf);	//��ʾ����ʱ��		
//		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//��������������ģ�÷�ֵ 
//		printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
//		printf("FFT Result:\r\n");
//		for(i=0;i<FFT_LENGTH;i++)
//		{
//			printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
//		}
  }
}

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
  * ��������: ADCת����ɻص�����
  * �������: hadc��ADC�����豸���
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	DMA_Transfer_Complete_Count++; 
	  ADC_NtcValue=HAL_ADC_GetValue(&hadcx3);
}

float  time =0;
/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim==(&A_htimx)){       //��ʱ��3
		timer_count++;		
		printf("\r\n timer_count = %d \r\n", timer_count); 
	}
} 
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
