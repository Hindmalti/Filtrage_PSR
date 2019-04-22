TARGET=lanceServeurs
FLAGS=-Wall -Wextra -I ../libs
LIBS=-lpcap -lnet -lthrd -lpthread -L ../libs

all: compile

clean:
	rm -f *.o *.out

exec: compile
	./$(TARGET).out

compile:
	gcc *.c -o $(TARGET).out $(LIBS) $(FLAGS)
