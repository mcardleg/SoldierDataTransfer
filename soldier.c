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

void randomData(char* output);
void setupString(char* output);
const int id = 12345;

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
		randomData(buff);				//random values
		
        write(sock, buff, sizeof(buff));//wite to router
        printf("Data being sent: %s\n", buff);
        
        bzero(buff, sizeof(buff));		//clear buffer
        read(sock, buff, sizeof(buff));	//read from router
        printf("From Server: %s\n", buff);
        
        //if empty string sent from router, transmission over, exit
        if(strcmp(buff,"")==0){
        	printf("Mission Complete, Exiting...\n");
        	exit(-1);
        }
        
        delay(1000);
    }
}

int main(){
    int sock;
    struct sockaddr_in servaddr;

    // socket create and verification
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else printf("Socket successfully created.\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the router failed.\n");
        exit(0);
    }
    else
        printf("Connected to the router.\n");

    soldier_tell(sock);	// tell server this is a soldier
    io(sock);			// function for transmission and retrieval
    close(sock);		// close the socket
}

void randomData(char* output) {
	//generate random values for soldier 

	int heart, impact;
	
	heart = rand() % 100 + 100; 	//heartrate between 100 and 200BPM
	impact = rand() % 21;			//impact rating between 0 and 20
	
	//format output string
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
