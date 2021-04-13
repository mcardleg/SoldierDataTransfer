#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             //read(), close(), write()
#include <arpa/inet.h>          //inet_addr()
                                    //includes sys/socket.h - SOCK_STREAM, AF_INET(), socket(), connect()
                                    //includes netinet/in.h - sin_family, sin_port, sin_addr, sockaddr_in, htons()
#include <sys/select.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int sock;
struct sockaddr_in servaddr;
FILE *fpt1;
FILE *fpt2;
FILE *fpt3;
FILE *fpt4;
FILE *fpt5;
FILE *fpt6;

char buf[0x100];

char identity1[6] = {0};
char identity2[6] = {0};
char identity3[6] = {0};
char identity4[6] = {0};
char identity5[6] = {0};
char identity6[6] = {0};
int counter = 0;

void setup(){
    //socket() creates a socket and returns a file descriptor that can be used in later function calls that operate on sockets
    sock = socket(AF_INET, SOCK_STREAM, 0);     //AF_INET  => IP4 address type, SOCK_STREAM => TCP socket type, 0 => unspecified default protocol
    if (sock == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created.\n");
        bzero(&servaddr, sizeof(servaddr));

    //sin_family, sin_addr and sin_port are part of the sockaddr_in struct
    servaddr.sin_family = AF_INET;              //AF_INET => IP4 address type
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");      //inet_addr() converts a string of IP4 address to an int for use as an internet address
    servaddr.sin_port = htons(PORT);            //htons translates a short integer from host byte order to network byte order

    // connect the client socket to server socket
    if (connect(sock, (SA*)&servaddr, sizeof(servaddr)) != 0) {         //int connect(int socket, const struct sockaddr *address, socklen_t address_len);
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
    char buff[MAX], game[MAX], ID[10], HEART[10], IMPACT[10];
    int n, x, y, id, heart, impact;

    fd_set fds;
    struct timeval tv = {.tv_sec=0,.tv_usec=0};
    printf("Press 'Q' in terminal to Exit\n");
  
    for (;;) {
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) {                  //fix exit
            printf("Base Exit.\n");
            break;
        }

        char ID[10];
        char HEART[10];
        char IMPACT[10];
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
        impact = atoi(IMPACT);	//turns impact string into int

        //fprintf(fpt,"%d, %d, %d\n", id, heart, impact);	//puts ID, heartrate and impact into csv file

        if(heart<=50 && heart!=0){
        	printf("Heartrate is too low for soldier %d: %d\n", id, heart);
        }

        if(impact>=14){			//if soldier has impact above 14Gs sends message
        	printf("Large impact force for soldier %d: %d\n", id, impact);
        }
        printf("From router buff: %s \n", buff);
        
        
        if(strlen(identity1)==0 && (strncmp(ID,identity2,5))!=0 && (strncmp(ID,identity3,5))!=0 && (strncmp(ID,identity4,5))!=0 && (strncmp(ID,identity5,5))!=0 && (strncmp(ID,identity6,5))!=0){
        	strcpy(identity1, ID);
        	printf("%s.csv should be created\n", identity1);
        	snprintf(buf, sizeof(buf), "%s.csv", identity1);
        	fpt1 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt1 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt1,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if(strlen(identity2)==0 && (strncmp(ID,identity1,5))!=0 && (strncmp(ID,identity3,5))!=0 && (strncmp(ID,identity4,5))!=0 && (strncmp(ID,identity5,5))!=0 && (strncmp(ID,identity6,5))!=0){
		strcpy(identity2, ID);
		printf("%s.csv should be created\n",identity2);
		snprintf(buf, sizeof(buf), "%s.csv", identity2);
        	fpt2 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt2 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt2,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if(strlen(identity3)==0 && (strncmp(ID,identity2,5))!=0 && (strncmp(ID,identity1,5))!=0 && (strncmp(ID,identity4,5))!=0 && (strncmp(ID,identity5,5))!=0 && (strncmp(ID,identity6,5))!=0){
        	strcpy(identity3, ID);
        	printf("%s.csv should be created\n", identity3);
        	snprintf(buf, sizeof(buf), "%s.csv", identity3);
        	fpt3 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt3 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt3,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if(strlen(identity4)==0 && (strncmp(ID,identity2,5))!=0 && (strncmp(ID,identity3,5))!=0 && (strncmp(ID,identity1,5))!=0 && (strncmp(ID,identity5,5))!=0 && (strncmp(ID,identity6,5))!=0){
        	strcpy(identity4, ID);
        	printf("%s.csv should be created\n", identity4);
        	snprintf(buf, sizeof(buf), "%s.csv", identity4);
        	fpt4 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt4 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt4,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if(strlen(identity5)==0 && (strncmp(ID,identity2,5))!=0 && (strncmp(ID,identity3,5))!=0 && (strncmp(ID,identity4,5))!=0 && (strncmp(ID,identity1,5))!=0 && (strncmp(ID,identity6,5))!=0){
        	strcpy(identity5, ID);
        	printf("%s.csv should be created\n", identity5);
        	snprintf(buf, sizeof(buf), "%s.csv", identity5);
        	fpt5 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt5 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt5,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if(strlen(identity6)==0 && (strncmp(ID,identity2,5))!=0 && (strncmp(ID,identity3,5))!=0 && (strncmp(ID,identity4,5))!=0 && (strncmp(ID,identity5,5))!=0 && (strncmp(ID,identity1,5))!=0){
        	strcpy(identity6, ID);
        	printf("%s.csv should be created\n", identity6);
        	snprintf(buf, sizeof(buf), "%s.csv", identity6);
        	fpt6 = fopen(buf, "w");				//opens or creates a CSV file to store data

    		if(fpt6 == NULL){					//if it doesnt open sends error
      			printf("Error!");
      			exit(1);
    		}	

    		fprintf(fpt6,"ID, Heartrate, Impact\n");		//puts headers in  CSV file
        }
        
        if((strncmp(ID,identity1,5))==0){
        	fprintf(fpt1,"%d, %d, %d\n", id, heart, impact);
        }
        else if((strncmp(ID,identity2,5))==0){
        	fprintf(fpt2,"%d, %d, %d\n", id, heart, impact);
        }
        else if((strncmp(ID,identity3,5))==0){
        	fprintf(fpt3,"%d, %d, %d\n", id, heart, impact);
        }
        else if((strncmp(ID,identity4,5))==0){
        	fprintf(fpt4,"%d, %d, %d\n", id, heart, impact);
        }
        else if((strncmp(ID,identity5,5))==0){
        	fprintf(fpt5,"%d, %d, %d\n", id, heart, impact);
        }
        else if((strncmp(ID,identity6,5))==0){
        	fprintf(fpt6,"%d, %d, %d\n", id, heart, impact);
        }

	FD_ZERO(&fds);      //Initializes all file descriptors in the set to have zero bits
	FD_SET(0,&fds);     //Sets the bit for the file descriptor fd in the file descriptor set fdset.

	//if there is an input
	if(select(1,&fds,NULL,NULL,&tv)){       //select() function indicates which of the specified file descriptors is ready for reading, ready for writing,
                                            //or has an error condition pending. If the specified condition is false for all of the specified file descriptors,
                                            //select() blocks, up to the specified timeout interval, until the specified condition is true for at least one of
                                            //the specified file descriptors or until a signal arrives that needs to be delivered.
		scanf("%s", game);	//read in input
        	if (strcmp(game, "Q") == 0) {	//if 'Q', exit
			printf("Mission Complete, Exiting...\n");
			write(sock, game, sizeof(game));
			break;
		}
	}

    }
}

int main(){

    setup();
    base_tell(sock);    				// tell server this is a coach
    io(sock);						// function for transmission and retrieval
    if(strlen(identity1)!=0){
    	fclose(fpt1);
    }
    
    if(strlen(identity2)!=0){
    	fclose(fpt2);
    }
    
    if(strlen(identity3)!=0){
    	fclose(fpt3);
    }
    
    if(strlen(identity4)!=0){
    	fclose(fpt4);
    }
    
    if(strlen(identity5)!=0){
    	fclose(fpt5);
    }
    
    if(strlen(identity6)!=0){
    	fclose(fpt6);
    }

    return 0;
}














