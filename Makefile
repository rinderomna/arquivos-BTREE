objects = *.c
debug:
	cp antes/* .
	gcc $(objects) -o bin -g -Wall -Werror 
	./bin <tests/17.in && cat tests/17.out

all: $(objects)
	gcc $(objects) -o bin -g -Wall -Werror 
run:
	./bin
