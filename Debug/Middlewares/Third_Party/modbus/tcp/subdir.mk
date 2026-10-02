################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/modbus/tcp/mbtcp.c 

C_DEPS += \
./Middlewares/Third_Party/modbus/tcp/mbtcp.d 

OBJS += \
./Middlewares/Third_Party/modbus/tcp/mbtcp.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/modbus/tcp/%.o Middlewares/Third_Party/modbus/tcp/%.su Middlewares/Third_Party/modbus/tcp/%.cyclo: ../Middlewares/Third_Party/modbus/tcp/%.c Middlewares/Third_Party/modbus/tcp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/modbus/ascii -I../Middlewares/Third_Party/modbus/function -I../Middlewares/Third_Party/modbus/include -I../Middlewares/Third_Party/modbus/rtu -I../Middlewares/Third_Party/modbus -I../Middlewares/Third_Party/modbus/port -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-modbus-2f-tcp

clean-Middlewares-2f-Third_Party-2f-modbus-2f-tcp:
	-$(RM) ./Middlewares/Third_Party/modbus/tcp/mbtcp.cyclo ./Middlewares/Third_Party/modbus/tcp/mbtcp.d ./Middlewares/Third_Party/modbus/tcp/mbtcp.o ./Middlewares/Third_Party/modbus/tcp/mbtcp.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-modbus-2f-tcp

