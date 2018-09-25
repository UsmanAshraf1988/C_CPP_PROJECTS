################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/MemoryManager/heap_tlsf.c \
../FreeRTOS/MemoryManager/tlsf.c 

OBJS += \
./FreeRTOS/MemoryManager/heap_tlsf.o \
./FreeRTOS/MemoryManager/tlsf.o 

C_DEPS += \
./FreeRTOS/MemoryManager/heap_tlsf.d \
./FreeRTOS/MemoryManager/tlsf.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/MemoryManager/%.o: ../FreeRTOS/MemoryManager/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -pedantic -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op  -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\STM324xG_EVAL" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\src" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\MemoryManager" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\cmsis" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\system\include\stm32f4-hal" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\include" -I"C:\Users\stusashr\test_stm32f4_MPU_FPU\FreeRTOS\ARM_CM3_MPU" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


