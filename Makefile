objects = *.c
debug:
	cp antes/* .
	gcc $(objects) -o bin -g -Wall -Werror 
	./bin <tests/15.in

all: $(objects)
	gcc $(objects) -o bin -g -Wall -Werror 
run:
	./bin
