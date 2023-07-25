################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/W25QXX/w25qxx.c 

OBJS += \
./Drivers/W25QXX/w25qxx.o 

C_DEPS += \
./Drivers/W25QXX/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/W25QXX/%.o Drivers/W25QXX/%.su Drivers/W25QXX/%.cyclo: ../Drivers/W25QXX/%.c Drivers/W25QXX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/ykl/STM32CubeIDE_1.12.1/workspace/PublicDrivers/LCD" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-W25QXX

clean-Drivers-2f-W25QXX:
	-$(RM) ./Drivers/W25QXX/w25qxx.cyclo ./Drivers/W25QXX/w25qxx.d ./Drivers/W25QXX/w25qxx.o ./Drivers/W25QXX/w25qxx.su

.PHONY: clean-Drivers-2f-W25QXX

