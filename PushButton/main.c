/* Code to control onboard LEDs of STM32F4 discovery board with onboard push button */
/* Include header file of STM32F4 series microcontroller */
#include "stm32f4xx_hal.h"

/* Function protoypes to configure GPIO pins as digital output and digital input */
void Init_OnBoard_LEDs(void);
void configure_Button(void);
void Delay_ms(volatile int time_ms); //ms delay function 

/* main code to call initialize functions, read state of push button and controlling LEDs */
void configreLEDPins(void);
void msDelay(uint32_t msTime);
int main_pushButton(void);


//EXTERN LED ,Pa0 push button PC1 Led, PC2 exten push button
 int main(void)
{
	HAL_Init();
	//config PC1 as ouput for led
	{
	__HAL_RCC_GPIOC_CLK_ENABLE(); //ut;  // declare a variab
	
	//Enable clock to GPIOA
	GPIO_InitTypeDef external_led_ouput;
	//config PE9 as ouput for buttonle of type struct GPIO_InitTypeDef
	external_led_ouput.Mode = GPIO_MODE_OUTPUT_PP; // set pin mode to input
	external_led_ouput.Pin = GPIO_PIN_1;  // select pin PC1 only
	HAL_GPIO_Init(GPIOC, &external_led_ouput); //  initialize PA0 pins by passing port name and address of PushButton struct
	}
	
	//config PE9 as input for button, i connect to ground, when button press, it will drain to group. so set it as a pull up resistor mode
	if(1)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE(); //Enable clock to GPIOA
		GPIO_InitTypeDef external_PE9_button;  // declare a variable of type struct GPIO_InitTypeDef
		external_PE9_button.Mode = GPIO_MODE_INPUT; // set pin mode to input
		external_PE9_button.Pin = GPIO_PIN_9;  // select pin PC1 only
		external_PE9_button.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOE, &external_PE9_button); //  initialize PA0 pins by passing port name and address of PushButton struct
	}
	
	//config PC3 as input for button, when button press, it will drain to group. so set it as a pull up resistor mode
	if(1)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE(); //Enable clock to GPIOC
		GPIO_InitTypeDef external_PC3_button;  // declare a variable of type struct GPIO_InitTypeDef
		external_PC3_button.Mode = GPIO_MODE_INPUT; // set pin mode to input
		external_PC3_button.Pin = GPIO_PIN_3;  // select pin PC1 only
		external_PC3_button.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOC, &external_PC3_button); //  initialize PA0 pins by passing port name and address of PushButton struct
	}
	
	Init_OnBoard_LEDs();
	
	configure_Button(); // call Push button GPIO pins initialization function 
	GPIO_PinState PA0_button_state; // Define a enum struct which contain boolean states 
	GPIO_PinState PC3_button_state; // Define a enum struct which contain boolean states 
	GPIO_PinState PE9_button_state; // Define a enum struct which contain boolean states 
   while(1)
   {
		
		PA0_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); // read state of push button and save it in "state" variable
		PE9_button_state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9); // read state of push button and save it in "state" variable
		PC3_button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3); // read state of push button and save it in "state" variable
		 
		// if state is high, turn on  LEDs
    if( ( GPIO_PIN_SET == PA0_button_state) || ( GPIO_PIN_RESET == PE9_button_state ) || ( GPIO_PIN_RESET == PC3_button_state ))
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
		}
// if state is low, turn off LEDs
    else
    {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
    }
		
		Delay_ms(50);
   }
	 
}
int main_Led(void)
{
	//1:Configre LEDs pins
	configreLEDPins();
	
	//2.Define a delay function
	
	while(1)
	{
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 |GPIO_PIN_15);
		GPIOD-> ODR ^= (0xfUL<<12);
		Delay_ms(1000);
	}
	
}

void configreLEDPins(void)
{
	
	if(1) //LL layer
	{
		//__HAL_RCC_GPIOD_CLK_ENABLE();
		RCC->AHB1ENR |= (1<<3);
		
		//set mode as output for GPIO Pins 12-15
		GPIOD->MODER &= ~(0xffUL << 12*2); //reset mode
		GPIOD->MODER |= (0x55 <<12*2);
	}
		else
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
	
		GPIO_InitTypeDef myLEDConfig;
		myLEDConfig.Mode = GPIO_MODE_OUTPUT_PP;
		myLEDConfig.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 |GPIO_PIN_15;
		HAL_GPIO_Init(GPIOD, &myLEDConfig);
		}
	
	
}

int main_pushButton(void)
{
	Init_OnBoard_LEDs(); // calls LEDs GPIO pins initialization function 
	configure_Button(); // call Push button GPIO pins initialization function 
	GPIO_PinState state; // Define a enum struct which contain boolean states 
   while(1)
   {
		
state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); // read state of push button and save it in "state" variable
 // if state is high, turn on  LEDs
    if(state)
	{
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
	}
// if state is low, turn off LEDs
    else
       {
HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
       }
   }
}

/* Function to configure PD12-PD15 pin of as digital output pins */
void Init_OnBoard_LEDs(void)
{
	 __HAL_RCC_GPIOD_CLK_ENABLE(); //Enable clock to GPIOD
	GPIO_InitTypeDef BoardLEDs; // declare a variable of type struct GPIO_InitTypeDef
	BoardLEDs.Mode = GPIO_MODE_OUTPUT_PP; // set pin mode to output 
	BoardLEDs.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; // select pins PD12-PD15 
	HAL_GPIO_Init(GPIOD, &BoardLEDs); // initialize PD12-PD15 pins by passing port name and address of BoardLEDs struct
}

/* Function to configure PA0 pin of as adigital input pin */
void configure_Button(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); //Enable clock to GPIOA
	GPIO_InitTypeDef PushButton;  // declare a variable of type struct GPIO_InitTypeDef
	PushButton.Mode = GPIO_MODE_INPUT; // set pin mode to input
	PushButton.Pin = GPIO_PIN_0;  // select pin PA0 only
	PushButton.Pull = GPIO_NOPULL; // set no internal pull-up or pull-down resistor
	HAL_GPIO_Init(GPIOA, &PushButton); //  initialize PA0 pins by passing port name and address of PushButton struct
}

/* ms delay function */
void Delay_ms(volatile int time_ms)
{
	      int j;
        for(j = 0; j < time_ms*4000; j++)
            {}  /* excute NOP for 1ms */
}
