#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int sock;
struct sockaddr_in servaddr;

void setup(){
    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created.\n");
        bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with server failed.\n");
        exit(0);
    }
    else
        printf("Connected to server.\n");
}

void base_tell(int sock){
    char buff[MAX];
    int n;
    bzero(buff, sizeof(buff));
    printf("Telling server this is the base.\n");
    strcpy(buff, "base\0");
    write(sock, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    printf("From router: %s", buff);
}

void io(int sock){
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("From router: %s\n", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {                  //fix exit
            printf("Base Exit.\n");
            break;
        }
    }
}

int main(){

    setup();
    // tell server this is a coach
    base_tell(sock);

    // function for transmission and retrieval
    io(sock);

    return 0;
}
