#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NUMBER_OF_DATAS 20

typedef struct node_s{
	int data;
	struct node_s *next,*prev;
}node_t;

typedef struct stack_s {
	struct node_s *top;
	int size;
}stack;

typedef struct queue_s {
	struct node_s *head,*tail;
	int size;
}queue;

typedef struct bst_node_s{
	int data;
	struct bst_node_s *left,*right;
}bst_node_t;

typedef struct bst_s {
	struct bst_node_s *root;
	int size;
}bst;

void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[20]);
void ordered_print(stack * stack_, queue * queue_, bst * bst_);
void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search);
void special_traverse(stack * stack_, queue * queue_, bst * bst_);
void swap_nodes(node_t *node_1 , node_t *node_2);
void bst_print(bst_node_t *root);
void bst_next_max_finder(bst_node_t *root , int *current_max ,int last_max);
void bst_next_min_finder(bst_node_t *root , int *current_min ,int last_min);

int main(){
	int data[20]={5, 2, 7, 8, 11, 3, 21, 7, 6, 15, 19, 35, 24, 1, 8, 12, 17, 8, 23, 4 };

	bst * bst_;
	queue * queue_;
	stack * stack_;
	fill_structures(&stack_, &queue_, &bst_, data);
	ordered_print(stack_, queue_, bst_);
	search(stack_, queue_, bst_, 5);
	special_traverse(stack_, queue_, bst_); 
	return 0;
}

void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[20]){
	int i ;
	double stack_time , queue_time , bst_time ; 
	clock_t start,stop;
	node_t *new_node = NULL;
	bst_node_t *new_bst_node = NULL , *current_bst;
	
	/*		Filling Stack 		*/
	start = clock();
	(*stack_) = (stack*)malloc(sizeof(stack));
	(*stack_)->size = 0;
	(*stack_)->top = NULL;
	for( i = 0 ; i < NUMBER_OF_DATAS ; i++ ){
		new_node = (node_t*)malloc(sizeof(node_t));
		new_node->data = data[i];
		if((*stack_)->top == NULL){
			new_node->next = new_node->prev = NULL;
			(*stack_)->top = new_node;
		}
		else{
			new_node->next = (*stack_)->top;
			new_node->prev = NULL;
			(*stack_)->top->prev = new_node;
			(*stack_)->top = new_node;
		}
		((*stack_)->size)++;
	} 
    printf("Stack Filled!\n");
    stop = clock();
    stack_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
    
	/*		Filling Queue		*/
	start = clock();
	(*queue_) = (queue*)malloc(sizeof(queue));
	(*queue_)->size = 0;
	(*queue_)->head = (*queue_)->tail = NULL;
	for(i = 0 ; i < NUMBER_OF_DATAS ; i++ ){
		new_node = (node_t*)malloc(sizeof(node_t));
		new_node->data = data[i];
		if( (*queue_)->head == NULL ){
			new_node->next = new_node->prev = NULL;
			(*queue_)->head = (*queue_)->tail = new_node;
		}
		else{
			new_node->next = (*queue_)->tail;
			new_node->prev = NULL;
			(*queue_)->tail->prev = new_node;
			(*queue_)->tail = new_node;
		}
		((*queue_)->size) ++;
	}
	printf("Queue Filled!\n");
	stop = clock();
    queue_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		Filling Binary Search Tree		*/
	start = clock();
	(*bst_) = (bst*)malloc(sizeof(bst));
	(*bst_)->size = 0;
	(*bst_)->root = NULL;
	for(i = 0 ; i < NUMBER_OF_DATAS ; i++ ){
		current_bst = (*bst_)->root;
		new_bst_node = (bst_node_t*)malloc(sizeof(bst_node_t));
		new_bst_node->data = data[i];
		new_bst_node->left = new_bst_node->right = NULL;
		if( i == 0 ){
			(*bst_)->root = new_bst_node;
		}
		else{
			while( current_bst->data != data[i] ){ // its blocking the duplicate
				if(current_bst->data > data[i]){
					if(current_bst->left == NULL){
						current_bst->left = new_bst_node;
					}
					current_bst = current_bst->left;
				}
				else if(current_bst->data < data[i]){
					if(current_bst->right == NULL){
						current_bst->right = new_bst_node;
					}
					current_bst = current_bst->right;
				}
			}
		}
		((*bst_)->size)++;
	}
	printf("BST Filled!\n");
	stop = clock();
    bst_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
    
    printf("\n%-15s\tStack\tQueue\tBST\n%-15s\t%-5.2lf\t%-5.2lf\t%-5.2lf","Structures","Exec. Time",stack_time,queue_time,bst_time);
	printf("\n**********************************************\n");
}

void ordered_print(stack * stack_, queue * queue_, bst * bst_){
	int i ;
	double stack_time , queue_time , bst_time ; 
	clock_t start,stop;
	node_t *current,*current_2;

	/*		Ordered Printing Stack		*/
	start = clock();
	current=stack_->top;
	while(current->next != NULL){
		current_2 = current->next;
		while(current_2 != NULL){
			if(current->data < current_2->data ){
				swap_nodes(current,current_2);
			}
			current_2 = current_2->next;
		}
		current = current->next;
	} 
	current=stack_->top;
	printf("\nStack Ordered!\nOrdered stack: ");
	for( i = 0 ; i < NUMBER_OF_DATAS ; i++ ){
		printf(" %d ",current->data);
		current= current->next;
	}
	printf("\n");
	stop = clock();
	stack_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		Ordered Printing Queue		*/
	start = clock();
	current=queue_->head;
	while(current->prev != NULL){
		current_2 = current->prev;
		while(current_2 != NULL){
			if(current->data < current_2->data ){
				swap_nodes(current,current_2);
			}
			current_2 = current_2->prev;
		}
		current = current->prev;
	} 
	current=queue_->head;
	printf("\nQueue Ordered!\nOrdered queue: ");
	for( i = 0 ; i < NUMBER_OF_DATAS ; i++ ){
		printf(" %d ",current->data);
		current= current->prev;
	}
	stop = clock();
	queue_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		Ordered Printing BST	*/
	start = clock();
	printf("\n\nBST Ordered!\nOrdered BST: ");
	bst_print(bst_->root);
	stop = clock();
	bst_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	printf("\n\n%-15s\tStack\tQueue\tBST\n%-15s\t%-5.2lf\t%-5.2lf\t%-5.2lf","Structures","Exec. Time",stack_time,queue_time,bst_time);
	printf("\n**********************************************\n");
} 

void bst_print(bst_node_t *root){
	if( root == NULL ) 		return;
	bst_print(root->right);
	printf(" %d ",root->data);
	bst_print(root->left);
}

void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search){
	double stack_time , queue_time , bst_time ; 
	clock_t start,stop;
	node_t *current_node , *founded_datas_list=NULL , *new_founded_data , *temp;
	bst_node_t *current_bst;
	int counter_1 , counter_2;  // counter 2 is for number of founded data
	
	/*		Stack Search		*/
	start = clock();
	current_node = stack_->top;
	temp = founded_datas_list;
	counter_1 = 1;
	counter_2 = 0;
	while(current_node != NULL ){
		if( current_node->data == val_to_search ){
				new_founded_data = (node_t*)malloc(sizeof(node_t));
				new_founded_data->data = counter_1; // storing that where is the value
				new_founded_data->next = NULL;
			if( founded_datas_list == NULL ){
				founded_datas_list = new_founded_data;
				temp = founded_datas_list;
			}
			else{
				temp->next = new_founded_data;
				temp = temp->next;
			}
			counter_2++;
		}
		counter_1++;
		current_node = current_node->next;
	}
	printf("\n\nSearch in Stack Complated!");
	if(counter_2 > 0){
		printf("\n%d result founded on %d. Step",counter_2,founded_datas_list->data);
		founded_datas_list = founded_datas_list->next;
		while ( founded_datas_list != NULL ){
			printf(", %d. Step",founded_datas_list->data);
			founded_datas_list = founded_datas_list->next;
		}
		printf(".\n");
	}
	else	printf("\nNo result found!\n");
	free(founded_datas_list);
	founded_datas_list=NULL;
	stop = clock();
	stack_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		Queue Search		*/
	start = clock();
	current_node = queue_->head;
	temp = founded_datas_list;
	counter_1 = 1;
	counter_2 = 0;
	while(current_node != NULL ){
		if( current_node->data == val_to_search ){
				new_founded_data = (node_t*)malloc(sizeof(node_t));
				new_founded_data->data = counter_1; // storing that where is the value
				new_founded_data->next = NULL;
			if( founded_datas_list == NULL ){
				founded_datas_list = new_founded_data;
				temp = founded_datas_list;
			}
			else{
				temp->next = new_founded_data;
				temp = temp->next;
			}
			counter_2++;
		}
		counter_1++;
		current_node = current_node->prev;
	}
	printf("\nSearch in Queue Complated! ");
	if(counter_2 > 0){
		printf("\n%d result founded on %d. Step",counter_2,founded_datas_list->data);
		founded_datas_list = founded_datas_list->next;
		while ( founded_datas_list != NULL ){
			printf(", %d. Step",founded_datas_list->data);
			founded_datas_list = founded_datas_list->next;
		}
		printf(".\n");
	}
	else	printf("\nNo result found!\n");
	free(founded_datas_list);
	founded_datas_list=NULL;
	stop = clock();
	queue_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		BST Search		*/
	start = clock();
	current_bst = bst_->root;
	counter_1 = 0;
	counter_2 = 0;
	while ( current_bst != NULL){	
		counter_1++;
		if(current_bst->data == val_to_search){
			counter_2++;
			current_bst = NULL; // end the loop
		}
		else if(current_bst->data > val_to_search ){
			current_bst = current_bst->left;
		}
		else if(current_bst->data < val_to_search ){
			current_bst = current_bst->right;
		}
	}
	if( counter_2 > 0 ){
		printf("\nSearch in BST Complated\n%d result found on %d. Step.",counter_2,counter_1);
	}
	stop = clock();
	bst_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	printf("\n\n%-15s\tStack\tQueue\tBST\n%-15s\t%-5.2lf\t%-5.2lf\t%-5.2lf","Structures","Exec. Time",stack_time,queue_time,bst_time);
	printf("\n**********************************************\n");
}

void special_traverse(stack * stack_, queue * queue_, bst * bst_){
	double stack_time , queue_time , bst_time ; 
	clock_t start,stop;
	node_t *min , *max;
	bst_node_t *current;
	int min_d,max_d,current_min,current_max;
	
	/*		Stack Special Traverse		*/
	start = clock();
	printf("\nStack Special Traverse Complated!\n");
	min = stack_->top;
	while( min->next != NULL ){
		min = min->next;
	}
	max = stack_->top;
	while( min != max && min != max->prev && max != min->next){
		printf("%d %d ",max->data,min->data);
		min = min->prev;
		max = max->next;
	}
	printf("\n");
	stop = clock();
	stack_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	/*		Queue Special Traverse		*/
	start = clock();
	printf("\n\nQueue Special Traverse Complated!\n");
	min = queue_->tail;
	max = queue_->head;
	while( min != max && min != max->next && max != min->prev){
		printf("%d %d ",max->data,min->data);
		min = min->next;
		max = max->prev;
	}
	printf("\n");
	stop = clock();
	queue_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
		
	/*		BST Special Traverse		*/
	start = clock();
	printf("\n\nBST Special Traverse Complated!\n");
	current= bst_->root;
	while( current->left !=NULL ){ 	// finding min data on bst
		current = current->left;
	}
	min_d=current->data;
	current= bst_->root;
	while( current->right !=NULL){	// finding max data on bst
		current = current->right;
	}
	max_d=current->data;
	current_min = min_d;
	current_max = max_d;
	while ( current_max > current_min ){
		printf("%d %d ",max_d,min_d);
		current_min = max_d; 	// reversing datas to go back/next 
		current_max = min_d;	// reversing datas to go back/next 
		bst_next_max_finder(bst_->root , &current_max , max_d);
		if(current_max > min_d){ // blocking the duplicate min/max
			bst_next_min_finder(bst_->root , &current_min , min_d);
		}
		max_d = current_max;
		min_d = current_min;
	}
	if ( current_max == current_min) printf("%d ", current_min); // needed to if min and max duplicate but that data didnt printed	
	stop = clock();
	bst_time = (double)(stop - start) * (double)1000 / CLOCKS_PER_SEC;
	
	
	printf("\n\n%-15s\tStack\tQueue\tBST\n%-15s\t%-5.2lf\t%-5.2lf\t%-5.2lf","Structures","Exec. Time",stack_time,queue_time,bst_time);
	printf("\n**********************************************\n");
}

void bst_next_min_finder(bst_node_t *root , int *current_min ,int last_min){
	if( root == NULL ) 		return;
	bst_next_min_finder(root->left , current_min , last_min);
	if(root->data < (*current_min) && root->data > last_min  ){
		(*current_min) = root->data;
	}
	bst_next_min_finder(root->right , current_min , last_min);
}
void bst_next_max_finder(bst_node_t *root , int *current_max ,int last_max){
	if( root == NULL ) 		return;
	bst_next_max_finder(root->left , current_max , last_max);
	if(root->data > (*current_max) && root->data < last_max ){
		(*current_max) = root->data;
	}
	bst_next_max_finder(root->right , current_max , last_max);
}

void swap_nodes(node_t *node_1 , node_t *node_2){
	int temp;
	temp = node_1->data;
	node_1->data = node_2->data;
	node_2->data = temp;
}
