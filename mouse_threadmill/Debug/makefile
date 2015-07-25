################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: mouse_threadmill.axf

# Tool invocations
mouse_threadmill.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Piotr\Documents\LPCXpresso_7.8.0_426\mouse_threadmill\lpc_board_nxp_lpcxpresso_1769\Debug" -L"C:\Users\Piotr\Documents\LPCXpresso_7.8.0_426\mouse_threadmill\lpc_chip_175x_6x\Debug" -Xlinker -Map="mouse_threadmill.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "mouse_threadmill_Debug.ld" -o "mouse_threadmill.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) mouse_threadmill.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "mouse_threadmill.axf"; # arm-none-eabi-objcopy -v -O binary "mouse_threadmill.axf" "mouse_threadmill.bin" ; # checksum -p LPC1769 -d "mouse_threadmill.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
