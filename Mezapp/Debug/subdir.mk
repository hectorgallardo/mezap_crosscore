################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demodulator.c \
../filter_coeffic.c \
../modem.c \
../modulator.c \
../peripherals.c 

LDF_SRCS += \
../app.ldf 

SRC_OBJS += \
./demodulator.doj \
./filter_coeffic.doj \
./modem.doj \
./modulator.doj \
./peripherals.doj 

C_DEPS += \
./demodulator.d \
./filter_coeffic.d \
./modem.d \
./modulator.d \
./peripherals.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="Mezapp" -proc ADSP-BF537 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -I"C:\Users\Hector\git\mezap_crosscore\Mezapp\system" -I"C:/Analog Devices/ADSP-BF537_EZKIT-Rel1.0.0/BF537_EZ-KIT_Lite/Blackfin/include" -I"C:\Analog Devices\CrossCore Embedded Studio 1.0.2\Blackfin\lib\src\libdsp" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -sdram -gnu-style-dependencies -MD -Mo "demodulator.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


