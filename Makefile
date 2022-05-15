objects = *.c

all: $(objects)
	gcc $(objects) -o bin -Wall -Werror
run:
	./bin
