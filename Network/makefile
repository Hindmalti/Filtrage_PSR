TARGET=libnet
FLAGS=-Wall -pedantic -Wextra -I ../libs
LIBS=-lthrd -L ../libs

all: compile

clean:
	rm -f *.o *.a

compile:
	gcc $(TARGET).c -c $(FLAGS) $(LIBS)
	ar cq $(TARGET).a *.o
	cp $(TARGET).a ../libs/$(TARGET).a
	cp $(TARGET).h ../libs/$(TARGET).h
	make clean
