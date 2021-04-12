#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>      //inet_addr()
                                //includes sys/socket.h - SOCK_STREAM, AF_INET(), socket()
                                //includes netinet/in.h - sin_family, sin_port, sin_addr, sockaddr_in, htons()
#include <unistd.h>         //read(), close(), write()
#include <time.h>           //clock_t, clock()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void randomData(char* output);
void setupString(char* output);
int id;

void delay(int seconds){
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

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

void io(int sock){
    char buff[MAX];
    int n;
    for (;;) {
    	bzero(buff, sizeof(buff));
	setupString(buff);				//setup output
	randomData(buff);         //random values
        write(sock, buff, sizeof(buff));
        printf("Data being sent: %s\n", buff);
        /*bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("From Server: %s\n", buff);*/
        delay(200);
      
        //if empty string sent from router, transmission over, exit
        if(strcmp(buff,"")==0){
        	printf("Mission Complete, Exiting...\n");
        	exit(-1);
        }
    }
}

int main(){
    int sock;
    struct sockaddr_in servaddr;        //struct which stores internet addresses

    //socket() creates a socket and returns a file descriptor that can be used in later function calls that operate on sockets
    sock = socket(AF_INET, SOCK_STREAM, 0);     //AF_INET  => IP4 address type, SOCK_STREAM => TCP socket type, 0 => unspecified default protocol
    if (sock == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else printf("Socket successfully created.\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;              //AF_INET => IP4 address type
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");     //inet_addr() converts a string of IP4 address to an int for use as an internet address
    servaddr.sin_port = htons(PORT);           //htons translates a short integer from host byte order to network byte order

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {         //int connect(int socket, const struct sockaddr *address, socklen_t address_len);
        printf("Connection with the router failed.\n");
        exit(0);
    }
    else
        printf("Connected to the router.\n");

    printf("Please enter the soldier's five digit ID number.\n");
    scanf("%d",&id);
    soldier_tell(sock);		// tell server this is a soldier
    io(sock);			// function for transmission and retrieval
    close(sock);		// close the socket
}

void randomData(char* output) {       //generate random values for soldier

	int heart, impact;
	heart = rand() % 100 + 100; 	//heartrate between 100 and 200BPM
	impact = rand() % 21;			//impact rating between 0 and 20

	output[6] = '0' + heart / 100;
	output[7] = '0' + (heart % 100) / 10;
	output[8] = '0' + heart % 10;
	output[9] = ',';
	output[10] = '0' + impact / 10;
	output[11] = '0' + impact % 10;
	output[12] = ';';
}

void setupString(char* output) {
	//initialise output with soldier ID
	output[0] = '0' + id / 10000;
	output[1] = '0' + (id % 10000) / 1000;
	output[2] = '0' + (id % 1000) / 100;
	output[3] = '0' + (id % 100) / 10;
	output[4] = '0' + (id % 10);
	output[5] = ',';
}
