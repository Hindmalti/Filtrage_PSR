TARGET=lanceServeurs
FLAGS=-Wall -Wextra -I ../libs
LIBS=-lpcap -lnet -lthrd -lpthread -L ../libs

all: compile

debug: FLAGS += -DDEBUG
debug: compile

clean:
	rm -f *.o *.out
	rm -f ../bin/$(TARGET).out

exec: compile
	./$(TARGET).out

compile:
	gcc *.c -o $(TARGET).out $(LIBS) $(FLAGS)
	mkdir -p ../bin
	cp $(TARGET).out ../bin/
