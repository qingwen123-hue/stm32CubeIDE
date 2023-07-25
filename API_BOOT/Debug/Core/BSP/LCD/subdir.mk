################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/LCD/lcd.c 

OBJS += \
./Core/BSP/LCD/lcd.o 

C_DEPS += \
./Core/BSP/LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/LCD/%.o Core/BSP/LCD/%.su Core/BSP/LCD/%.cyclo: ../Core/BSP/LCD/%.c Core/BSP/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/BSP/STMFLASH -I../Core/BSP/LCD -I../Core/BSP/IAP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-LCD

clean-Core-2f-BSP-2f-LCD:
	-$(RM) ./Core/BSP/LCD/lcd.cyclo ./Core/BSP/LCD/lcd.d ./Core/BSP/LCD/lcd.o ./Core/BSP/LCD/lcd.su

.PHONY: clean-Core-2f-BSP-2f-LCD

