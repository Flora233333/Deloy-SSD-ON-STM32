# Deploy-SSD-ON-STM32

*An interesting project. Deploying SSD Object Detection Models on the STM32*

**I will soon be updating a specific introduction video on Bilibili.**
**Looking forward to your support!**

### 👑 Describption

#### 🎉Model

- st_ssd_mobilenet_v1    ( .h5  ---->  .tflite  ---->  STM32CubeAI quantized)
- Train on keras, use Adam(lr=0.001), 500 epochs.
- Input_shape : (192, 192, 3) (RGB888 data)
- Output_shape (It has three output) :
    1. cls : (1, 3830, 2) 
    2. boxes : (1, 3830, 4)
    3. anchors : (1, 3830, 4)
- Input quantization : uint8 
- Output quantization: Float

#### 🥇MCU

- STM32H750XBH6 (STM32H743 also can run, if the memory enough)
- ROM : 128 kB (Actually, it has 2 mB, as same as STM32H743XI)
- RAM : 1 mB 
- Generated by STM32CubeMX and STM32CubeAI

#### 🎨Project

- Used Flash : 580.31 kB
- Used ROM   : 607.76 kB
- Infer Cost (Excluding postprocess): 190 ms


### 📂 Folder Structure

- deployment on mcu: Contains code and files for deploying the SSD model on STM32.

- model: Includes model files for training and conversion.

- train: Stores scripts for model training using Keras.

### 🧐 Acknowledgment

Many thanks to [STM32 AI Model Zoo](https://github.com/STMicroelectronics/stm32ai-modelzoo) by STMicroelectronics for their valuable work!