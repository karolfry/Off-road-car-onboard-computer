################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include 1Wire/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
pomiar_jasnosci.lss \

FLASH_IMAGE += \
pomiar_jasnosci.hex \

EEPROM_IMAGE += \
pomiar_jasnosci.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: pomiar_jasnosci.elf secondary-outputs

# Tool invocations
pomiar_jasnosci.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,pomiar_jasnosci.map -mmcu=atmega8 -o "pomiar_jasnosci.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

pomiar_jasnosci.lss: pomiar_jasnosci.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S pomiar_jasnosci.elf  >"pomiar_jasnosci.lss"
	@echo 'Finished building: $@'
	@echo ' '

pomiar_jasnosci.hex: pomiar_jasnosci.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex pomiar_jasnosci.elf  "pomiar_jasnosci.hex"
	@echo 'Finished building: $@'
	@echo ' '

pomiar_jasnosci.eep: pomiar_jasnosci.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex pomiar_jasnosci.elf  "pomiar_jasnosci.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: pomiar_jasnosci.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 pomiar_jasnosci.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) pomiar_jasnosci.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
