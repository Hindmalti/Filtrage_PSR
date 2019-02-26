TARGET=ARPCountTest
LIBS=-lpcap -lpthread

all: compile

clean:
	rm -f *.o *.out

exec: compile
	./$(TARGET).out

compile:
	gcc -Wall $(TARGET).c -o $(TARGET).out $(LIBS)
