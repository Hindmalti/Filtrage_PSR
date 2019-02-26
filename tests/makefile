TARGET=UDPTest
LIBS=-lpcap -lnet

all: compile

clean:
	rm -f *.o *.out

exec: compile
	./$(TARGET).out

compile:
	gcc -Wall $(TARGET).c -o $(TARGET).out $(LIBS)

kate:
	kate makefile $(TARGET).c &
