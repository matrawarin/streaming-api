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

    printf("$[%u] [%s]\n", len, cmd );
    if(strcmp(cmd, "getheight")==0){
        // load chain length
        void *chain; size_t sz = load_chain(&chain);
        size_t hdrs = sz / 80;
        char resp[64]; snprintf(resp, sizeof(resp), "%zu", hdrs);
        send_msg(s, resp, strlen(resp));
        free(chain);
        printf("chain complate [getheight].     success\n");
    } else if(strncmp(cmd, "submitblock ", 12)==0){
        // hex decode 80 bytes
        const char *hex = cmd+12; if(strlen(hex) < 160){ send_msg(s, "err", 3); }
        else{
            unsigned char hdr[80]; for(int i=0;i<80;i++){ sscanf(hex+2*i, "%2hhx", &hdr[i]); }
            append_block_header(hdr, 80);
            send_msg(s, "ok", 2);
        }
        printf("chain complate [submitblock].     success\n");
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
