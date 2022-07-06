objects = *.c

all: $(objects)
	gcc $(objects) -o bin -g -Wall -Werror 
run:
	./bin
restore:
	cp antes/* binarios/
