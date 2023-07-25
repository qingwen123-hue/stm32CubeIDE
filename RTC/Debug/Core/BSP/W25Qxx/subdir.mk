################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/W25Qxx/w25qxx.c 

OBJS += \
./Core/BSP/W25Qxx/w25qxx.o 

C_DEPS += \
./Core/BSP/W25Qxx/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/W25Qxx/%.o Core/BSP/W25Qxx/%.su Core/BSP/W25Qxx/%.cyclo: ../Core/BSP/W25Qxx/%.c Core/BSP/W25Qxx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/BSP/LCD -I../Core/BSP/W25Qxx -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-W25Qxx

clean-Core-2f-BSP-2f-W25Qxx:
	-$(RM) ./Core/BSP/W25Qxx/w25qxx.cyclo ./Core/BSP/W25Qxx/w25qxx.d ./Core/BSP/W25Qxx/w25qxx.o ./Core/BSP/W25Qxx/w25qxx.su

.PHONY: clean-Core-2f-BSP-2f-W25Qxx

