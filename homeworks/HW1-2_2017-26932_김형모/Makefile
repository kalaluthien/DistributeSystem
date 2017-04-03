all:
	gcc -o test -O1 test.c -pthread

ser:
	gcc -o test -O1 test.c -D SER

asm:
	gcc -o test.s -O1 -S test.c -pthread -D MT
