TARGET=libthrd
CFLAGS=-Wall -pedantic -Wextra
CLIB=ar cq

all: $(TARGET).a

clean:
	rm -f *.o *.a
	rm -f ../libs/$(TARGET).a
	rm -f ../libs/$(TARGET).h

$(TARGET).o: $(TARGET).c $(TARGET).h
	#Compilation du libthread.o à partir de libthrd.c et .h
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET).a: $(TARGET).o
	#Suppression libthrd.a
	rm -rf $@
	#Création libthrd.a à partir de libthrd.o
	$(CLIB) $@ $+
	#Copie dans le dossier libs
	cp $(TARGET).a ../libs/$(TARGET).a
	cp $(TARGET).h ../libs/$(TARGET).h
