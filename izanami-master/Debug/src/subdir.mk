################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/izanami-master.c \
../src/main.c \
../src/masterexecutor.c \
../src/masteriregioninfo.c \
../src/mastersideworker.c 

OBJS += \
./src/izanami-master.o \
./src/main.o \
./src/masterexecutor.o \
./src/masteriregioninfo.o \
./src/mastersideworker.o 

C_DEPS += \
./src/izanami-master.d \
./src/main.d \
./src/masterexecutor.d \
./src/masteriregioninfo.d \
./src/mastersideworker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/caojx/workspace/mydb/izanami-common/src -I/home/caojx/download/iniparser-master/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


