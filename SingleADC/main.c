#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_gpio.h"
//ADC_HandleTypeDef hadc1;
uint32_t adc_value = 0;
HAL_StatusTypeDef status;

void GPIO_Config(void);
void ADC_Config(void);
void Systick_config(void);
ADC_HandleTypeDef hadc1;
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
uint32_t counter;
uint32_t tickstart;

uint32_t try_count = 0;
int main(void)
{
	HAL_Init();
	Systick_config();
	GPIO_Config();
	ADC_Config();
	
	
	
	while(1)
	{
		if(1) //register version
		{
			//1.Start ADC
			ADC1->CR2 |= ADC_CR2_ADON;
			ADC1->CR2 |= ( 1<<30);
			
			if(0)//Terry this is put from demo code, yet i think it can be ommitted
			{
				//Stabilizer the ADC clock;
				counter = (ADC_STAB_DELAY_US * (SystemCoreClock/1000000U));
				while(counter !=0U)
				{
					counter--;
				}
			}
			
			//Poll for the end of conversion
			tickstart = 0U;
			tickstart = HAL_GetTick();
			while((ADC1->SR & 0x2) != 0x2)
			{
				if(( HAL_GetTick() - tickstart) > 10)
				{
					break;
				}
			}
			
			//read the ADC value
			++try_count;
			adc_value = ADC1->DR;
			
		
			
			
			if(0)//Terry this is put from demo code, yet i think it can be ommitted
			{
					//Stop the ADC
					ADC1->CR2 &= ~0x1;
					ADC1->SR = 0x00;
				
				
				//2.100ms delay
				HAL_Delay(100);
			}
			
		}else //HAL version
		{
			
		HAL_ADC_Start(&hadc1);
		status = HAL_ADC_PollForConversion(&hadc1, 5);
		
		if(status == HAL_OK)
		{
			adc_value = HAL_ADC_GetValue(&hadc1);
			
		}
		HAL_ADC_Stop(&hadc1);
		HAL_Delay(100);
	}
		
	}
}

void GPIO_Config(void)
{
	if(0)//MX generated code
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//Terry add
	{
			GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}

	
	}
	else{
		
		if(0)// HAL layer
		{
			
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pin = GPIO_PIN_1;
		
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}
		
		if(1)//register layer
		{
			//__HAL_RCC_GPIOA_CLK_ENABLE();
			RCC->AHB1ENR |= (0x01 );
		
			//GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
			//GPIO_InitStruct.Pin = GPIO_PIN_1;
			GPIOA->MODER |=( 0x03 << 1*2); //1 group, each group 2 bits
			
			
			//GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIOA->PUPDR &= ~(0x0000003 << 1*2);
		}
	}
}

void ADC_Config(void)
{
	if(0)//MX generated code
	{
		/* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
	
  /* USER CODE END ADC1_Init 2 */
	
	}
	else
	{
		
		//hadc1 initial is need for the main functino usage, however in register layer, we do not need actually call HAL_ADC_Init and HAL_ADC_ConfigChannel
		{
		__HAL_RCC_ADC1_CLK_ENABLE();
		
		hadc1.Instance = ADC1;
		hadc1.Init.Resolution = ADC_RESOLUTION_8B;
		hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; //register align to 0-9 bits for 8 bit accuacy
		
	
		}
	if(0)//Hal version
	{
	
		HAL_ADC_Init(&hadc1);
		
		
		ADC_ChannelConfTypeDef myADCchannelTypedDef;
		myADCchannelTypedDef.Channel = ADC_CHANNEL_1;
		myADCchannelTypedDef.Rank = 1;
		myADCchannelTypedDef.SamplingTime = ADC_SAMPLETIME_15CYCLES;
		HAL_ADC_ConfigChannel(&hadc1, &myADCchannelTypedDef);
	}
	if(1) //register layer
	{
		__HAL_RCC_ADC1_CLK_ENABLE();
		ADC1->CR1 =  (0x2 << 24); //Set the resolution to 8 bits 
		ADC1->CR1 |= (0x1); //adc channel 1
		
		ADC1 ->CR2 = (0x1 << 10); //EOC flag at the end of regular channel conversion
		
		ADC1->SMPR2 = (0x1 << 1*3); //
		ADC1->SQR1 = (0x0 << 20); // Number of conversion =1
		ADC1 ->SQR3 = 0x01;//sequncier to convert channel 1 first (it is the only
		
	}
	
	
	}
	
	
}

void Systick_config(void)
{
	if(0)//MX generated code
	{
			RCC_OscInitTypeDef RCC_OscInitStruct = {0};
			RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

			/** Configure the main internal regulator output voltage
			*/
			__HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
			/** Initializes the RCC Oscillators according to the specified parameters
			* in the RCC_OscInitTypeDef structure.
			*/
			RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
			RCC_OscInitStruct.HSIState = RCC_HSI_ON;
			RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
			RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
			if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
			{
				Error_Handler();
			}
			/** Initializes the CPU, AHB and APB buses clocks
			*/
			RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
																	|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
			RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
			RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
			RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

			if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
			{
				Error_Handler();
			}	
	
	}else
	{
		//Set clock source and speed
		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
		//Systick interruput priority and enable
		HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
		HAL_NVIC_EnableIRQ(SysTick_IRQn);
	}
	
}


void SysTick_Handler(void)
{
	HAL_IncTick();
}
