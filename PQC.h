
// FILE PQC.h

#ifndef HEADER_PQC
#define HEADER_PQC

/************************************************/
// structures

char TextName[50]; // Define a file name variable, used to enter the file name name
FILE* fp;

typedef struct //pokemon structure attributes
{
	int num;
	char name[50];
	char Type_1[20];
	char Type_2[20];
	int Total;
	int HP;
	int Attack;
	int Defense;
	int Sp_Atk;
	int Sp_Def;
	int Speed;
	int Generation;
	char Legendary[10];
}Pokemon;

typedef struct node // structure Node
{
	Pokemon data;
	struct node *next;
}Node;

typedef struct List // structure List
{
	Node *headNode;
	int size;
}list;

list * CreateList() // Allocates memory for a new list and initializes it 
{
	list* mylist = (list*)malloc(sizeof(list));
	mylist->headNode = NULL; // pointer initialization
	mylist->size = 0; // parameter initialization
	return mylist;
}

Node *CreateNode(Pokemon posData) // Allocates memory for a new list Node
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->data = posData; // store data in the node field
	newNode->next = NULL; // Initialize the pointer
	return newNode;
}

// Insert node into linked list
void InsertByTail(list *mylist,Pokemon posData);

int Get_Num(char* str);

// save pokemon data
void Save_Data(list *mylist,char* str);

// clear
void list_Clear(list* mylist);

void Meun();
//void Keyon(list *mylist);

#endif
