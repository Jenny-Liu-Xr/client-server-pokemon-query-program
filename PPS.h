
// FILE PPS.h

#ifndef HEADER_PPS
#define HEADER_PPS

/************************************************/
// structures

FILE *fp=NULL;
//char TextName[50];
char TextName[] = "pokemon.csv";
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

list* mylist=NULL;

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

// read pokemon file
void Read_CSV(list *mylist,char * Type);

#endif
