/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DIODE_Pin GPIO_PIN_13
#define DIODE_GPIO_Port GPIOC
#define BLUETOOTH_EN_Pin GPIO_PIN_14
#define BLUETOOTH_EN_GPIO_Port GPIOC
#define KEY_Pin GPIO_PIN_0
#define KEY_GPIO_Port GPIOA
#define KEY_EXTI_IRQn EXTI0_IRQn
#define PUMP1_CURR_MEAS_Pin GPIO_PIN_4
#define PUMP1_CURR_MEAS_GPIO_Port GPIOA
#define PUMP2_CURR_MEAS_Pin GPIO_PIN_5
#define PUMP2_CURR_MEAS_GPIO_Port GPIOA
#define PUMP3_CURR_MEAS_Pin GPIO_PIN_6
#define PUMP3_CURR_MEAS_GPIO_Port GPIOA
#define PUMP4_CURR_MEAS_Pin GPIO_PIN_7
#define PUMP4_CURR_MEAS_GPIO_Port GPIOA
#define PUMP5_CURR_MEAS_Pin GPIO_PIN_0
#define PUMP5_CURR_MEAS_GPIO_Port GPIOB
#define BLUETOOTH_RX_Pin GPIO_PIN_10
#define BLUETOOTH_RX_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define BLUETOOTH_TX_Pin GPIO_PIN_15
#define BLUETOOTH_TX_GPIO_Port GPIOA
#define PUMP5_Pin GPIO_PIN_3
#define PUMP5_GPIO_Port GPIOB
#define PUMP4_Pin GPIO_PIN_4
#define PUMP4_GPIO_Port GPIOB
#define PUMP3_Pin GPIO_PIN_5
#define PUMP3_GPIO_Port GPIOB
#define PUMP2_Pin GPIO_PIN_6
#define PUMP2_GPIO_Port GPIOB
#define PUMP1_Pin GPIO_PIN_7
#define PUMP1_GPIO_Port GPIOB
#define GLASS_SCALE_SCK_Pin GPIO_PIN_8
#define GLASS_SCALE_SCK_GPIO_Port GPIOB
#define GLASS_SCALE_DATA_Pin GPIO_PIN_9
#define GLASS_SCALE_DATA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
