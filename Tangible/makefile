MCU = atmega328p
FCPU = 16000000

CC = avr-gcc
LD = avr-gcc
FLAGS = -mmcu=$(MCU)
CFLAGS = -Wall $(FLAGS) -DF_CPU=$(FCPU) -Os
LDFLAGS = $(FLAGS)

DUDE = avrdude
PROTOCOL = stk500v1	# stk500 / stk500v1
SPEED = 115200		# 57600 / 115200
SERIAL = /dev/ttyACM0	# ttyUSB0 / ttyACM0

TARGET = tangibleInterface
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET).hex

debug: FLAGS += -DDEBUG -g
debug: $(TARGET).hex

clean:
	rm -f *.o $(TARGET).hex $(TARGET)
	rm -f ../bin/$(TARGET).hex

$(TARGET): $(OBJECTS)

$(TARGET).hex: $(TARGET)
	avr-objcopy -j .text -j .data -O ihex $(TARGET) $(TARGET).hex
	mkdir -p ../bin
	cp $(TARGET).hex ../bin/

upload: $(TARGET).hex
	stty -F $(SERIAL) hupcl
	$(DUDE) -F -v -p $(MCU) -c $(PROTOCOL) -b $(SPEED) -P $(SERIAL) -U flash:w:$(TARGET).hex

size: $(TARGET)
	avr-size --format=avr --mcu=$(MCU) $(TARGET)
