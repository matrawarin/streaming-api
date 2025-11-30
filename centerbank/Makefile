CC=gcc
CFLAGS=-O2 -Wall -std=c11
LIBS=-lsqlite3 -lcrypto

OBJ=main.o db.o ledger.o account.o transaction.o audit.o crypto.o

all: bank

bank: $(OBJ)
	$(CC) $(CFLAGS) -o bank $(OBJ) $(LIBS)

clean:
	rm *.o bank

