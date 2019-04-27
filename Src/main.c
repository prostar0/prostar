/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

int flag ;  // 8-LED_Switching. 4-Sensor B/D , 2-Sensor_flsg, 1-Key_falg
int KEY_VALUE =0x000000FF ;
int KEY_TEMP[4];

int KEY_LED = 0x00000000 ;

int SENSOR_VALUE;
int SENSOR_TEMP[4];


int x1, y1, z1;
int i, j, k;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_GPIO_WritePin(GPIOB, ALM_OUT_Pin, GPIO_PIN_SET);     
       
  HAL_GPIO_WritePin(GPIOC, RESETN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, RESETN_Pin, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOC, RESETN_Pin, GPIO_PIN_SET);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
HAL_I2C_Master_Transmit(&hi2c1, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    
    i++;
    if ( i == 10000000 )
    {
      flag &=  ~1;
      i=0;
    }
    
    KEY_LED =  KEY_VALUE;

     HAL_GPIO_ReadPin(GPIOB, K_LED0_Pin);       
     
//HAL_Delay(1000);
     
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
 // UNUSED(htim);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
//HAL_TIM_Base_Start_IT(&htim6);
  
  int i ;
  
//--------------------------- Start Button Read------------------------------  
  if(htim->Instance == TIM6)
  {
    HAL_GPIO_WritePin(GPIOB, K_LED0_Pin, GPIO_PIN_RESET); // KEY LED Off
    HAL_GPIO_WritePin(GPIOB, K_LED1_Pin, GPIO_PIN_RESET);  
    
    HAL_GPIO_WritePin(GPIOB,BUZ_OUT_Pin,GPIO_PIN_SET) ;
    flag ^= 8;

    KEY_TEMP[3] = KEY_TEMP[2];
    KEY_TEMP[2] = KEY_TEMP[1]; 
    KEY_TEMP[1] = KEY_TEMP[0]; 

    //    K_COL_IN_Init(); // Inout
   
    GPIOB->CRH = ((GPIOB->CRH & 0x0000FFFF) | 0x88880000); //Input Mode : PB12,13,14,15
    GPIOB->ODR = ((GPIOB->ODR & 0xFFFF0FFF) | 0x0000F000);
   
    HAL_GPIO_WritePin(GPIOB, K_ROW0_Pin, GPIO_PIN_RESET); 
    HAL_GPIO_WritePin(GPIOB, K_ROW1_Pin, GPIO_PIN_SET); 

    for (i=0; i<20 ;i++) asm("NOP");
   
    KEY_TEMP[0] = GPIOB->IDR & (K_COL0_Pin | K_COL1_Pin | K_COL2_Pin | K_COL3_Pin); // Read Key Left PB12,13,14,15
    KEY_TEMP[0] >>= 4;
    
    HAL_GPIO_WritePin(GPIOB, K_ROW0_Pin, GPIO_PIN_SET); 
    HAL_GPIO_WritePin(GPIOB, K_ROW1_Pin, GPIO_PIN_RESET);  
    
    for (i=0; i<20 ;i++)  asm("NOP");
    
    KEY_TEMP[0] |= (GPIOB->IDR & (K_COL0_Pin | K_COL1_Pin | K_COL2_Pin | K_COL3_Pin)); // Read Key Right PB12,13,14,15
  
        
    if (KEY_TEMP[3] == KEY_TEMP[2] && KEY_TEMP[2]==KEY_TEMP[1] && KEY_TEMP[1]==KEY_TEMP[0])
    {    
       if ((KEY_TEMP[0] & 0x0000FF00) != 0x0000FF00)
       {
         flag |= 1;
         KEY_VALUE = (KEY_TEMP[3]>>8);
       }
    }
    
    HAL_GPIO_WritePin(GPIOB, K_ROW0_Pin, GPIO_PIN_SET); 
    HAL_GPIO_WritePin(GPIOB, K_ROW1_Pin, GPIO_PIN_SET);  
   
    HAL_GPIO_WritePin(GPIOB,BUZ_OUT_Pin,GPIO_PIN_RESET) ;
    
    
//--------------------------- end of button read------------------------------
//---------------------------- Setting KEY_LED -------------------------------

 //    K_COL_OUT_Init();
     GPIOB->CRH = ((GPIOB->CRH & 0x0000FFFF) | 0x77770000); // Output Mode : PB12,13,14,15
      
     if (flag & 8)
     {
        GPIOB->ODR = ((GPIOB->ODR & 0xFFFF0FFF) | ((KEY_LED<<8) & 0x0000F000));
        HAL_GPIO_WritePin(GPIOB, K_LED0_Pin, GPIO_PIN_SET); 
        HAL_GPIO_WritePin(GPIOB, K_LED1_Pin, GPIO_PIN_RESET);  

     }
     else
     { 
        GPIOB->ODR = ((GPIOB->ODR & 0xFFFF0FFF) | ((KEY_LED<<(4+8)) & 0x0000F000));
        HAL_GPIO_WritePin(GPIOB, K_LED0_Pin, GPIO_PIN_RESET); 
        HAL_GPIO_WritePin(GPIOB, K_LED1_Pin, GPIO_PIN_SET);  
     }
//----------------------- end of KEY_LED Setting------------------------------
//----------------------- ALARM/REMOTE Port Read -----------------------------
     
    HAL_GPIO_WritePin(GPIOC, SPI_CS_Pin, GPIO_PIN_SET);     
     
    SENSOR_TEMP[3] = SENSOR_TEMP[2];
    SENSOR_TEMP[2] = SENSOR_TEMP[1]; 
    SENSOR_TEMP[1] = SENSOR_TEMP[0];   
    
    SENSOR_TEMP[0] = 0;
    for (int j=0; j<9; j++)
    {
      HAL_GPIO_WritePin(GPIOC, SPI_CLK_Pin, GPIO_PIN_RESET);   
       asm("NOP"); 
       
      SENSOR_TEMP[0] <<= 1;
      SENSOR_TEMP[0] |= HAL_GPIO_ReadPin(GPIOC, SPI_MISO_Pin); 
      HAL_GPIO_WritePin(GPIOC, SPI_CLK_Pin, GPIO_PIN_SET);      
       asm("NOP"); 
       asm("NOP"); 
    }    
        
    if (SENSOR_TEMP[3] == SENSOR_TEMP[2] && SENSOR_TEMP[2] == SENSOR_TEMP[1] && SENSOR_TEMP[1] == SENSOR_TEMP[0])
    {
      if ((SENSOR_TEMP[0] != 0x000001FF) &&  (SENSOR_TEMP[0] != 0x000001FE))
       {
         flag |= 2;   // Sensor Signal Input.
         SENSOR_VALUE = SENSOR_TEMP[0]>>1;
       }
       flag |= ((SENSOR_TEMP[0] & 1)<<2) ; // 4 = Sensor Board is present (Sensor board exists)
    } 
    HAL_GPIO_WritePin(GPIOC, SPI_CS_Pin, GPIO_PIN_RESET); 
     
  }
  
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
