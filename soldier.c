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

void soldier_tell(int sock){
    char buff[MAX];
    int n;
    bzero(buff, sizeof(buff));
    printf("Telling server this is a soldier.\n");
    strcpy(buff, "soldier\0");
    write(sock, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    printf("From Server: %s\n", buff);
}

void func(int sock){
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sock, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("From Server: %s\n", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {                  //fix exit
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(){
    int sock, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // tell server this is a player
    soldier_tell(sock);

    // function for chat
    func(sock);

    // close the socket
    close(sock);
}
