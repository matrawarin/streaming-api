CC = gcc
CFLAGS = -O2 -Wall -std=c11 -I.
LDFLAGS = -lcrypto

PICFLAGS = -fPIC
SOFLAGS  = -shared

# Your library sources
LIBSRC = crypto.c network.c db.c
LIBOBJ = $(LIBSRC:.c=.o)

all: libxcoin.so chain node miner wallet

# --- Build shared library -------------------------------------

libxcoin.so: $(LIBOBJ)
	$(CC) $(SOFLAGS) -o $@ $(LIBOBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(PICFLAGS) -c $< -o $@

# --- Build executables ----------------------------------------
chain: chain.o
	$(CC) $(CFLAGS) -o $@ chain.o -L. -lxcoin $(LDFLAGS)
	
node: node.o
	$(CC) $(CFLAGS) -o $@ node.o -L. -lxcoin $(LDFLAGS)

miner: miner.o
	$(CC) $(CFLAGS) -o $@ miner.o -L. -lxcoin $(LDFLAGS)

wallet: wallet.o
	$(CC) $(CFLAGS) -o $@ wallet.o -L. -lxcoin $(LDFLAGS)

# --- Cleanup ---------------------------------------------------

clean:
	rm -f *.o *.so node miner wallet
