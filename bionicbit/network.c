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
    printf("connect complate [peer].           success \n");
    return s;
}

// send framed message: 4-byte BE length + payload
int send_msg(int s, const void *payload, uint32_t len){
    uint32_t be = htonl(len);
    if(write(s, &be, 4)!=4) return -1;
    if(len>0) if(write(s, payload, len)!=(ssize_t)len) return -1;
    printf("send_msg complate [%u].           success \n", len);
    return 0;
}

// read framed message into dynamically allocated buffer (caller frees), returns len or -1
int recv_msg(int s, void **out){
    uint32_t be; ssize_t r = read(s, &be, 4);
    if(r==0) return 0; if(r!=4) return -1;
    uint32_t len = ntohl(be);
    void *buf = NULL;
    if(len>0){ buf = malloc(len); size_t got=0; while(got < len){ ssize_t n = read(s, (char*)buf+got, len-got); if(n<=0){ free(buf); return -1;} got+=n; } }
    printf("send_msg complate [%u].           success \n", len);
    *out = buf; return (int)len;
}
