main: main.c arena.c array.c
	clang $< -Wall -Wextra -g -o main
