#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "PQC.h"

// Insert node into linked list
void InsertByTail(list *mylist,Pokemon posData)
{
	Node *newNode = CreateNode(posData); // create a node
	Node *pMove = mylist->headNode; // define a pointer to the head of the linked list
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

void Save_Data(list *mylist,char* str)
{
	fp = fopen(str, "a");
	char Str[2000] = " ";
	Node *pMove = mylist->headNode; // define a pointer to the head of the linked list
	fprintf(fp,"#,name,Type_1,Type_2,Total,HP,Attack,Defense,Sp.Atk,Sp.Def,Speed,Generation,Legendary\n"); // save the form header into the first line of the file
	while (pMove) // check if pMove is null
	{
		sprintf(Str, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", pMove->data.num,
			pMove->data.name, pMove->data.Type_1, pMove->data.Type_2,
			pMove->data.Total, pMove->data.HP, pMove->data.Attack, pMove->data.Defense,
			pMove->data.Sp_Atk, pMove->data.Sp_Def, pMove->data.Speed, pMove->data.Generation, pMove->data.Legendary); // Format conversion and store in the Str array
		fwrite(Str, 1, strlen(Str), fp); // Write this line of data to the file
		memset(Str, 0, 2000);
		pMove = pMove->next; // move pointer 
	}
	fclose(fp); 
}

// clear
void list_Clear(list* mylist)
{
	Node* pMove = NULL;
	Node* Front = NULL;
loop:pMove = mylist->headNode;
	if (pMove == NULL)
	{
		return;
	}
	if (pMove->next == NULL)
	{
		mylist->headNode = NULL;
		mylist->size--;
		free(pMove);
		return;
	}
	while (pMove->next)
	{
		Front = pMove;
		pMove = Front->next;
	}
	Front->next = NULL;
	mylist->size--;
	free(pMove);
	goto loop;
}


void Meun()
{
	
	printf("*******【Pokemon Query Program】*******\n");
	printf("1-Type Search\n");
	printf("2-Save results\n");
	printf("3-Exit the query program\n");
	printf("*******【Pokemon Query Program】*******\n");
	printf("Please choose 1~3\n");
}

int main(int argc, char **argv)
{
	list* mylist = CreateList();
	
	// Step 1: Create a socket for the client.
	int sockfd;
	if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) 
	{ 
		printf("Failed to create socket.\n"); 
		return -1; 
	}
	 
	// Step 2: Initiate a connection request to the server.
	struct hostent* h;
	if ( (h = gethostbyname("127.0.0.1")) == 0 )   // Specify the ip address of the server.
	{ 
		printf("Failed to get host by name.\n"); 
		close(sockfd); 
		return -1; 
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080); // Specify the port of the server
	memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
	if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  //Initiate a connection request to the server.
	{ 
		printf("Cannot connect with PPS!\n");
		close(sockfd);
		return -1; 
	}
	
	printf("Please enter the file name (including suffix): \n");
	scanf("%s", TextName);
	
	
	char buffer[1024];
	char temp[50]="";
	int num;
	
	while(1){
		int iret;
		memset(buffer,0,sizeof(buffer));
		
		strcpy(buffer,TextName);
		
		if ( (iret=send(sockfd,buffer,strlen(buffer),0))<=0) // Send a request message to the server
		{
			 printf("Failed to send!");
			 break; 
		}
		
		memset(buffer,0,sizeof(buffer));
		if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0) // Receive the response message from the server
		{
			printf("Failed to received! \niret=%d\n",iret);
			break;
		}
		printf("%s\n", buffer);
		if(strcmp(buffer,"file.ok")==0)
		{
			while(1)
			{
			//system("clear");
			Meun();
			int keys = 0;
			scanf("%d", &keys);
			switch (keys)
			{
			case 1: //Query in the file by type and read into the memory
			{
				memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"%d",keys);
				if ( (iret=send(sockfd,buffer,sizeof(buffer),0))<=0) // Receive the response message from the server
				{
					printf("Failed to received! \niret=%d\n",iret);
					break;
				}
				
				char temp[50];
				printf("Please enter the type of Pokemon\n");
				scanf("%s", temp);
				if ( (iret=send(sockfd,temp,strlen(temp),0))<=0) // Send a request message to the server
				{
					 printf("Failed to send!");
					 break; 
				}
				memset(buffer,0,sizeof(buffer));
				if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0) // Receive the response message from the server
				{
					printf("Failed to received! \niret=%d\n",iret);
					break;
				}
				num=atoi(buffer);
				//printf("%d\n",num);
				for(int i=0;i<num;i++)
				{
					//printf("%d\n",num);
					if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0) // Receive the response message from the server
					{
						
						break;
					}
					//printf("%s",buffer);
					Pokemon temp_pet;
					char *record = NULL;
					record = strtok(buffer, ","); // Data between comma separators
					temp_pet.num = Get_Num(record); // save the current data to the corresponding attribute member, the same operation as below
					record = strtok(NULL, ",");
					strcpy(temp_pet.name, record);
					record = strtok(NULL, ",");
					strcpy(temp_pet.Type_1, record);
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
						record[len-1] = '\0';
					}
					strcpy(temp_pet.Legendary, record);
					
					printf("%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", temp_pet.num,
						temp_pet.name, temp_pet.Type_1, temp_pet.Type_2,
						temp_pet.Total, temp_pet.HP, temp_pet.Attack, temp_pet.Defense,
						temp_pet.Sp_Atk, temp_pet.Sp_Def, temp_pet.Speed, temp_pet.Generation, temp_pet.Legendary);
					memset(buffer, 0, 1024);
					InsertByTail(mylist, temp_pet); // insert data into the linked list
				}
				break;
			}
			case 2:
			{
				printf("Please enter a new file name: \n");
				scanf("%s", temp);
				while (temp == NULL) // Determine whether the file name has been enteredDetermine whether the file name has been entered
				{
					printf("Unable to create the new file. Please enter the name of the file again.");
					printf("Please enter a new file name: \n");
					scanf("%s", temp);
				}
				Save_Data(mylist, temp); // Save the queried type-related Pokémon into a custom file
				list_Clear(mylist);
				
				sleep(1);
				break;
			}
			case 3:
				printf("You have already checked %d types of Pokemon. \n",num);
				if(strcmp(temp,"")!=0)
				{
					printf("You have created new files: %s. \n",temp);
				}else{
					printf("You did not create any new files. \n");
				}
				exit(0); // exit the program 
				break;
			default:
				printf("Please choose 1-3 again: \n"); //Menu invalid input, re-enter
				break;
			}
		}
		}else{
			printf("Cannot find the file. Please enter again.\n");
			printf("PLease enter the file name(including suffix)\n");
			scanf("%s", TextName);
		}
	}
	
	return 0;
}

