TARGET=

all: compile

clean:
	rm -f *.o *.out *.a

exec: compile
	./$(TARGET).out

compile:
	gcc $(TARGET).c -lpthread -o $(TARGET).out
