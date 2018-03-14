CC = gcc
TRG = hw1

CFLAGS = -O1
LDFLAGS = -lpthread

all:
	$(CC) -o $(TRG) $(TRG).c $(CFLAGS) $(LDFLAGS)

ser:
	$(CC) -o $(TRG) $(TRG).c -D SER $(CFLAGS)

asm:
	$(CC) -o $(TRG).s -S $(TRG).c $(CFLAGS) $(LDFLAGS)

run: $(TRG)
	./$(TRG)

clean:
	rm -rf $(TRG) $(TRG).s
