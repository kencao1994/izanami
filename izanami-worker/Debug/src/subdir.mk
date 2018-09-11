################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ifile.c \
../src/iregion.c \
../src/izanami-worker.c \
../src/main.c \
../src/readaction.c \
../src/workerexecutor.c 

OBJS += \
./src/ifile.o \
./src/iregion.o \
./src/izanami-worker.o \
./src/main.o \
./src/readaction.o \
./src/workerexecutor.o 

C_DEPS += \
./src/ifile.d \
./src/iregion.d \
./src/izanami-worker.d \
./src/main.d \
./src/readaction.d \
./src/workerexecutor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/caojx/workspace/mydb/izanami-common/src -I/home/caojx/download/iniparser-master/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


