################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ADC.c" \
"../Sources/App.c" \
"../Sources/FTM.c" \
"../Sources/GPIO.c" \
"../Sources/LDM.c" \
"../Sources/LDR.c" \
"../Sources/Quanser.c" \
"../Sources/SysTick.c" \
"../Sources/UART.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/ADC.c \
../Sources/App.c \
../Sources/FTM.c \
../Sources/GPIO.c \
../Sources/LDM.c \
../Sources/LDR.c \
../Sources/Quanser.c \
../Sources/SysTick.c \
../Sources/UART.c \
../Sources/main.c \

OBJS += \
./Sources/ADC.o \
./Sources/App.o \
./Sources/FTM.o \
./Sources/GPIO.o \
./Sources/LDM.o \
./Sources/LDR.o \
./Sources/Quanser.o \
./Sources/SysTick.o \
./Sources/UART.o \
./Sources/main.o \

C_DEPS += \
./Sources/ADC.d \
./Sources/App.d \
./Sources/FTM.d \
./Sources/GPIO.d \
./Sources/LDM.d \
./Sources/LDR.d \
./Sources/Quanser.d \
./Sources/SysTick.d \
./Sources/UART.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/ADC.o" \
"./Sources/App.o" \
"./Sources/FTM.o" \
"./Sources/GPIO.o" \
"./Sources/LDM.o" \
"./Sources/LDR.o" \
"./Sources/Quanser.o" \
"./Sources/SysTick.o" \
"./Sources/UART.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/ADC.d" \
"./Sources/App.d" \
"./Sources/FTM.d" \
"./Sources/GPIO.d" \
"./Sources/LDM.d" \
"./Sources/LDR.d" \
"./Sources/Quanser.d" \
"./Sources/SysTick.d" \
"./Sources/UART.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/ADC.o \
./Sources/App.o \
./Sources/FTM.o \
./Sources/GPIO.o \
./Sources/LDM.o \
./Sources/LDR.o \
./Sources/Quanser.o \
./Sources/SysTick.o \
./Sources/UART.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ADC.o: ../Sources/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ADC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/App.o: ../Sources/App.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/App.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/App.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/FTM.o: ../Sources/FTM.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/FTM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/FTM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GPIO.o: ../Sources/GPIO.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPIO.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPIO.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/LDM.o: ../Sources/LDM.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/LDM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/LDM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/LDR.o: ../Sources/LDR.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/LDR.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/LDR.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Quanser.o: ../Sources/Quanser.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Quanser.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Quanser.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SysTick.o: ../Sources/SysTick.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/SysTick.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/SysTick.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART.o: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/UART.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/UART.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


