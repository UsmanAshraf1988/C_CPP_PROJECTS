################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/adc.cpp \
../src/bargraph.cpp \
../src/button.cpp \
../src/led_blinker.cpp \
../src/main.cpp \
../src/system_support.cpp \
../src/uart.cpp \
../src/uart_task.cpp 

OBJS += \
./src/adc.o \
./src/bargraph.o \
./src/button.o \
./src/led_blinker.o \
./src/main.o \
./src/system_support.o \
./src/uart.o \
./src/uart_task.o 

CPP_DEPS += \
./src/adc.d \
./src/bargraph.d \
./src/button.d \
./src/led_blinker.d \
./src/main.d \
./src/system_support.d \
./src/uart.d \
./src/uart_task.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -pedantic -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op  -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\STM324xG_EVAL" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\src" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\MemoryManager" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\cmsis" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\stm32f4-hal" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\ARM_CM3_MPU" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


