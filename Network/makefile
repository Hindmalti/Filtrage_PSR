TARGET=libnet

all: compile

clean:
	rm -f *.o *.a

compile:
	gcc $(TARGET).c -c
	ar cq $(TARGET).a *.o
