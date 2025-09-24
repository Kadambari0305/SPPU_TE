################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/memory\ management.cpp 

CPP_DEPS += \
./src/memory\ management.d 

OBJS += \
./src/memory\ management.o 


# Each subdirectory must supply rules for building sources it contributes
src/memory\ management.o: ../src/memory\ management.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/memory management.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/memory\ management.d ./src/memory\ management.o

.PHONY: clean-src

