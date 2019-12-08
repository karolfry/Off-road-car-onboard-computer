################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
wyswietlacz_spi.lss \

FLASH_IMAGE += \
wyswietlacz_spi.hex \

EEPROM_IMAGE += \
wyswietlacz_spi.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: wyswietlacz_spi.elf secondary-outputs

# Tool invocations
wyswietlacz_spi.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,wyswietlacz_spi.map -mmcu=atmega8 -o "wyswietlacz_spi.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

wyswietlacz_spi.lss: wyswietlacz_spi.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S wyswietlacz_spi.elf  >"wyswietlacz_spi.lss"
	@echo 'Finished building: $@'
	@echo ' '

wyswietlacz_spi.hex: wyswietlacz_spi.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex wyswietlacz_spi.elf  "wyswietlacz_spi.hex"
	@echo 'Finished building: $@'
	@echo ' '

wyswietlacz_spi.eep: wyswietlacz_spi.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex wyswietlacz_spi.elf  "wyswietlacz_spi.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: wyswietlacz_spi.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 wyswietlacz_spi.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) wyswietlacz_spi.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
