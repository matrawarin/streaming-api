// compile: gcc chat_server.c -o chat_server -pthread
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9000
#define MAX_CLIENTS 100
#define BUF_SIZE 1024

typedef struct {
    int sock;
    char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_to_all(char *msg, int exclude_sock) {
    pthread_mutex_lock(&clients_mutex);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            if(clients[i]->sock != exclude_sock){
                send(clients[i]->sock, msg, strlen(msg), 0);
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

client_t* find_by_name(const char* name) {
    client_t *res = NULL;
    pthread_mutex_lock(&clients_mutex);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i] && strcmp(clients[i]->name, name)==0){
            res = clients[i];
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return res;
}

void add_client(client_t *cl) {
    pthread_mutex_lock(&clients_mutex);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(!clients[i]){ clients[i]=cl; break; }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int sock) {
    pthread_mutex_lock(&clients_mutex);
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            if(clients[i]->sock == sock){
                free(clients[i]);
                clients[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    client_t *cli = (client_t*)arg;
    char buff[BUF_SIZE];
    // announce
    char welcome[128];
    sprintf(welcome, "%s has joined.\n", cli->name);
    send_to_all(welcome, cli->sock);

    while(1){
        int r = recv(cli->sock, buff, BUF_SIZE-1, 0);
        if(r <= 0) break;
        buff[r]=0;
        // trim newline
        while(r>0 && (buff[r-1]=='\n' || buff[r-1]=='\r')) { buff[--r]=0; }

        if(strlen(buff)==0) continue;

        // direct message syntax: @username message
        if(buff[0]=='@'){
            char target[32]; int i=1,j=0;
            while(buff[i] && buff[i] != ' ' && j < 31) target[j++] = buff[i++]; 
            target[j]=0;
            char *msg = buff + i + (buff[i]==' ' ? 0 : 0);
            client_t *to = find_by_name(target);
            if(to){
                char out[BUF_SIZE];
                snprintf(out, sizeof(out), "(private) %s: %s\n", cli->name, buff + i + (buff[i]==' ' ? 1 : 0));
                send(to->sock, out, strlen(out), 0);
            } else {
                char err[64]; snprintf(err, sizeof(err), "User %s not found.\n", target);
                send(cli->sock, err, strlen(err), 0);
            }
        } else {
            char out[BUF_SIZE];
            snprintf(out, sizeof(out), "%s: %s\n", cli->name, buff);
            send_to_all(out, cli->sock);
        }
    }

    // left
    char left[128];
    sprintf(left, "%s has left.\n", cli->name);
    send_to_all(left, cli->sock);
    close(cli->sock);
    remove_client(cli->sock);
    return NULL;
}

int main(){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; server_addr.sin_addr.s_addr = INADDR_ANY; server_addr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(listenfd, 10);
    printf("Chat server listening on %d\n", PORT);

    while(1){
        struct sockaddr_in cli_addr; socklen_t clilen = sizeof(cli_addr);
        int conn = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);
        // ask for name
        char name[32];
        send(conn, "Enter your name: ", 17, 0);
        int n = recv(conn, name, sizeof(name)-1, 0);
        if(n <= 0){ close(conn); continue; }
        name[n]=0; name[strcspn(name, "\r\n")] = 0;

        client_t *cli = malloc(sizeof(client_t));
        cli->sock = conn; strncpy(cli->name, name, sizeof(cli->name)-1);
        add_client(cli);

        pthread_t tid;
        pthread_create(&tid, NULL, &handle_client, (void*)cli);
        pthread_detach(tid);
    }
    close(listenfd);
    return 0;
}

