###############################################################################
# Makefile for the project asroa
###############################################################################

## General Flags
PROJECT = asroa
MCU = atmega8535
TARGET = asroa.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                        -DF_CPU=3686400UL -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=asroa.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\FickleBJT\Projects\AsRoA\src\include" 

## Objects that must be built in order to link
OBJECTS = adc.o asroa.o buttons.o IK_calc.o LED.o PWM_gen.o integration.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) asroa.hex asroa.eep asroa.lss size

## Compile
adc.o: ../adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

asroa.o: ../asroa.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

buttons.o: ../buttons.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

IK_calc.o: ../IK_calc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LED.o: ../LED.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PWM_gen.o: ../PWM_gen.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

integration.o: ../integration.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) asroa.elf dep/* asroa.hex asroa.eep asroa.lss asroa.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

