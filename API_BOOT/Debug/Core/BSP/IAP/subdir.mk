################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/IAP/iap.c 

OBJS += \
./Core/BSP/IAP/iap.o 

C_DEPS += \
./Core/BSP/IAP/iap.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/IAP/%.o Core/BSP/IAP/%.su Core/BSP/IAP/%.cyclo: ../Core/BSP/IAP/%.c Core/BSP/IAP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/BSP/STMFLASH -I../Core/BSP/LCD -I../Core/BSP/IAP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-IAP

clean-Core-2f-BSP-2f-IAP:
	-$(RM) ./Core/BSP/IAP/iap.cyclo ./Core/BSP/IAP/iap.d ./Core/BSP/IAP/iap.o ./Core/BSP/IAP/iap.su

.PHONY: clean-Core-2f-BSP-2f-IAP

