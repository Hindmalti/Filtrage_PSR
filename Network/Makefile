TARGET=libnet
CFLAGS=-Wall -pedantic -Wextra -I ../libs
CLIB=ar cq

all: $(TARGET).a

clean:
	rm -f *.o *.a
	rm -f ../libs/$(TARGET).o
	rm -f ../libs/$(TARGET).h

$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET).a: $(TARGET).o
	rm -rf $@
	$(CLIB) $@ $+
	cp $(TARGET).a ../libs/$(TARGET).a
	cp $(TARGET).h ../libs/$(TARGET).h