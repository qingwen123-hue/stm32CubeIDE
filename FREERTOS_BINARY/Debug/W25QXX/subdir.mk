################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./W25QXX/w25qxx.o 

C_DEPS += \
./W25QXX/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
W25QXX/w25qxx.o: D:/ykl/STM32CubeIDE_1.12.1/workspace/SPI/Drivers/W25QXX/w25qxx.c W25QXX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/ykl/STM32CubeIDE_1.12.1/workspace/PublicDrivers/LCD" -I"D:/ykl/STM32CubeIDE_1.12.1/workspace/SPI/Drivers/W25QXX" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-W25QXX

clean-W25QXX:
	-$(RM) ./W25QXX/w25qxx.cyclo ./W25QXX/w25qxx.d ./W25QXX/w25qxx.o ./W25QXX/w25qxx.su

.PHONY: clean-W25QXX

