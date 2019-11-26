/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 使用双ADC慢速交叉模式采集电压
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
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
#define NPT 256

uint32_t R_ADCValue[NPT]; //ADC采集保存数组
long lBufInArray[NPT];       //FFT运算的输入数组
long lBufOutArray[NPT/2];    //由于FFT计算出来的数据是对称的，因此通常而言输出数组取一半的数据
long lBufMagArray[NPT/2];    //谐波幅值

uint32_t DMA_Transfer_Complete_Count=0;
uint16_t timer_count=0;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 谐波幅值计算
  * 输入参数: lBufOutArray[]
  * 返 回 值: lBufMagArray[]
  * 说    明: 无
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
		
		//除以32768再乘65536是为了符合浮点数计算规律
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
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{  
//  arm_cfft_radix4_instance_f32 scfft;	
	
	uint16_t i=0,j=0;
	int Temp=0;//,adc1,adc2
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
	LED_GPIO_Init();
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();  
  /* ADC 初始化 */
  MX_ADCx_Init();				
	/*NTC热敏电阻初始化**采用中断采集*/	
	NTC_ADCx_Init();
  /* 通用定时器初始化并配置PWM输出功能 */
  GENERAL_TIMx_Init();
	HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_4);	
  /* 基本定时器初始化：1 ms中断一次 */
  ADVANCED_TIMx_Init();
	/* 在中断模式下启动定时器 */
//	HAL_TIM_Base_Start_IT(&A_htimx);	
	
  printf("\n----This is an voltage acquisition experiment (DMA transfer)-----\n"); 	
  /* 无限循环 */
  while (1){
//		__HAL_TIM_SET_COUNTER(&A_htimx,0);//重设TIM1定时器的计数器值
//		timer_count=0;
//		HAL_Delay(1000);
//		time = __HAL_TIM_GET_COUNTER(&A_htimx);//计算所用时间 

//		LEDx_StateSet(LED1,LED_TOGGLE);
//		printf("\r\n 程序执行时间： %0.3fms   %d \r\n",time,timer_count);
		
//		if(j==0){     
//			//STEP1：输出激发光
//			HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_4);	
//			HAL_Delay(2000);

//			Start_Collecting();		

// 			for(i=0;i<NPT;i+=2){
//				R_ADCValue[i] = ADC_ConvertedValue&0xFFF;
//				R_ADCValue[i+1] = (ADC_ConvertedValue>>16)&0xFFF;
//				printf("ADC1转换原始值R_ADCValue[][%d] = %04d \r\n",i, ADC_ConvertedValue&0xFFFF);    
//				printf("ADC2转换原始值R_ADCValue[][%d] = %04d \r\n",i+1,(ADC_ConvertedValue>>16)&0xFFF);  					
//			}
//			HAL_TIM_PWM_Stop(&htimx,TIM_CHANNEL_4); 
//			Stop_Collecting();			
//			//*******记录时间*****************/
////			__HAL_TIM_SET_COUNTER(&A_htimx,0);//重设TIM1定时器的计数器值
////			timer_count=0;				
//			//填充数组
//			for(i=0;i<NPT;i++){
//				//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执
//				//加入直流偏执后，需要在软件上减去2048即一半，达到负半周期测量的目的（需要根据具体情况来进行配置）
//				lBufInArray[i] = (signed short)R_ADCValue[i];
////				lBufInArray[i] = ((signed short)(R_ADCValue[i]-2048)) << 16;
//				/* NPT 点实序列快速FFT */ 
//				cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);

//				printf("\r\nlBufOutArray[%d] %ld ; lBufInArray[%d] = %ld \r\n",i,lBufOutArray[i],i,lBufInArray[i]);
//			}					
//			GetPowerMag(); //谐波幅值计算				
//			HAL_Delay(3000);
//			j++;
//		}
//		else if(j==1){
//			//STEP3：输出参考光
//			HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_3);
//			HAL_Delay(3000);
//			//采集开始
//			Start_Collecting();	
//			for(i=0;i<=NPT;i+=2){
//				R_ADCValue[i] = ADC_ConvertedValue&0xFFF;
//				R_ADCValue[i+1] = (ADC_ConvertedValue>>16)&0xFFF;
//				printf("\nADC1转换原始值R_ADCValue[][%d] = %04d ",i, ADC_ConvertedValue&0xFFFF);    
//				printf("\r\nADC2转换原始值R_ADCValue[][%d] = %04d ",i+1,(ADC_ConvertedValue>>16)&0xFFF);  		
////				adc2+=(R_ADCValue[j][i]+R_ADCValue[j][i+1]);			
//			}	
////			printf("\r\n参考光源转换原始值总和adc2 = %d \r\n",adc2);  
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
//				printf("\r\nAD转换温度值Temp = %d \r\n", Temp);
//				NTCStop_Collecting();				
//				j++;
//			}
		 
//		for(i=0;i<FFT_LENGTH;i++)//生成信号序列
//		{
//			 fft_inputbuf[2*i]=100+
//												 10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
//								 30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
//												 50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//生成输入信号实部
//			 fft_inputbuf[2*i+1]=0;//虚部全部为0
//		}
//		__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//重设TIM3定时器的计数器值
//		timeout=0;
//		arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
//					time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//计算所用时间
//		sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
//		
//		LCD_ShowString(30+12*8,160,100,16,16,buf);	//显示运行时间		
//		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//把运算结果复数求模得幅值 
//		printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
//		printf("FFT Result:\r\n");
//		for(i=0;i<FFT_LENGTH;i++)
//		{
//			printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
//		}
  }
}

/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9倍频，得到72MHz主时钟
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
       
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1时钟：36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  /* ADC时钟配置 */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6; // 6分频，12MHz
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}	
/**
  * 函数功能: ADC转换完成回调函数
  * 输入参数: hadc：ADC外设设备句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	DMA_Transfer_Complete_Count++; 
	  ADC_NtcValue=HAL_ADC_GetValue(&hadcx3);
}

float  time =0;
/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim==(&A_htimx)){       //定时器3
		timer_count++;		
		printf("\r\n timer_count = %d \r\n", timer_count); 
	}
} 
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
