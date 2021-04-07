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
#define MAX 80
#define TRUE   1
#define FALSE  0
#define PORT 8080
#define NUM_SOLDIERS 10

//globals
int count;
int count0, count1, count2, count3, count4, count5;
int hb_array[1000][5];
int impact_array[1000][5];
int opt;
int master_socket, addrlen, new_socket, client_socket[NUM_SOLDIERS+1], max_clients, clients, soldier_check[NUM_SOLDIERS+1], activity, i, valread, curr_sock, base;
int max_sd;
struct sockaddr_in address;
char buffer[1025];  //data buffer of 1K
char player[30];
fd_set readfds;     //set of socket descriptors
int q;

void delay(int seconds)
{
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
}
/*
void storage(int i, int hr, int impact){
    switch(i-1) {
        case 0: hb_array[count0][i]=hr;
            impact_array[count0][i]=impact;
            count0++;
            break;
        case 1: hb_array[count1][i]=hr;
            impact_array[count1][i]=impact;
            count1++;
            break;
        case 2: hb_array[count2][i]=hr;
            impact_array[count2][i]=impact;
            count2++;
            break;
        case 3: hb_array[count3][i]=hr;
            impact_array[count3][i]=impact;
            count3++;
            break;
        case 4: hb_array[count4][i]=hr;
            impact_array[count4][i]=impact;
            count4++;
            break;
        case 5: hb_array[count5][i]=hr;
            impact_array[count5][i]=impact;
            count5++;
            break;
    }
}	*/

int setup(){
    opt = TRUE;
    max_clients = 7;
    clients = 0;
    count0 = 0, count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;
    q=0;
    count=0;

    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;              //initialise all client_socket[] to 0 so not checked
        soldier_check[i] = 0;               //initialize all sockets to not player.
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    return 1;
}

int socket_in_out(int i){
    curr_sock = client_socket[i];

    if (FD_ISSET( curr_sock, &readfds))
    {
        //Check if it was for closing , and also read the
        //incoming message
        if ((valread = read( curr_sock, buffer, 1024)) == 0)
        {
            //Somebody disconnected , get his details and print
            getpeername(curr_sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //Close the socket and mark as 0 in list for reuse
            close( curr_sock );
            client_socket[i] = 0;
        }
        //If incoming message says "exit", echo it back.
        else if ((strncmp(buffer, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            buffer[valread] = '\0';
            send(curr_sock , buffer , strlen(buffer) , 0 );
            q--;
            if(q=0){
            send(base, buffer, strlen(buffer), 0);
            }
        }
        //Check if the socket is a player
        else if(strcmp(buffer, "soldier") == 0)
        {
            strcpy(buffer, "ack soldier\0");
            soldier_check[i] = 1;                    //track player sockets
            send(curr_sock, buffer, strlen(buffer), 0 );
            q++;
        }
        //Check if the socket is a coach
        else if(strcmp(buffer, "base") == 0)
        {
            strcpy(buffer, "ack base\0");
            base = client_socket[i];
            send(curr_sock, buffer, strlen(buffer), 0 );
        }
        //Check if it's a message from a player
        else if(soldier_check[i] == 1)
        {

            char str1[15];
            char str2[15];
            int x =0;
            int y =4;
            sprintf(player, "%d", i);		//DIFFERENT FOR EACH PI
            player[1]=' ';
            player[2] = ':';
            player[3] = ' ';
            buffer[valread] = '\0';
            while(buffer[x]!='\0'){
                player[y]=buffer[x];
                x++;
                y++;
            }
            str1[0]=player[4];
            str1[1]=player[5];
            str1[2]=player[6];
            str2[0]=player[8];
            if(player[8]!='\0'){
            str2[1]=player[9];
            }
            else{player[9]=' ';}
            player[y++] = '\0';

            int a = atoi(str1);
            int b = atoi(str2);

            if(a>=180){
            count++;
            }
            else{
            count=0;
            }

            if(count>5){
            send(base, player, strlen(player), 0 );
            }

            if(b>=12){
            send(base, player, strlen(player), 0 );
            }

            strcpy(buffer, "forwarded\0");
            send(curr_sock, buffer, strlen(buffer), 0 );
            //storage(i, a, b);
        }
        //If it's not a player, its a coach requesting data.
        else
        {
            strcpy(buffer, "ack request\0");
            send(curr_sock, buffer, strlen(buffer), 0 );
        }
    }
    return 1;
}



int comms(char *message){
    int e;          //for error handling
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < max_clients ; i++){
        curr_sock = client_socket[i];

        //if current socket descriptor is valid, add to read list
        if(curr_sock > 0)
            FD_SET( curr_sock , &readfds);

        //highest file descriptor number, need it for the select function
        if(curr_sock > max_sd)
            max_sd = curr_sock;
        }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

    if ((activity < 0) && (errno!=EINTR))
        printf("select error");
    
    //If something happened on the master socket, then its an incoming connection
    if (FD_ISSET(master_socket, &readfds))
    {
        if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
        	perror("accept");
        	exit(EXIT_FAILURE);
        }

        //inform user of socket number - used in send and receive commands
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //send new connection greeting message
        if( send(new_socket, message, strlen(message), 0) != strlen(message) )
        {
            perror("send");
        }

        puts("Welcome message sent successfully");

        //add new socket to array of sockets
        for (i = 0; i < max_clients; i++)
        {
            //if position is empty
            if( client_socket[i] == 0 )
            {
                client_socket[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
            }
        }
    }

    //else its some IO operation on some other socket
    for (i = 0; i < max_clients; i++)
    {
        e = socket_in_out(i);
        if(!e){
        //DO SOME ERROR HANDLING HERE
        }
        e = 0;          //reset
    }
    delay(250);
    return 1;
}

int main(int argc , char *argv[])
{
    //Message that is sent to new connections.
    char *message = "Connected to router.\r\n";         //maybe add number for which pi
    int e = 0;          //for error checking

    e = setup();
    if (!e){
    // DO SOME ERROR HANDLING HERE
    }
    e = 0;          //reset

    while(TRUE)
    {
        e = comms(message);
        if (!e){
        //DO SOME ERROR HANDLING HERE
        }
        e = 0;      //reset
    }

    return 0;
}
