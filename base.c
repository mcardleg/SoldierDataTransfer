#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
int sock;
struct sockaddr_in servaddr;
FILE *fpt;
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
    char game[MAX];
    int n;

    fd_set fds;
    struct timeval tv = {.tv_sec=0,.tv_usec=0};

    printf("Press 'Q' in terminal to Exit\n");
    for (;;) {
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("From router: %s\n", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {                  //fix exit
            printf("Base Exit.\n");
            break;
        }
        char ID[10];
        char HEART[10];
        char IMPACT[10];
        int x=0;
        int y=0;
        int id, heart, impact;
        ID[0]=buff[0];
        ID[1]=buff[1];
        ID[2]=buff[2];
        ID[3]=buff[3];
        ID[4]=buff[4];
        HEART[0]=buff[6];
        HEART[1]=buff[7];
        HEART[2]=buff[8];
        IMPACT[0]=buff[10];
        IMPACT[1]=buff[11];
        id = atoi(ID);			//turns id string into int
        heart = atoi(HEART);		//turns heartrate string into int
        impact = atoi(IMPACT);		//turns impact string into int
        fprintf(fpt,"%d, %d, %d\n", id, heart, impact);	//puts ID, heartrate and impact into csv file
        if(heart<=50 && heart!=0){
        	printf("Heartrate is too low for soldier %d: %d\n", id, heart);
        }
        if(impact>=14){			//if soldier has impact above 20Gs sends message
        	printf("Large impact force for soldier %d: %d\n", id, impact);
        }
        printf("From router buff: %s \n", buff);
        printf("From router int: %d ,%d, %d \n", id, heart, impact);
        //printf("From router int: %d ,%d, %d \n", id, heart, impact);
        //printf("From router string: %s, %s, %s\n\n", ID, HEART, IMPACT);

	//scanf("%s", buff);
        if (strcmp(buff, "Q") == 0) {
		printf("Mission Complete, Exiting...\n");
		write(sock, "Q", sizeof(char));
		break;

	//terms for select function
	FD_ZERO(&fds);	
	FD_SET(0,&fds);

	//if there is an input 
	if(select(1,&fds,NULL,NULL,&tv)){
		scanf("%s", game);	//read in input
        	if (strcmp(game, "Q") == 0) {	//if 'Q', exit
			printf("Mission Complete, Exiting...\n");
			write(sock, game, sizeof(game));
			break;
		}	
	}

    }
}
}

int main(){
    fpt = fopen("SoldierData.csv", "w");		//opens or creates a CSV file to store data
    if(fpt == NULL){					//if it doesnt open sends error
      	printf("Error!");
      	exit(1);
    }
    fprintf(fpt,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
    setup();
    base_tell(sock);    				// tell server this is a coach
    io(sock);						// function for transmission and retrieval
    fclose(fpt);
    return 0;
}
