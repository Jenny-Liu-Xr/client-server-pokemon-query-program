#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "PPS.h"

// Insert node into linked list
void InsertByTail(list *mylist,Pokemon posData)
{
	Node *newNode = CreateNode(posData); // create a node
	Node *pMove = mylist->headNode;
	if (mylist->headNode == NULL) // Determine whether there is node data at the head of the linked list
	{
		mylist->headNode = newNode; // Let the first node be inserted into the linked list so that the head of the linked list points to the first node
		mylist->size++; // record the number of nodes
		return;
	}
	while (pMove->next != NULL) // if next node is null
	{
		pMove = pMove->next; // move pMove to the next
	}
	pMove->next = newNode; // Insert the node at the end of the linked list
	newNode->next=NULL;
	mylist->size++; // record the number of nodes
}
int Get_Num(char* str)
{
	int len = strlen(str);
	int num = 0;
	for (int i = 0; i < len; i++)
	{
		num += str[i] - '0'; // Convert and add the current position
		if(len-i!=1) // Judge the length, if it reaches the last digit, the value in num does not have to move to the high digit
		num *= 10;
	}
	return num;
}
void Read_CSV(list *mylist,char * Type) // read pokemon file
{
	char *line = NULL, *record = NULL; //Define two pointers, a pointer to store the data of the row, and a pointer to the data of the delimiter key
	char Str[20000] = " "; 
	Pokemon temp_pet = { 0 }; //Initialize variables
	fseek(fp, 88L, SEEK_SET); // Position to the second row
	while ((line = fgets(Str, sizeof(Str), fp)) != NULL) // when the end of file is not read, the loop continues
	{
		record = strtok(line, ","); // Data between comma separators
		temp_pet.num = Get_Num(record); // save the current data to the corresponding attribute member, the same operation as below
		record = strtok(NULL, ",");
		strcpy(temp_pet.name, record);
		record = strtok(NULL, ",");
		strcpy(temp_pet.Type_1, record);
		if (strcmp(temp_pet.Type_1, Type))
			continue;
		record = strtok(NULL, ",");
		if (record[1] >= '0'&&record[1] <= '9') // check the current data type
		{
			strcpy(temp_pet.Type_2, "     ");
		}
		else
		{
			strcpy(temp_pet.Type_2, record);
			record = strtok(NULL, ",");
		}
		temp_pet.Total = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.HP = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Attack = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Defense = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Sp_Atk = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Sp_Def = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Speed = Get_Num(record);
		record = strtok(NULL, ",");
		temp_pet.Generation = Get_Num(record);
		record = strtok(NULL, ",");
		if (record != NULL)
		{
			int len = strlen(record);
			record[len - 1] = '\0';
		}
		strcpy(temp_pet.Legendary, record);
		memset(Str, 0, 20000);
		InsertByTail(mylist, temp_pet); // insert data into the linked list
	}
	fclose(fp);
}

int main(int argc, char **argv)
{
	mylist = CreateList();
	//printf("Please enter the file name including suffix: \n");
	//scanf("%s", TextName);

	
	fp=fopen("./pokemon.csv","r");
	
	while(fp == NULL)
	{
		printf("Please enter the file name (including suffix): \n");
		scanf("%s", TextName);
		fp=fopen(TextName,"r");
	}

	// Step 1: Create a socket on the server side.
	int listenfd;
	if ( (listenfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("Failed to create socket.\n");
		
		return -1;
	}

	// Step 2: Bind the address and port used by the server for communication to the socket.
	struct sockaddr_in servaddr;    // The data structure of the server address information.
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;  // The protocol family can only be AF_INET in socket programming.
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // Any ip address
	
	servaddr.sin_port = htons(8080);  // Specify the port
	printf("==============***");
	
	int reuse = 1;
	
	if (-1 == setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) {
		printf("setsockopt errorn");
		return 1;
	}
	
	if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
	{
		perror("bind");
		printf("Failed to bind socket.\n");
		close(listenfd);
		return -1;
	}
	
	// Step 3: Set the socket to monitor mode.
	if (listen(listenfd,5) != 0 )
	{
		//perror("listen");
		printf("Failed to monitor socket.\n");
		close(listenfd);
		return -1;
	}

	// Step 4: Accept the client's connection.
	int  clientfd;                  // client socket
	int  socklen=sizeof(struct sockaddr_in); // struct the size of sockaddr_in
	struct sockaddr_in clientaddr;  // The address information of the client.
	clientfd=accept(listenfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
	
	// Step 5: Communicate with the client, and reply ok after receiving the message sent by the client.
	char buffer[1024];
	while (1)
	{
		int iret;
		memset(buffer,0,sizeof(buffer));
		if ( (iret=recv(clientfd,buffer,sizeof(buffer),0))<=0) // Receive the client's request message.
		{
		   printf("Failed to received! \niret=%d\n",iret);
		   //break;
		}
		if(strcmp(buffer,TextName)==0)
		{
			strcpy(buffer,"file.ok");
			if((iret=send(clientfd,buffer,strlen(buffer),0))<=0)
			{
				printf("Failed to send!");
				//break;
			}
			memset(buffer,0,sizeof(buffer));
			if ( (iret=recv(clientfd,buffer,sizeof(buffer),0))<=0) // Receive the client's request message.
			{
				printf("Failed to received \niret=%d\n",iret);
				//break;
			}
			if(strcmp(buffer,"1")==0)
			{
				memset(buffer,0,sizeof(buffer));
				if ( (iret=recv(clientfd,buffer,sizeof(buffer),0))<=0) // Receive the client's request message.
				{
					printf("Failed to received \niret=%d\n",iret);
					//break;
				}
				Read_CSV(mylist, buffer); 

				//printf("%d\n",mylist->size);

				char str[1024];
				sprintf(str,"%d",mylist->size);

				if((iret=send(clientfd,str,strlen(str),0))<=0)
				{
					printf("Failed to send.");
					//break;
				}
				sleep(1);
				memset(str, 0, sizeof(str));
				Node *pMove = mylist->headNode; // define a pointer to the head of the linked list
				while (pMove) // check if pMove is null
				{
					sprintf(str, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", pMove->data.num,
						pMove->data.name, pMove->data.Type_1, pMove->data.Type_2,
						pMove->data.Total, pMove->data.HP, pMove->data.Attack, pMove->data.Defense,
						pMove->data.Sp_Atk, pMove->data.Sp_Def, pMove->data.Speed, pMove->data.Generation, pMove->data.Legendary); // Format conversion and store in the Str array
					if((iret=send(clientfd,str,strlen(str),0))<=0)
					{
						printf("Failed to send!");
						break;
					}
					memset(str, 0, sizeof(str));
					pMove = pMove->next; // move pointer to next
					sleep(0.1);
				}
			}
		}else{
			strcpy(buffer,"file.notok");
			if((iret=send(clientfd,buffer,strlen(buffer),0))<=0)
			{
				printf("Failed to send!");
				//break;
			}
		}
	}
	
	// Step 6: Close the socket and release resources.
	close(listenfd);
	close(clientfd);

	return 0;
}



