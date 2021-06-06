CC=gcc
CFLAGS= -g -fPIC -Wall -Werror -fsanitize=address -fno-omit-frame-pointer
platformer: main.c
	${CC} ${CFLAGS} main.c -o platformer && ./platformer
clean:
	rm platformer