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

    int rl2 = 0;

    //int tries=1;
    //while(tries){    
    for(uint64_t tries=0;;tries++){
        uint32_t n = (uint32_t)rand(); memcpy(hdr+76, &n, 4);
        double_sha256(hdr, 80, hash);
        // simple target: first 2 bytes zero
        if(hash[0]==0 && hash[1]==0){
            printf("found nonce: %u\n", n); print_hex(hash,32); printf("\n");
            // submit
            printf("node win64: %d ,%u\n", rl2, s);
            char hex[161];
            char msg[200];
            
            for(int i=0;i<80;i++){ sprintf(hex+2*i, "%02x", hdr[i]); hex[160]=0; }
            snprintf(msg, sizeof(msg), "submitblock %s", hex);
            send_msg(s, msg, strlen(msg)); 
            void *resp; 
            printf("data: %s \n", msg);
            while((rl2=recv_msg(s, &resp))>1){
                printf("node resp: %u \n", rl2);
                 if(rl2>0){ 
                    printf("node response: %.*s\n", rl2, (char*)resp); 
                    free(resp);
                } 
            }
            //break;
        }
        if((tries & 0xFFFFFF)==0) printf("tries=%llu\r", (unsigned long long)tries);
    }
    close(s);
    return 0;
}

