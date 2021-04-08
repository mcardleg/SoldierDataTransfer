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

int sock, connfd;
struct sockaddr_in servaddr, cli;
FILE *fpt;
    
void setup(){
    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
        bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with server failed...\n");
        exit(0);
    }
    else
        printf("connected to server\n");
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

/*
int func(int socket) {          //send requests for data periodically
    char buff[MAX];
    bzero(buff, sizeof(buff));
    read(socket, buff, sizeof(buff));
    printf("From Pi: %s\n", buff);
    if ((strncmp(buff, "exit", 4)) == 0) {
        return 1;
    }
    return 0;
}	*/

void func(int sock){
    char buff[MAX];
    int n;
    printf("Press 'Q' in terminal to Exit\n");
    for (;;) {
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        char ID[5];
        char HEART[3];
        char IMPACT[2];
        int x=0;
        int y=0;
        int id, heart, impact;
        
        while(buff[y] != ','){	//takes in 5 digit ID
        	ID[x]=buff[y];
        	x++;
        	y++;
        }
        x=0;
        y++;
        
        while(buff[y] != ','){	//takes in heartrate 
        	HEART[x]=buff[y];
        	x++;
        	y++;
        }
        x=0;
        y++;
        
        while(buff[y] != '\0'){	//takes in impact
        	IMPACT[x]=buff[y];
        	x++;
        	y++;
        }
        
        id = atoi(ID);			//turns id string into int
        heart = atoi(HEART);		//turns heartrate string into int
        impact = atoi(IMPACT);	//turns impact string into int
        
        fprintf(fpt,"%d, %d, %d\n", id, heart, impact);	//puts ID, heartrate and impact 									  into csv file
        
        if(heart<=50){			//if heartrate is less then 50 send alert message
        	printf("Heartrate is too low for soldier %d: %d", id, heart);
        }
        
        if(impact>=20){		//if soldier has impact above 20Gs sends message
        	printf("Large impact force for soldier %d: %d", id, impact);
        }
        
        printf("From router: %s\n", buff);	//prints out id, heartrate and imapct to base 						  terminal
        
	scanf("%s", buff);
        if (strcmp(buff, "Q") == 0) {
		printf("Mission Complete, Exiting...\n");
		write(sock, "Q", sizeof(buff));
		break;
	}
    }
}

int main(){

    fpt = fopen("SoldierData.csv", "w+");		//opens or creates a CSV file to store data
	
    if(fpt == NULL){					//if it doesnt open sends error
      	printf("Error!");   
      	exit(1);             
    }
    
    fprintf(fpt,"ID, Heartrate, Impact\n");		//puts headers in  CSV file

	
    setup();
    // tell server this is a coach
    base_tell(sock);

    // function for transmission and retrieval
    func(sock);
    
    
    fclose(fpt);

    return 0;
}















