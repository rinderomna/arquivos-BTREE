objects = *.c

all: $(objects)
	gcc $(objects) -o bin
run:
	./bin
