TARGET=lanceServeurs
INCLUDES=pagehtml.c
FLAGS=-Wall -Wextra -I ../libs
LIBS=-lpcap -lnet -lthrd -lpthread -L ../libs

all: compile

clean:
	rm -f *.o *.out

exec: compile
	./$(TARGET).out

compile:
	gcc *.c -o $(TARGET).out $(LIBS) $(FLAGS)

kate:
	kate makefile *.c &
