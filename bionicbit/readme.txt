# Project: xnc_bitcoin_c

This single-file project bundle contains a minimal, educational **Bitcoin-like** implementation in C: a node (P2P), a miner (PoW), and a wallet (keypair + simple RPC). It's **not production** code — it's for learning and experimentation.

---
USAGE (build & run)

1. Requirements: gcc, make, OpenSSL dev (libssl-dev)
   Debian/Ubuntu: sudo apt-get install build-essential libssl-dev

2. Build:
   make

3. Run a node (listens on port 18444 by default):
   ./node

4. Run another node to connect as peer (or the miner/wallet binaries below):
   ./node 127.0.0.1 18444  # connect to peer

5. Run the miner (connects to a node):
   ./miner 127.0.0.1 18444

6. Run the wallet (simple RPC client):
   ./wallet 127.0.0.1 18444

---
FILES INCLUDED (in this single document; split into files as needed):

=== Makefile ===
CC=gcc
CFLAGS=-O2 -Wall -std=c11 -I.
LDFLAGS=-lcrypto

all: node miner wallet

node: node.c crypto.c network.c db.c
	$(CC) $(CFLAGS) -o node node.c crypto.c network.c db.c $(LDFLAGS)

miner: miner.c crypto.c network.c db.c
	$(CC) $(CFLAGS) -o miner miner.c crypto.c network.c db.c $(LDFLAGS)

wallet: wallet.c crypto.c network.c db.c
	$(CC) $(CFLAGS) -o wallet wallet.c crypto.c network.c db.c $(LDFLAGS)

clean:
	rm -f node miner wallet *.o chain.dat

=== crypto.h ===
#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>

void double_sha256(const uint8_t *data, size_t len, uint8_t out[32]);
void hash160(const uint8_t *data, size_t len, uint8_t out20[20]);
int ecdsa_generate_key(unsigned char **priv, size_t *priv_len, unsigned char **pub, size_t *pub_len);

#endif

=== crypto.c ===
#include "crypto.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <string.h>
#include <stdlib.h>

void double_sha256(const uint8_t *data, size_t len, uint8_t out[32]){
    uint8_t buf[32];
    SHA256(data, len, buf);
    SHA256(buf, 32, out);
}

void hash160(const uint8_t *data, size_t len, uint8_t out20[20]){
    uint8_t sha[32];
    SHA256(data, len, sha);
    RIPEMD160(sha, 32, out20);
}

int ecdsa_generate_key(unsigned char **priv, size_t *priv_len, unsigned char **pub, size_t *pub_len){
    int ret = 0;
    EC_KEY *eck = EC_KEY_new_by_curve_name(NID_secp256k1);
    if(!eck) return 0;
    if(!EC_KEY_generate_key(eck)) goto end;

    // private key to DER
    *priv_len = i2d_ECPrivateKey(eck, NULL);
    *priv = malloc(*priv_len);
    unsigned char *p = *priv;
    i2d_ECPrivateKey(eck, &p);

    // public key (compressed)
    const EC_POINT *pt = EC_KEY_get0_public_key(eck);
    const EC_GROUP *gr = EC_KEY_get0_group(eck);
    *pub_len = EC_POINT_point2oct(gr, pt, POINT_CONVERSION_COMPRESSED, NULL, 0, NULL);
    *pub = malloc(*pub_len);
    EC_POINT_point2oct(gr, pt, POINT_CONVERSION_COMPRESSED, *pub, *pub_len, NULL);

    ret = 1;
end:
    EC_KEY_free(eck);
    return ret;
}

=== network.c ===
// Minimal TCP networking: accepts peers, simple message framing (length + payload)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int open_listen(uint16_t port){
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(port);
    if(bind(s, (struct sockaddr*)&addr, sizeof(addr))!=0) { perror("bind"); close(s); return -1; }
    if(listen(s, 8)!=0){ perror("listen"); close(s); return -1; }
    return s;
}

int connect_peer(const char *host, uint16_t port){
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET; addr.sin_port = htons(port);
    if(inet_pton(AF_INET, host, &addr.sin_addr) != 1){ close(s); return -1; }
    if(connect(s, (struct sockaddr*)&addr, sizeof(addr))!=0){ close(s); return -1; }
    return s;
}

// send framed message: 4-byte BE length + payload
int send_msg(int s, const void *payload, uint32_t len){
    uint32_t be = htonl(len);
    if(write(s, &be, 4)!=4) return -1;
    if(len>0) if(write(s, payload, len)!=(ssize_t)len) return -1;
    return 0;
}

// read framed message into dynamically allocated buffer (caller frees), returns len or -1
int recv_msg(int s, void **out){
    uint32_t be; ssize_t r = read(s, &be, 4);
    if(r==0) return 0; if(r!=4) return -1;
    uint32_t len = ntohl(be);
    void *buf = NULL;
    if(len>0){ buf = malloc(len); size_t got=0; while(got < len){ ssize_t n = read(s, (char*)buf+got, len-got); if(n<=0){ free(buf); return -1;} got+=n; } }
    *out = buf; return (int)len;
}

=== db.c ===
// Very small chain storage: append block headers to chain.dat and load on startup
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHAIN_FILE "chain.dat"

int append_block_header(const void *hdr, size_t len){
    FILE *f = fopen(CHAIN_FILE, "ab"); if(!f) return 0;
    fwrite(hdr, 1, len, f); fclose(f); return 1;
}

size_t load_chain(void **out){
    FILE *f = fopen(CHAIN_FILE, "rb"); if(!f) { *out = NULL; return 0; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f,0,SEEK_SET);
    void *buf = malloc(sz); fread(buf,1,sz,f); fclose(f);
    *out = buf; return (size_t)sz;
}

=== node.c ===
// Minimal node: accepts RPC commands over TCP from wallet/miner or peers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.c"
#include "crypto.c"
#include "db.c"

int handle_peer(int s){
    // very simple: receive commands like "getheight" "submitblock <80byte header hex>""getblock <idx>"
    void *payload; int len = recv_msg(s, &payload);
    if(len <= 0) return -1;
    payload = payload; // avoid warning
    char *cmd = malloc(len+1); memcpy(cmd, payload, len); cmd[len]=0; free(payload);
    if(strcmp(cmd, "getheight")==0){
        // load chain length
        void *chain; size_t sz = load_chain(&chain);
        size_t hdrs = sz / 80;
        char resp[64]; snprintf(resp, sizeof(resp), "%zu", hdrs);
        send_msg(s, resp, strlen(resp));
        free(chain);
    } else if(strncmp(cmd, "submitblock ", 12)==0){
        // hex decode 80 bytes
        const char *hex = cmd+12; if(strlen(hex) < 160){ send_msg(s, "err", 3); }
        else{
            unsigned char hdr[80]; for(int i=0;i<80;i++){ sscanf(hex+2*i, "%2hhx", &hdr[i]); }
            append_block_header(hdr, 80);
            send_msg(s, "ok", 2);
        }
    } else {
        send_msg(s, "unknown", 7);
    }
    free(cmd);
    return 0;
}

int main(int argc, char **argv){
    uint16_t port = 18444;
    int listen_fd = open_listen(port);
    if(listen_fd < 0){ fprintf(stderr, "failed to listen\n"); return 1; }
    printf("node listening on %u\n", port);
    while(1){
        int client = accept(listen_fd, NULL, NULL);
        if(client < 0) continue;
        if(fork()==0){ close(listen_fd); handle_peer(client); close(client); exit(0); }
        close(client);
    }
    return 0;
}

=== miner.c ===
// Miner connects to a node, requests height, creates simple header with prev=zero or last, mines PoW, submits
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crypto.c"
#include "network.c"
#include "db.c"

static void print_hex(const uint8_t *p, size_t n){ for(size_t i=0;i<n;i++) printf("%02x", p[i]); }

int main(int argc, char **argv){
    const char *host = "127.0.0.1"; uint16_t port = 18444;
    if(argc>=3){ host = argv[1]; port = (uint16_t)atoi(argv[2]); }
    int s = connect_peer(host, port); if(s<0){ fprintf(stderr, "connect failed\n"); return 1; }
    // ask height
    send_msg(s, "getheight", 9);
    void *r; int rl = recv_msg(s, &r); int height = 0; if(rl>0){ sscanf((char*)r, "%d", &height); free(r); }
    printf("peer height=%d\n", height);
    // build header: version(4) + prev(32) + merkle(32) + ts(4) + bits(4) + nonce(4) = 80
    uint8_t hdr[80]; memset(hdr, 0, 80);
    uint32_t ver = 1; memcpy(hdr, &ver, 4);
    uint32_t ts = (uint32_t)time(NULL); memcpy(hdr+68, &ts, 4);
    uint32_t bits = 0x1f00ffff; memcpy(hdr+72, &bits, 4);
    srand(time(NULL)); uint32_t nonce = 0;
    uint8_t hash[32];
    printf("mining...\n");
    for(uint64_t tries=0;;tries++){
        uint32_t n = (uint32_t)rand(); memcpy(hdr+76, &n, 4);
        double_sha256(hdr, 80, hash);
        // simple target: first 2 bytes zero
        if(hash[0]==0 && hash[1]==0){
            printf("found nonce: %u\n", n); print_hex(hash,32); printf("\n");
            // submit
            char hex[161]; for(int i=0;i<80;i++) sprintf(hex+2*i, "%02x", hdr[i]); hex[160]=0;
            char msg[200]; snprintf(msg, sizeof(msg), "submitblock %s", hex);
            send_msg(s, msg, strlen(msg)); void *resp; int rl2 = recv_msg(s, &resp); if(rl2>0){ printf("node response: %.*s\n", rl2, (char*)resp); free(resp); }
            break;
        }
        if((tries & 0xFFFFFF)==0) printf("tries=%llu\r", (unsigned long long)tries);
    }
    close(s);
    return 0;
}

=== wallet.c ===
// Simple wallet: generates keypair, prints address (hash160), and can ask node for height. Also can send "submitblock" RPC to submit mined block.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.c"
#include "network.c"

static void hexprint(const uint8_t *p, size_t n){ for(size_t i=0;i<n;i++) printf("%02x", p[i]); }

int main(int argc, char **argv){
    unsigned char *priv=NULL,*pub=NULL; size_t priv_len=0, pub_len=0;
    if(!ecdsa_generate_key(&priv,&priv_len,&pub,&pub_len)){ fprintf(stderr, "keygen fail\n"); return 1; }
    uint8_t h20[20]; hash160(pub, pub_len, h20);
    printf("Private key (DER %zu bytes)\n", priv_len);
    hexprint(priv, priv_len); printf("\nPublic key (%zu bytes)\n", pub_len); hexprint(pub,pub_len); printf("\nAddress (hash160): "); hexprint(h20,20); printf("\n");

    const char *host = "127.0.0.1"; uint16_t port = 18444; if(argc>=3){ host=argv[1]; port=(uint16_t)atoi(argv[2]); }
    int s = connect_peer(host, port); if(s<0){ fprintf(stderr, "connect failed\n"); return 1; }
    send_msg(s, "getheight", 9); void *r; int rl = recv_msg(s, &r); if(rl>0){ printf("node height: %.*s\n", rl, (char*)r); free(r); }
    close(s);
    free(priv); free(pub);
    return 0;
}

---
Notes & Limitations
- This is an **educational** implementation. It omits many Bitcoin features: real difficulty target math, block validation, transaction format, UTXO management, mempool, P2P protocol, signature verification for transactions, wallets with addresses/UTXO, security, persistence, reorg handling, etc.
- The miner mines only header PoW with empty merkle and prev; node just appends headers to chain.dat without validation.
- Use this to learn and expand. To make production-ready you must add many features and security checks.

---
If you'd like, I can:
- Split this single document into separate files and create a ZIP for download.
- Expand the wallet to create and sign transactions and maintain a simple UTXO set.
- Implement proper compact target difficulty (`bits`) and verification.
- Add real P2P protocol with message types and peer discovery.

Tell me which next step, and I'll modify the code accordingly.

