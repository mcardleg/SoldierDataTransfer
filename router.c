#include <stdio.h>
#include <string.h>         //strlen
#include <stdlib.h>
#include <errno.h>          //errno, perror()
#include <unistd.h>         //read(), close()
#include <sys/time.h>       //FD_SET, FD_ISSET, FD_ZERO, select()
#include <arpa/inet.h>      //inet_ntoa()
                                //includes sys/socket.h - SOCK_STREAM, setsockopt(), SO_REUSEADDR, AF_INET(), accept(), bind(), getpeername(), listen(), send(), socket()
                                //includes netinet/in.h - sin_family, sin_port, sin_addr, sockaddr_in, INNADDR_ANY, htons(), ntohs()
#define MAX 80
#define TRUE   1
#define FALSE  0
#define PORT 8080
#define NUM_SOLDIERS 10

//globals
int opt, addrlen, activity, valread;
int client_socket[NUM_SOLDIERS+1], soldier_check[NUM_SOLDIERS+1], i, base;
int master_socket, max_sock, new_socket, curr_sock;
struct sockaddr_in address;                                                     //struct which stores internet addresses
char buffer[1025];                                                              //data buffer of 1K
char soldier[30];                                                               //data buffer for passing on soldier's message
fd_set socket_set;                                                              //set of socket descriptors

int setup(){
    opt = TRUE;

    for (i = 0; i < NUM_SOLDIERS+1; i++){
        client_socket[i] = 0;                           //initialise all client_socket[] to 0 so not checked
        soldier_check[i] = 0;                           //initialize all sockets to not player.
    }

    //socket() creates a socket and returns a file descriptor that can be used in later function calls that operate on sockets
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){              //AF_INET  => IP4 address type, SOCK_STREAM => TCP socket type, 0 => unspecified default protocol
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //setsockopt() changes socket settings, SOL_SOCKET specifies that it's changing settings at the socket level, SO_REUSEADDR allows reuse of local addresses
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){   //int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;                       //AF_INET => IP4 address type
    address.sin_addr.s_addr = INADDR_ANY;               //INADDR_ANY allows connection to any active port
    address.sin_port = htons( PORT );                   //htons translates a short integer from host byte order to network byte order

    //bind() assigns an address to an unnamed socket
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){       //argument 1 - socket descriptor, argument 2 - struct holding the address to be bound
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    if (listen(master_socket, 3) < 0){                  //listen() changes the socket mode to accept connections, argument 2 sets the length of the socket's listen queue
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    return 1;
}

int socket_in_out(int i){
    curr_sock = client_socket[i];

    if (FD_ISSET(curr_sock, &socket_set)){              //Returns a non-zero value if the bit for curr_sock is set in socket_set, and 0 otherwise.

        //Read the message
        //If the length of the message is zero, the client has disconnected
        if ((valread = read( curr_sock, buffer, 1024)) == 0){
            getpeername(curr_sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);                                   //getpeername() stores the address & address length of curr_sock in a sockaddr struct
            printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));     //inet_ntoa() converts an internet host address into a string in internet standard dot notation
                                                                                                                        //ntohs() converts from network byte order to host byte order.
            close(curr_sock);                           //Close the socket and mark as 0 in list for reuse
            client_socket[i] = 0;                       //Mark as 0 in array for reuse
        }
        //If incoming message says "exit", echo it back.
        else if ((strncmp(buffer, "Q", 4)) == 0) {
            printf("Mission Complete, Exiting.\n");
            buffer[valread] = '\0';
            send(curr_sock, buffer, strlen(buffer), 0);
        }
        //Check if the socket is a soldier
        else if(strcmp(buffer, "soldier") == 0){
            strcpy(buffer, "ack soldier\0");
            soldier_check[i] = 1;                       //Track which sockets have soldiers
            send(curr_sock, buffer, strlen(buffer), 0 );
        }
        //Check if the socket is the base
        else if(strcmp(buffer, "base") == 0){
            strcpy(buffer, "ack base\0");
            base = client_socket[i];                    //Track which socket has the base
            send(curr_sock, buffer, strlen(buffer), 0 );
        }
        //Check if it's a message from a soldier
        else if (soldier_check[i] == 1){
        	buffer[valread] = '\0';
        	send(base, buffer, strlen(buffer), 0);
        	printf("%s\n", buffer);
        	strcpy(buffer, "forwarded\0");
        	send(curr_sock, buffer, strlen(buffer), 0);
        }
        //If it's not a soldier, its the base requesting data.
        else {
            strcpy(buffer, "ack request\0");
            send(curr_sock, buffer, strlen(buffer), 0);
        }
    }
    return 1;
}

int io(char *message){
    FD_ZERO(&socket_set);					//clear the socket set
    FD_SET(master_socket, &socket_set);    		//add master socket to set
    max_sock = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < NUM_SOLDIERS+1; i++){
        curr_sock = client_socket[i];
        if(curr_sock > 0) {FD_SET( curr_sock , &socket_set);} 	//if current socket descriptor is valid, add to read list
        if(curr_sock > max_sock) {max_sock = curr_sock;}	//highest file descriptor number, need it for the select function
    }

    //wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
    activity = select(max_sock+1, &socket_set, NULL, NULL, NULL);

    if ((activity < 0) && (errno!=EINTR)) {printf("select error");}

    //If something happened on the master socket, then its an incoming connection
    if (FD_ISSET(master_socket, &socket_set)){
        if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        	perror("accept");
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

    while(TRUE){io(message);}

    return 0;
}
