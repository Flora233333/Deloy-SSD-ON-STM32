
/**
 ******************************************************************************
 * @file    app_x-cube-ai.c
 * @author  X-CUBE-AI C code generator
 * @brief   AI program body
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

/*
 * Description
 *   v1.0 - Minimum template to show how to use the Embedded Client API
 *          model. Only one input and one output is supported. All
 *          memory resources are allocated statically (AI_NETWORK_XX, defines
 *          are used).
 *          Re-target of the printf function is out-of-scope.
 *   v2.0 - add multiple IO and/or multiple heap support
 *
 *   For more information, see the embeded documentation:
 *
 *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
 *
 *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
 *   typical : C:\Users\<user_name>\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /* Includes ------------------------------------------------------------------*/

#if defined(__ICCARM__)
#elif defined(__CC_ARM) || (__GNUC__)
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "network.h"
#include "network_data.h"

    /* USER CODE BEGIN includes */
    /* USER CODE END includes */

    /* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
    AI_ALIGNED(4)
    ai_u8 data_in_1[AI_NETWORK_IN_1_SIZE_BYTES] = {0};
    ai_u8 *data_ins[AI_NETWORK_IN_NUM] = {
        data_in_1};
#else
ai_i8 *data_ins[AI_NETWORK_IN_NUM] = {
    NULL};
#endif

#if !defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
    AI_ALIGNED(4)
    ai_i8 data_out_1[AI_NETWORK_OUT_1_SIZE_BYTES] = {0};
    AI_ALIGNED(4)
    ai_i8 data_out_2[AI_NETWORK_OUT_2_SIZE_BYTES] = {0};
    AI_ALIGNED(4)
    ai_i8 data_out_3[AI_NETWORK_OUT_3_SIZE_BYTES] = {0};
    ai_i8 *data_outs[AI_NETWORK_OUT_NUM] = {
        data_out_1,
        data_out_2,
        data_out_3};
#else
ai_i8 *data_outs[AI_NETWORK_OUT_NUM] = {
    NULL,
    NULL,
    NULL};
#endif

    /* Activations buffers -------------------------------------------------------*/

    AI_ALIGNED(32)
    static uint8_t pool0[AI_NETWORK_DATA_ACTIVATION_1_SIZE];

    ai_handle data_activations0[] = {pool0};

    /* AI objects ----------------------------------------------------------------*/

    static ai_handle network = AI_HANDLE_NULL;

    static ai_buffer *ai_input;
    static ai_buffer *ai_output;

    static void ai_log_err(const ai_error err, const char *fct)
    {
        /* USER CODE BEGIN log */
        if (fct)
            printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
                   err.type, err.code);
        else
            printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

        do
        {
        } while (1);
        /* USER CODE END log */
    }

    static int ai_boostrap(ai_handle *act_addr)
    {
        ai_error err;

        /* Create and initialize an instance of the model */
        err = ai_network_create_and_init(&network, act_addr, NULL);
        if (err.type != AI_ERROR_NONE)
        {
            ai_log_err(err, "ai_network_create_and_init");
            return -1;
        }

        ai_input = ai_network_inputs_get(network, NULL);
        ai_output = ai_network_outputs_get(network, NULL);

#if defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
        /*  In the case where "--allocate-inputs" option is used, memory buffer can be
         *  used from the activations buffer. This is not mandatory.
         */
        for (int idx = 0; idx < AI_NETWORK_IN_NUM; idx++)
        {
            data_ins[idx] = ai_input[idx].data;
        }
#else
    for (int idx = 0; idx < AI_NETWORK_IN_NUM; idx++)
    {
        ai_input[idx].data = data_ins[idx];
    }
#endif

#if defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
        /*  In the case where "--allocate-outputs" option is used, memory buffer can be
         *  used from the activations buffer. This is no mandatory.
         */
        for (int idx = 0; idx < AI_NETWORK_OUT_NUM; idx++)
        {
            data_outs[idx] = ai_output[idx].data;
        }
#else
    for (int idx = 0; idx < AI_NETWORK_OUT_NUM; idx++)
    {
        ai_output[idx].data = data_outs[idx];
    }
#endif

        return 0;
    }

    static int ai_run(void)
    {
        ai_i32 batch;

        batch = ai_network_run(network, ai_input, ai_output);
        if (batch != 1)
        {
            ai_log_err(ai_network_get_error(network),
                       "ai_network_run");
            return -1;
        }

        return 0;
    }

    extern __IO uint32_t Camera_Buffer[Display_BufferSize];

    /* USER CODE BEGIN 2 */
    int acquire_and_process_data(ai_u8 *data[])
    {

        Dma2d_Memcpy_PFC((uint32_t *)Camera_Buffer,
                         (uint32_t *)data[0],
                         0, 0, 192, 192, 192,
                         DMA2D_INPUT_RGB565,
                         DMA2D_OUTPUT_RGB888);

        return 0;
    }
    

    PP_OUT_t target[AI_OBJDETECT_SSD_ST_PP_MAX_BOXES_LIMIT];
    PP_OUT_t detections[AI_OBJDETECT_SSD_ST_PP_MAX_PORCESS_LIMIT];

    // 计算两个矩形框的IOU
    float compute_iou(int x1_min, int y1_min, int x1_max, int y1_max,
                      int x2_min, int y2_min, int x2_max, int y2_max)
    {
        int inter_x_min = fmax(x1_min, x2_min);
        int inter_y_min = fmax(y1_min, y2_min);
        int inter_x_max = fmin(x1_max, x2_max);
        int inter_y_max = fmin(y1_max, y2_max);

        int inter_area = fmax(0, inter_x_max - inter_x_min) * fmax(0, inter_y_max - inter_y_min);
        int area1 = (x1_max - x1_min) * (y1_max - y1_min);
        int area2 = (x2_max - x2_min) * (y2_max - y2_min);

        return (float)inter_area / (area1 + area2 - inter_area);
    }

    // 非极大值抑制（NMS）处理
    int nms(PP_OUT_t *detections, int count)
    {
        int selected_count = 0;

        for (int i = 0; i < count && selected_count < AI_OBJDETECT_SSD_ST_PP_MAX_BOXES_LIMIT; i++)
        {
            int keep = 1;
            for (int j = 0; j < selected_count; j++)
            {
                float iou = compute_iou(detections[i].x_min, detections[i].y_min, detections[i].x_max, detections[i].y_max,
                                        target[j].x_min, target[j].y_min, target[j].x_max, target[j].y_max);
                if (iou > AI_OBJDETECT_SSD_ST_PP_IOU_THRESHOLD)
                {
                    keep = 0;
                    break;
                }
            }

            if (keep)
            {
                target[selected_count++] = detections[i];
            }
        }

        return selected_count;
    }

    // 修改后的后处理函数
    int post_process(ai_i8 *data[])
    {
        float *scores = (float *)data[0];
        float *boxes = (float *)data[1];
        float *anchors = (float *)data[2];

        int detection_count = 0;

        for (int i = 0; i < AI_OBJDETECT_SSD_ST_PP_TOTAL_DETECTIONS; i++)
        {
            float class1_conf = scores[i * 2 + 1];

            if (class1_conf < AI_OBJDETECT_SSD_ST_PP_CONF_THRESHOLD)
            {
                continue;
            }

            if (detection_count >= AI_OBJDETECT_SSD_ST_PP_MAX_PORCESS_LIMIT)
            {
                break;
            }

            float xmin = boxes[i * 4 + 0];
            float ymin = boxes[i * 4 + 1];
            float xmax = boxes[i * 4 + 2];
            float ymax = boxes[i * 4 + 3];

            float anchor_x_min = anchors[i * 4 + 0];
            float anchor_y_min = anchors[i * 4 + 1];
            float anchor_x_max = anchors[i * 4 + 2];
            float anchor_y_max = anchors[i * 4 + 3];

            int _xmin = (int)((anchor_x_min + xmin * (anchor_x_max - anchor_x_min)) * IMG_INPUT_WIDTH);
            int _ymin = (int)((anchor_y_min + ymin * (anchor_y_max - anchor_y_min)) * IMG_INPUT_HEIGHT);
            int _xmax = (int)((anchor_x_max + xmax * (anchor_x_max - anchor_x_min)) * IMG_INPUT_WIDTH);
            int _ymax = (int)((anchor_y_max + ymax * (anchor_y_max - anchor_y_min)) * IMG_INPUT_HEIGHT);

            _xmin = _xmin < 0 ? 0 : _xmin;
            _ymin = _ymin < 0 ? 0 : _ymin;
            _xmax = _xmax > IMG_INPUT_WIDTH ? IMG_INPUT_WIDTH : _xmax;
            _ymax = _ymax > IMG_INPUT_HEIGHT ? IMG_INPUT_HEIGHT : _ymax;

            detections[detection_count++] = (PP_OUT_t){_xmin, _ymin, _xmax, _ymax, class1_conf};
        }

        int selected_count = nms(detections, detection_count);

        // for (int i = 0; i < selected_count; i++)
        // {
        //     printf("Target %d - Conf: %.3f; Box: (%d, %d),(%d, %d) \r\n",
        //            i, target[i].conf, target[i].x_min, target[i].y_min, target[i].x_max, target[i].y_max);
        // }

        return selected_count;
    }
    /* USER CODE END 2 */

    /* Entry points --------------------------------------------------------------*/

    void MX_X_CUBE_AI_Init(void)
    {
        /* USER CODE BEGIN 5 */
        printf("\r\nAI Model - initialization\r\n");

        ai_boostrap(data_activations0);

        printf("AI Model - run - main loop\r\n");
        /* USER CODE END 5 */
    }

    // uint16_t img_565[192 * 192] = {0};

    void MX_X_CUBE_AI_Process(void)
    {
        /* USER CODE BEGIN 6 */
        int res = -1;

        if (network)
        {

            /* 1 - acquire and pre-process input data */
            res = acquire_and_process_data(data_ins);
            /* 2 - process the data - call inference engine */
            if (res == 0)
                res = ai_run();
            /* 3- post-process the predictions */
            if (res == 0)
            {
                res = post_process(data_outs);
                if (res > 0)
                {
                    Dma2d_Memcpy_PFC((uint32_t *)data_ins[0],
                                     (uint32_t *)Camera_Buffer,
                                     0, 0, 192, 192, 192,
                                     DMA2D_INPUT_RGB888,
                                     DMA2D_OUTPUT_RGB565);

                    for (int i = 0; i < res; i++)
                    {
                        int x_min = target[i].x_min;
                        int y_min = target[i].y_min;
                        int width = target[i].x_max - target[i].x_min;
                        int height = target[i].y_max - target[i].y_min;

                        // 使用 _LCD_DrawRect 绘制检测框
                        _LCD_DrawRect(x_min, y_min, width, height, IMG_INPUT_WIDTH, (uint16_t *)Camera_Buffer);
                    }

                    res = 0;
                }
            }
        }

        if (res)
        {
            ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
            ai_log_err(err, "Process has FAILED");
        }
        /* USER CODE END 6 */
    }
#ifdef __cplusplus
}
#endif
