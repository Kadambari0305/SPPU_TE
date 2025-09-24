################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/page\ replacement.cpp 

CPP_DEPS += \
./src/page\ replacement.d 

OBJS += \
./src/page\ replacement.o 


# Each subdirectory must supply rules for building sources it contributes
src/page\ replacement.o: ../src/page\ replacement.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/page replacement.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/page\ replacement.d ./src/page\ replacement.o

.PHONY: clean-src

