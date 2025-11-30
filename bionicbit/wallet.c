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
