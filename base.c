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

<<<<<<< Updated upstream
int sock, connfd;
struct sockaddr_in servaddr, cli;
FILE *fpt;
    
=======
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

>>>>>>> Stashed changes
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
    for (;;) {
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
<<<<<<< Updated upstream
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
        
        if ((strncmp(buff, "exit", 4)) == 0) {	//fix exit
            printf("Client Exit...\n");
            break;
        }
=======
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
	
>>>>>>> Stashed changes
    }
}

int main(){

<<<<<<< Updated upstream
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
=======

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
>>>>>>> Stashed changes

    return 0;
}

<<<<<<< Updated upstream














=======
>>>>>>> Stashed changes
