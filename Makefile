all: sentperm

sentperm: sentperm.c
	$(CC) -Wall -std=c99 sentperm.c -o sentperm

tests:	sentperm
	./sentperm < test/1.i > 1.o
	./sentperm < test/2.i > 2.o
	./sentperm < test/3.i > 3.o
	./sentperm < test/4.i > 4.o

check:
	@echo "# Check (no output is ok)"
	diff -u test/1.t 1.o
	diff -u test/2.t 2.o
	diff -u test/3.t 3.o
	diff -u test/4.t 4.o

clean:
	rm -rf sentperm *.o
