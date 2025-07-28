all:
	gcc -Wall -Wextra -Wconversion -g -o clex main.c cvecs.c -I./ -lm
