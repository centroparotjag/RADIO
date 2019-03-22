################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EEPROME.c \
../src/ENCODERS.c \
../src/I2C_FUNC.c \
../src/LCD1602A_KS0066.c \
../src/MENU.c \
../src/RDA5807M.c \
../src/RDS.c \
../src/UART_MY.c \
../src/configuration.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/EEPROME.o \
./src/ENCODERS.o \
./src/I2C_FUNC.o \
./src/LCD1602A_KS0066.o \
./src/MENU.o \
./src/RDA5807M.o \
./src/RDS.o \
./src/UART_MY.o \
./src/configuration.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/EEPROME.d \
./src/ENCODERS.d \
./src/I2C_FUNC.d \
./src/LCD1602A_KS0066.d \
./src/MENU.d \
./src/RDA5807M.d \
./src/RDS.d \
./src/UART_MY.d \
./src/configuration.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F030C8Tx -DDEBUG -DSTM32F030 -DUSE_STDPERIPH_DRIVER -I"D:/MY_razrabotka_hobby/RADIO_RDA5807M/PRG_RDA5807m/StdPeriph_Driver/inc" -I"D:/MY_razrabotka_hobby/RADIO_RDA5807M/PRG_RDA5807m/inc" -I"D:/MY_razrabotka_hobby/RADIO_RDA5807M/PRG_RDA5807m/CMSIS/device" -I"D:/MY_razrabotka_hobby/RADIO_RDA5807M/PRG_RDA5807m/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


