################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/STMFLASH/stmflash.c 

OBJS += \
./Core/BSP/STMFLASH/stmflash.o 

C_DEPS += \
./Core/BSP/STMFLASH/stmflash.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/STMFLASH/%.o Core/BSP/STMFLASH/%.su Core/BSP/STMFLASH/%.cyclo: ../Core/BSP/STMFLASH/%.c Core/BSP/STMFLASH/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/BSP/STMFLASH -I../Core/BSP/LCD -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-STMFLASH

clean-Core-2f-BSP-2f-STMFLASH:
	-$(RM) ./Core/BSP/STMFLASH/stmflash.cyclo ./Core/BSP/STMFLASH/stmflash.d ./Core/BSP/STMFLASH/stmflash.o ./Core/BSP/STMFLASH/stmflash.su

.PHONY: clean-Core-2f-BSP-2f-STMFLASH

