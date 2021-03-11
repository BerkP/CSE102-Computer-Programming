#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TRUE 1
#define FALSE 0

struct person{
	char name[20];
	char surname[20];
	char musical_Work[20];
	int age;
	struct person *next;
}*top;

void addNode(char name[], char surname [], char creation [], int age);
void deleteNode();
void Sort_Alphabetically();
void Sort_Increasingly();
void swap_nodes(struct person *node_1 , struct person *node_2);
void print_stack();
int print_menu();



int main(){
	char name[20],surname[20],creation[20];
	int age,exit_status=FALSE,choice;

	while(exit_status == FALSE){
		choice=print_menu();
		if(choice == 1){
			printf("\nName: ");
			scanf(" %[^\n]s",name);
			printf("\nSurame: ");
			scanf(" %s",surname);
			printf("\nCreation: ");
			scanf(" %[^\n]s",creation);
			printf("\nAge: ");
			scanf(" %d",&age);
			addNode(name,surname,creation,age);
			print_stack();
		}
		else if(choice == 2){
			deleteNode();
			print_stack();
		}
		else if(choice == 3){
			Sort_Alphabetically();
			print_stack();
		} 
		else if(choice == 4){
			Sort_Increasingly();
			print_stack();
		}
		else if(choice == 5){
			exit_status = TRUE;
			free(top);
		}
	}
	return 0;
}

void addNode(char name[], char surname [], char creation [], int age){
	struct person *new_node=(struct person*)malloc(sizeof(struct person));
	strcpy(new_node->name,name);
	strcpy(new_node->surname,surname);
	strcpy(new_node->musical_Work,creation);
	new_node->age=age;
	new_node->next=top;
	top=new_node;	
}

void  deleteNode(){
	struct person *freep;
	if(top != NULL){
		freep=top;
		top=top->next;
		freep->next=NULL;
		free(freep);
	}
	else{
		printf("\n***LIST EMPTY!***\n");
	}
}

void Sort_Alphabetically(){
	struct person *current = top,*current_2,*temp_1,*temp_2;
	while(current->next != NULL){
		current_2 = current->next;
		while(current_2 != NULL){
			if(strcmp(current->name,current_2->name) > 0){
				temp_1=current;
				temp_2=current_2;
				swap_nodes(current,current_2);
				current=temp_2;
				current_2=temp_1;
			}
			current_2 = current_2->next;
		}
		current = current->next;
	} 
} 

void Sort_Increasingly(){
	struct person *current = top,*current_2,*temp_1,*temp_2;
	while(current->next != NULL){
		current_2 = current->next;
		while(current_2 != NULL){
			if(current->age > current_2->age){
				temp_1=current;
				temp_2=current_2;
				swap_nodes(current,current_2);
				current=temp_2;
				current_2=temp_1;
			}
			current_2 = current_2->next;
		}
		current = current->next;
	} 
}

void swap_nodes(struct person *node_1 , struct person *node_2){
	struct person *prev_1=top,*prev_2=top,*next_1,*next_2;
	if(top != node_1){
		while(prev_1->next != node_1)	prev_1=prev_1->next;
	}
	else prev_1=NULL;
	if(node_1->next != node_2){
		while(prev_2->next != node_2)	prev_2=prev_2->next;		
		next_1=node_1->next;
	}
	else {
		prev_2=NULL;
		next_1=NULL;
	}
	if(node_2->next != NULL) 	next_2=node_2->next;
	else next_2=NULL;
	
	node_1->next = node_2->next = NULL;
	if(prev_1 !=NULL)	prev_1->next = node_2;
	else top=node_2;
	if (next_1 != NULL) node_2->next = next_1;
	else node_2->next = node_1;
	if(prev_2 != NULL)	prev_2->next = node_1;
	if(next_2 != NULL)  node_1->next = next_2;
	else node_1->next =NULL;
}

void print_stack(){
	struct person *current;
	int counter=1;
	current=top;
	printf("-----------------");
	while(current != NULL){
		printf("\n%d)%s\n%s\n%s\n%d\n",counter,current->name,current->surname,current->musical_Work,current->age);
		current=current->next;
		counter++;
	}	
}

int print_menu(){
	int choice;
	printf("\n****MENU****\n1- Add a Person to the Stack\n2- Pop a Person from the Stack\n3- Sort in Alphabetical Order\n");
	printf("4- Sort Age in Increasing Order\n5- Exit menu\n*************\nSelect your Choice: ");
	do{
		scanf("%d",&choice);
		if(choice < 0 || choice > 5 ) 	printf("Invalid choice!\nChoice:");
	}while (choice < 0 || choice > 5);
	return choice;
} 
