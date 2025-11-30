// compile: gcc chat_client.c -o chat_client
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024

int sock;

void *recv_thread(void *arg){
    char buff[BUF_SIZE];
    while(1){
        int r = recv(sock, buff, BUF_SIZE-1, 0);
        if(r <= 0) break;
        buff[r]=0; printf("%s", buff);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc < 3){ printf("Usage: %s <server_ip> <port>\n", argv[0]); return 1; }
    char *ip = argv[1]; int port = atoi(argv[2]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv; serv.sin_family = AF_INET; serv.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv.sin_addr);
    if(connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0){ perror("connect"); return 1; }

    pthread_t tid; pthread_create(&tid, NULL, recv_thread, NULL);

    // first receive prompt "Enter your name:" then send name
    char buff[BUF_SIZE];
    int r = recv(sock, buff, BUF_SIZE-1, 0);
    if(r>0){ buff[r]=0; printf("%s", buff); }
    fgets(buff, sizeof(buff), stdin); send(sock, buff, strlen(buff), 0);

    while(fgets(buff, sizeof(buff), stdin)){
        send(sock, buff, strlen(buff), 0);
    }
    close(sock);
    return 0;
}

