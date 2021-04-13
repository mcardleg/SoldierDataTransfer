#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include<time.h>
#include<stdbool.h>
#define MAX 80
#define TRUE   1
#define FALSE  0
#define PORT 8080
#define NUM_SOLDIERS 10

//globals
int opt, addrlen, activity, valread;
int client_socket[NUM_SOLDIERS+1], soldier_check[NUM_SOLDIERS+1], i, base;
int master_socket, max_sock, new_socket, curr_sock;
struct sockaddr_in address;
char buffer[1025];  //data buffer of 1K
char soldier[30];
char temp[30];
fd_set readfds;     //set of socket descriptors
int timing = 5;
clock_t time_last;
clock_t timeout_start;
int count = 0;
char ID[6][5];
char tempID[30];
bool reconnect = FALSE;
float time1[6];

int setup(){
    opt = TRUE;
    for (i = 0; i < NUM_SOLDIERS+1; i++){
        client_socket[i] = 0;              //initialise all client_socket[] to 0 so not checked
        soldier_check[i] = 0;              //initialize all sockets to not player.
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8080
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    return 1;
}

void socket_in_out(int i){
    curr_sock = client_socket[i];

    if (FD_ISSET( curr_sock, &readfds)){
        //Check if it was for closing, and also read the incoming message
        if ((valread = read( curr_sock, buffer, 1024)) == 0)
        {
            if(time1[i]/CLOCKS_PER_SEC + timing < clock()/CLOCKS_PER_SEC && reconnect == FALSE) {   
                //Somebody disconnected , get his details and print
                getpeername(curr_sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                ID[i][5] = 0;
                printf("Soldier %s timed out , ip %s , port %d \n", ID[i], inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                //Close the socket and mark as 0 in list for reuse
                close( curr_sock );
                client_socket[i] = 0;
            }
            /*else if(reconnect == TRUE) {
                close( curr_sock ); 
                client_socket[i] = 0;
                reconnect = FALSE;
            }*/
            else {
                read( curr_sock, buffer, 1024);
            }
        }
       //If incoming message says "exit", echo it back.
       else if ((strncmp(buffer, "Q", 4)) == 0) {
            printf("Mission Complete, Exiting.\n");
            exit(-1);
        }
        //Check if the socket is a soldier
        else if(strncmp(buffer, "Soldier", 7) == 0){
            strcpy(temp , buffer);
            sprintf(buffer, "ack %s", temp);
            printf("Connected to %s\n", temp);
            /*strncpy(tempID, &temp[8], 5);
            for(int i = 0; i <= count; i++) {
                if(strcmp(tempID, ID[i]) == 0) {
                    reconnect = TRUE;
                }
            }*/
            strncpy(ID[i], &temp[8], 5);
            printf("ID = %s\n", ID[i]);
            count++;
            soldier_check[i] = 1;                    //track soldier sockets
            send(curr_sock, buffer, strlen(buffer), 0 );
            time1[i] = clock();
        }
        //Check if the socket is the base
        else if(strcmp(buffer, "base") == 0){
            strcpy(buffer, "ack base\0");
            base = client_socket[i];
            send(curr_sock, buffer, strlen(buffer), 0 );
            time1[i] = clock();
        }
        //Check if it's a message from a soldier
        else if (soldier_check[i] == 1){
        	buffer[valread] = '\0';
        	send(base, buffer, strlen(buffer), 0);
        	//printf("%s\n", buffer);
        	strcpy(buffer, "forwarded\0");
        	send(curr_sock, buffer, strlen(buffer), 0);
            time1[i] = clock();
        }
        //If it's not a soldier, its the base requesting data.
        else {
            strcpy(buffer, "ack request\0");
            send(curr_sock, buffer, strlen(buffer), 0);
        }
    }
}

int io(char *message){
    FD_ZERO(&readfds);					//clear the socket set
    FD_SET(master_socket, &readfds);    		//add master socket to set
    max_sock = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < NUM_SOLDIERS+1; i++){
        curr_sock = client_socket[i];
        if(curr_sock > 0) {FD_SET( curr_sock , &readfds);} 	//if current socket descriptor is valid, add to read list
        if(curr_sock > max_sock) {max_sock = curr_sock;}	//highest file descriptor number, need it for the select function
    }

    //wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
    activity = select(max_sock+1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno!=EINTR)) {printf("select error");}

    //If something happened on the master socket, then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)){
        if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        	perror("accept");
            printf("hello\n");
        	exit(EXIT_FAILURE);
        }

        //inform user of socket number - used in send and receive commands
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        //send new connection greeting message
        if(send(new_socket, message, strlen(message), 0) != strlen(message)) {perror("send");}
        puts("Welcome message sent successfully");

        //add new socket to array of sockets
        for (i = 0; i < NUM_SOLDIERS+1; i++){

            //if position is empty
            if( client_socket[i] == 0 ){
                client_socket[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
            }
        }
    }

    //else its some IO operation on some other socket
    for (i = 0; i < NUM_SOLDIERS+1; i++){
        socket_in_out(i);
    }
    return 1;
}

int main(int argc , char *argv[]){
    //Message that is sent to new connections.
    char *message = "Connected to router.\r\n";
    setup();

    while(TRUE) {
        io(message);
    }
    printf("hello\n");
    return 0;
}