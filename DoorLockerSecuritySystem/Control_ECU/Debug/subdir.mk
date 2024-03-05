################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../buzzer.c \
../control.c \
../eeprom.c \
../gpio.c \
../i2c.c \
../motor.c \
../timer.c \
../uart.c 

OBJS += \
./buzzer.o \
./control.o \
./eeprom.o \
./gpio.o \
./i2c.o \
./motor.o \
./timer.o \
./uart.o 

C_DEPS += \
./buzzer.d \
./control.d \
./eeprom.d \
./gpio.d \
./i2c.d \
./motor.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


