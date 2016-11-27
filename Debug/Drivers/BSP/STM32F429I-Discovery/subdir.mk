################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.c \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_gyroscope.c \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_io.c \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_lcd.c \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_sdram.c \
../Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_ts.c 

OBJS += \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.o \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_gyroscope.o \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_io.o \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_lcd.o \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_sdram.o \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.d \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_gyroscope.d \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_io.d \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_lcd.d \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_sdram.d \
./Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F429I-Discovery/%.o: ../Drivers/BSP/STM32F429I-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"/home/fu/workspace/Proyecto/Inc" -I"/home/fu/workspace/Proyecto/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/fu/workspace/Proyecto/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/fu/workspace/Proyecto/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/fu/workspace/Proyecto/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/home/fu/workspace/Proyecto/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/home/fu/workspace/Proyecto/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/fu/workspace/Proyecto/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/fu/workspace/Proyecto/Drivers/CMSIS/Include" -I"/home/fu/workspace/Proyecto/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/fu/workspace/Proyecto/Drivers/BSP/Components/Common" -I"/home/fu/workspace/Proyecto/Drivers/BSP/Components/ili9341" -I"/home/fu/workspace/Proyecto/Drivers/BSP/Components/l3gd20" -I"/home/fu/workspace/Proyecto/Drivers/BSP/STM32F429I-Discovery" -I"/home/fu/workspace/Proyecto/Utilities/Fonts" -I"/home/fu/workspace/Proyecto/Drivers/BSP/Components/stmpe811"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


