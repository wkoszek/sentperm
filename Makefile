all: sentperm

sentperm: sentperm.c
	$(CC) -Wall -std=c99 sentperm.c -o sentperm

clean:
	rm -rf sentperm
