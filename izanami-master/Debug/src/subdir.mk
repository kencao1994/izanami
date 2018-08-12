################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/izanami-master.c \
../src/main.c \
../src/masterexecutor.c 

OBJS += \
./src/izanami-master.o \
./src/main.o \
./src/masterexecutor.o 

C_DEPS += \
./src/izanami-master.d \
./src/main.d \
./src/masterexecutor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/caojx/workspace/mydb/izanami-common/src -I/home/caojx/download/iniparser-master/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


