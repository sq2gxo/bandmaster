###############################################################################
# Makefile for the project BandMaster
###############################################################################

## General Flags
PROJECT = BandMaster
MCU = atmega32
TARGET = BandMaster.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                       -DF_CPU=4000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=BandMaster.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = bandmaster.o lcd.o bm_config_data.o bm_init.o bm_timer.o bm_auto_band.o bm_display.o bm_messages.o bm_output.o bm_serial.o str_utils.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) BandMaster.hex BandMaster.eep BandMaster.lss size

## Compile
bandmaster.o: ../bandmaster.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd.o: ../lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_config_data.o: ../bm_config_data.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_init.o: ../bm_init.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_timer.o: ../bm_timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_auto_band.o: ../bm_auto_band.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_display.o: ../bm_display.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_messages.o: ../bm_messages.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_output.o: ../bm_output.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bm_serial.o: ../bm_serial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

str_utils.o: ../str_utils.c
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
	-rm -rf $(OBJECTS) BandMaster.elf dep/* BandMaster.hex BandMaster.eep BandMaster.lss BandMaster.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

