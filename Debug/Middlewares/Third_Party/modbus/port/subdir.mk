################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/modbus/port/mbtask.c \
../Middlewares/Third_Party/modbus/port/portserial.c \
../Middlewares/Third_Party/modbus/port/porttimer.c 

C_DEPS += \
./Middlewares/Third_Party/modbus/port/mbtask.d \
./Middlewares/Third_Party/modbus/port/portserial.d \
./Middlewares/Third_Party/modbus/port/porttimer.d 

OBJS += \
./Middlewares/Third_Party/modbus/port/mbtask.o \
./Middlewares/Third_Party/modbus/port/portserial.o \
./Middlewares/Third_Party/modbus/port/porttimer.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/modbus/port/%.o Middlewares/Third_Party/modbus/port/%.su Middlewares/Third_Party/modbus/port/%.cyclo: ../Middlewares/Third_Party/modbus/port/%.c Middlewares/Third_Party/modbus/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/modbus/ascii -I../Middlewares/Third_Party/modbus/function -I../Middlewares/Third_Party/modbus/include -I../Middlewares/Third_Party/modbus/rtu -I../Middlewares/Third_Party/modbus -I../Middlewares/Third_Party/modbus/port -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-modbus-2f-port

clean-Middlewares-2f-Third_Party-2f-modbus-2f-port:
	-$(RM) ./Middlewares/Third_Party/modbus/port/mbtask.cyclo ./Middlewares/Third_Party/modbus/port/mbtask.d ./Middlewares/Third_Party/modbus/port/mbtask.o ./Middlewares/Third_Party/modbus/port/mbtask.su ./Middlewares/Third_Party/modbus/port/portserial.cyclo ./Middlewares/Third_Party/modbus/port/portserial.d ./Middlewares/Third_Party/modbus/port/portserial.o ./Middlewares/Third_Party/modbus/port/portserial.su ./Middlewares/Third_Party/modbus/port/porttimer.cyclo ./Middlewares/Third_Party/modbus/port/porttimer.d ./Middlewares/Third_Party/modbus/port/porttimer.o ./Middlewares/Third_Party/modbus/port/porttimer.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-modbus-2f-port

