/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "usart.h"
#include "sdram.h" 
#include "lcd_image.h"
#include "lcd_spi_200.h"
#include "dcmi_ov5640.h" 
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define AI_OBJDETECT_SSD_ST_PP_NB_CLASSES         (1)
#define AI_OBJDETECT_SSD_ST_PP_IOU_THRESHOLD      (0.5)
#define AI_OBJDETECT_SSD_ST_PP_CONF_THRESHOLD     (0.5)
#define AI_OBJDETECT_SSD_ST_PP_MAX_BOXES_LIMIT    (5)
#define AI_OBJDETECT_SSD_ST_PP_MAX_PORCESS_LIMIT  (50)
#define AI_OBJDETECT_SSD_ST_PP_TOTAL_DETECTIONS   (3830)
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
void Dma2d_Memcpy_PFC(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,
                        uint32_t rowStride, uint32_t input_color_format, uint32_t output_color_format);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
