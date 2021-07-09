#include "stm32f4xx_hal.h"
void GPIO_Config(void);
void ADC_Config(void);
void TIM_Config(void);

ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim2;
	uint32_t adcVal = 0;
	uint32_t terry_counter = 0;
int main(void)
{
	 /* Initialize all configured peripherals */
  GPIO_Config();
  ADC_Config();
  TIM_Config();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim2);
	HAL_ADC_Start_IT(&hadc1);
	
}

void GPIO_Config(void)
{
		
		if(1)// HAL layer
		{
			
			__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO_InitTypeDef GPIO_InitStruct;
			GPIO_InitStruct.Pin = GPIO_PIN_1;
			GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			//GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			
			//Led Pins Config
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 |GPIO_PIN_14 |GPIO_PIN_15;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			
			//Enable Systick Interrupt
			HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
			HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
			HAL_NVIC_SetPriority(ADC_IRQn,0,0);
			HAL_NVIC_EnableIRQ(ADC_IRQn);
		}		
}

void ADC_Config(void)
{
	//Enable ADC clock
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	//ADC basic config
	hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.Resolution = ADC_RESOLUTION_10B;
	HAL_ADC_Init(&hadc1);
	
	ADC_ChannelConfTypeDef sConfig = {0};
	
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Offset = 0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
}


void TIM_Config(void)
{

  //TIM clock enable
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	//TIM basic initialization
  htim2.Instance = TIM2;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Prescaler = 16000;
  htim2.Init.Period = 100;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim2);
 
	//TI< clock source config
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_StatusTypeDef hr;
  hr = HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
 
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  hr =HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
  

}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc1);
		
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/*Prevent unused argument(s) compilation warning*/
	UNUSED(hadc);
	
	adcVal = HAL_ADC_GetValue(&hadc1);
	HAL_GPIO_TogglePin(GPIOD,  GPIO_PIN_13 );
	terry_counter++;
	
}
