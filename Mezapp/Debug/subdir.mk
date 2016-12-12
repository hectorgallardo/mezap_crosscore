################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demodulator.c \
../modem.c \
../modulador.c 

LDF_SRCS += \
../app.ldf 

SRC_OBJS += \
./demodulator.doj \
./modem.doj \
./modulador.doj 

C_DEPS += \
./demodulator.d \
./modem.d \
./modulador.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="AudioLoopback" -proc ADSP-BF537 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -I"C:\Users\Hector\Desktop\MONDRAGON UNIBERTSITATEA\3. Telecomunicaciones\POPBL5\WS_EJEMPLO_COMPLETO\WS_EJEMPLO\AudioLoopback\system" -I"C:/Analog Devices/ADSP-BF537_EZKIT-Rel1.0.0/BF537_EZ-KIT_Lite/Blackfin/include" -I"C:\Analog Devices\CrossCore Embedded Studio 1.0.2\Blackfin\lib\src\libdsp" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -sdram -gnu-style-dependencies -MD -Mo "demodulator.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


