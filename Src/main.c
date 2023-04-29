/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "lv_port_disp.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "stdio.h"
#include "ILI93xx.h"
#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"  
#include "touch.h" 
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "C:\Users\16004\Desktop\stm32\lvgl-8.2.0\examples\get_started\lv_example_get_started.h"
#include "C:\Users\16004\Desktop\stm32\lvgl-8.2.0\src\hal\lv_hal_indev.h"
extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t sins[200];
		uint16_t fangbos[200];
		uint16_t	sanjiaobosu[200];
		uint32_t highs[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void show_win1();
void show_win2();
void SineWaveGen(uint32_t NPoints, float VMaxRange, uint16_t* SineWaveTable)
{
#ifndef PI
#define PI 3.14159265358979323846
#endif

	int    i       = 0;
	double radian  = 0;  // 弧度
	double setup   = 0;  // 弧度和弧度之间的大小
	double voltage = 0;  // 输出电压

	setup = (2 * PI) / NPoints;  // 两点之间的间距

	while (i < NPoints)
	{
		voltage = VMaxRange / 2.0 * (sin(radian) + 1.0);              // 计算电压
		SineWaveTable[i] = voltage * 4096 / 3.3;          // 电压转为DAC数值
		radian += setup;                                              // 下一个点的弧度
		i++;
	}
}
	void sanjiaobo(float top,uint16_t* sanjiaobos)
{
	int i=1;
	double dig=top/3.3*4095;
	double dv=dig/50;
	sanjiaobos[0]=0;
	while(i<=50&&i>0)
	{
		sanjiaobos[i]=sanjiaobos[i-1]+dv;
		i++;
	}
	while(i>50&&i<=100)
	{
		sanjiaobos[i]=sanjiaobos[i-1]-dv;
		i++;
	}
}
void fangbo(float top,uint16_t* fangbos)
{
	int i=0;
	while(i<50&&i>=0)
	{
		fangbos[i]=top/3.3*4095;
		i++;
	}
	while(i>=50&&i<=99)
	{
		fangbos[i]=0;
		i++;
	}
}

static void btn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;
        lv_obj_t* label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void lv_example(void) {
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
}
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
	
		 if(htim->Instance == TIM11)
	  { 
			lv_tick_inc(1);
		}
		if(htim->Instance == TIM10)               //1s触发一次
	  { 
		//show_win1();
//printf("tim10");
		}
		
	}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_TIM11_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_FSMC_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim11);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim8);
 // HAL_TIM_Base_Start_IT(&htim10);

TIM11->ARR=499;
//TIM8->ARR=123; 

	TFTLCD_Init();   
					delay_init(168);               	//初始化延时函数
					KEY_Init();						//初始化KEY
					tp_dev.init();	
					

   SineWaveGen(100,3.0,sins);
		fangbo(3.0,fangbos);
		 sanjiaobo(3.0,sanjiaobosu);
// 初始化lvgl...
lv_init();

// 初始化lvgl底层屏幕驱动
lv_port_disp_init();
lv_port_indev_init();        //需放在disp_init后面
// 初始化lvgl demo
//lv_demo_widgets();
 //lv_example_get_started_2();
//lv_example_scroll_1();
			test_start();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		//printf("gg");
		/*POINT_COLOR=RED;
		LCD_DrawRectangle(0,0,50,50);*/

		/*lv_obj_t* btn = lv_btn_create(lv_scr_act()); 
lv_obj_set_pos(btn, 10, 10);
lv_obj_set_size(btn, 120, 50);
lv_obj_t* label = lv_label_create(btn);
lv_label_set_text(label, "Button");
lv_obj_center(label);*/
		
//lv_example();
		 //lv_tick_inc(1);
    lv_task_handler();
		tp_dev.scan(0);
	//	lv_tick_inc(1);
		

   // delay_ms(1);
	
//	HAL_Delay(5);
		
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch,FILE *f)
{
	 HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,HAL_MAX_DELAY);
return ch;	
}
int fgetc(FILE*f)
{	
	uint8_t ch;
	HAL_UART_Receive(&huart1,(uint8_t*)&ch,1,HAL_MAX_DELAY);
	return ch;	
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
