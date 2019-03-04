TARGET=LibthrdTest
FLAGS= -Wall -pedantic -Wextra -I ../libs
LIBS=-lpcap -lnet -lthrd -lpthread -L ../libs

all: compile

clean:
	rm -f *.o *.out

exec: compile
	./$(TARGET).out

compile:
	gcc $(TARGET).c -o $(TARGET).out $(LIBS) $(FLAGS)

kate:
	kate makefile $(TARGET).c &
