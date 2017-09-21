CROSS   ?=avr-
CC      :=$(CROSS)gcc
CXX     :=$(CROSS)g++
LD      :=$(CROSS)g++
SIZE    :=$(CROSS)size
OBJCOPY :=$(CROSS)objcopy
OBJDUMP :=$(CROSS)objdump

TARGET=a
MMCU?=attiny13a
AVRDUDE_MMCU?=t13
AVRDUDE_FLAGS?=-p $(AVRDUDE_MMCU) -c usbasp

SOURCES=$(wildcard *.c)
INCLUDES=

OBJECTS=$(SOURCES:.c=.o)

CSTD?=c99
COPT=-O1 -fdata-sections -ffunction-sections
CFLAGS=-mmcu=$(MMCU) -std=$(CSTD) $(COPT) -Wall
CFLAGS+=$(addprefix -I,$(INCLUDES))

LDFLAGS=-mmcu=$(MMCU) -Wl,--gc-sections -Wl,-Map=$(TARGET).map,--cref

all: $(TARGET).hex $(TARGET).lst $(TARGET).eep

$(TARGET).elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -lm -o $@
	$(SIZE) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).eep: $(TARGET).elf
	$(OBJCOPY) -O ihex -j .eeprom \
	  --set-section-flags=.eeprom="alloc,load" \
	  --change-section-lma .eeprom=0 $< $@

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -c

$(TARGET).lst: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

flash:
	avrdude $(AVRDUDE_FLAGS) -e -U flash:w:$(TARGET).hex

fuses:
	# default fuses: H:FF L:6A
	# CKDIV8 = 0
	avrdude $(AVRDUDE_FLAGS) -U hfuse:w:0xff:m -U lfuse:w:0x7a:m

eeprom:
	avrdude $(AVRDUDE_FLAGS) -U eeprom:w:$(TARGET).eep

clean:
	rm *.o
