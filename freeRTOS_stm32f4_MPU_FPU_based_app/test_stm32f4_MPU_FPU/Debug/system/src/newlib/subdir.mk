################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/newlib/_exit.c \
../system/src/newlib/_sbrk.c \
../system/src/newlib/_startup.c \
../system/src/newlib/_syscalls.c \
../system/src/newlib/assert.c 

CPP_SRCS += \
../system/src/newlib/_cxx.cpp 

OBJS += \
./system/src/newlib/_cxx.o \
./system/src/newlib/_exit.o \
./system/src/newlib/_sbrk.o \
./system/src/newlib/_startup.o \
./system/src/newlib/_syscalls.o \
./system/src/newlib/assert.o 

C_DEPS += \
./system/src/newlib/_exit.d \
./system/src/newlib/_sbrk.d \
./system/src/newlib/_startup.d \
./system/src/newlib/_syscalls.d \
./system/src/newlib/assert.d 

CPP_DEPS += \
./system/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/%.o: ../system/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -pedantic -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op  -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\STM324xG_EVAL" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\src" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\MemoryManager" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\cmsis" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\stm32f4-hal" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\ARM_CM3_MPU" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/%.o: ../system/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -pedantic -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op  -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\STM324xG_EVAL" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\src" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\MemoryManager" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\cmsis" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\stm32f4-hal" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\ARM_CM3_MPU" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/_startup.o: ../system/src/newlib/_startup.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -pedantic -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op  -g3 -DOS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\STM324xG_EVAL" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\src" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\MemoryManager" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\cmsis" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\stm32f4-hal" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\ARM_CM3_MPU" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"system/src/newlib/_startup.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


