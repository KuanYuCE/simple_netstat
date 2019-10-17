all:hw1

run:hw1
	./hw1

hw1:main.c lib.c
	gcc main.c lib.c -o hw1
clean:
	rm hw1

