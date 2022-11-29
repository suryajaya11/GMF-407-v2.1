################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/can_stuff.c \
../Core/Lib/hydraulic.c \
../Core/Lib/radio.c \
../Core/Lib/votol.c 

OBJS += \
./Core/Lib/can_stuff.o \
./Core/Lib/hydraulic.o \
./Core/Lib/radio.o \
./Core/Lib/votol.o 

C_DEPS += \
./Core/Lib/can_stuff.d \
./Core/Lib/hydraulic.d \
./Core/Lib/radio.d \
./Core/Lib/votol.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/%.o: ../Core/Lib/%.c Core/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Surya/STM32CubeIDE/workspace_1.7.0/GMF 407 v2.1/Core/Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

