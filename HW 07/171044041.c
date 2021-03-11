#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYER 2
#define MAX_BLOCK 20
#define MAX_HOUSE 3
#define BOARD_ROW 6
#define COLUMN 6
#define BLOCK_LENGTH 20
#define BLOCK_ROW 3
#define EMPTY -1
#define TRUE 1
#define FALSE 0

typedef enum type_p{
	noone , cap , car
}player_type;

typedef enum type_b{
	start , property , tax , punish
}block_type;

typedef struct{
	player_type type;
	int current_block_id;
	int owned_block_ids[11];
	int account;
	int turn_to_wait;
	char *name;
}player;

typedef struct{
	int block_id;
	char *name;
	int price;
	int rent;
	int rent_1 ;
	int rent_2;
	int rent_3;
	int house_price;
	int house_count;
	player owner;
	block_type type;
}block;

void init_the_board( block board[20]);
void init_the_players(player *player_one,player *player_two);
void show_board( block board[20], player player_one, player player_two);
void show_properties(block board[20]);
void buy_property(block *current_block, player *current_player);
void sell_property( block board[20], player *current_player); 
void gameplay ( block board[20], player player_one, player player_two); 
void printf_gameplay_menu();
int roll_the_dice();
void show_my_properties(block board[20], player current_player);
void bankrupt_function( block board[20], player *current_player);

int main(){
	int choice;
	player player_one,player_two;
	block board[MAX_BLOCK];
	
	srand(time(NULL));

	printf("Welcome to the MONOPOLY GAME!\n");
	
	init_the_board(board);
	init_the_players(&player_one,&player_two);
	gameplay (board,player_one,player_two);
	return 0;
}

void gameplay (block board[20],player player_one,player player_two){
	int choice,dice,flag;
	int gameover_flag=FALSE;
	int dice_situation;
	player *current_player=&player_one; //player 1 starts.
	player *current_enemy=&player_two;
	player *temp;
	
	while(gameover_flag==FALSE){
		dice_situation=FALSE; // reset dice situation
		show_board( board, player_one, player_two); 
		
		printf("\nNow turn is on %s\n",current_player->name);
		if(current_player->turn_to_wait > 0){
			printf("\n%s has punishment!Press enter to pass the turn.",current_player->name);
			current_player->turn_to_wait -=1;
			getchar();
			getchar();
		}
		else{
			do{
				flag=FALSE;
				printf_gameplay_menu();
				scanf("%d",&choice);
				if(choice == 1){
					if(dice_situation==TRUE){
						printf("You have already rolled the dice!\nPress enter to back gameplay menu!");
					}
					else{
						dice=roll_the_dice();
						dice_situation=TRUE;
						current_player->current_block_id +=dice;
						if(current_player->current_block_id > 19){
							current_player->current_block_id = current_player->current_block_id % 20;
							current_player->account += 10000;
						}
						show_board( board, player_one, player_two);
						printf("\nDice has rolled! %d is your lucky number! \n",dice);
						if(board[current_player->current_block_id].type==punish){
							printf("\nYou are  on the punish block! You have to wait %d turn.\n",board[current_player->current_block_id].rent);
							current_player->turn_to_wait += board[current_player->current_block_id].rent;
						}
						else if(board[current_player->current_block_id].type==tax){
							printf("\nYou are  on the tax block! You have paid %d $.\n",board[current_player->current_block_id].rent);
							current_player->account -= board[current_player->current_block_id].rent;
							if(current_player->account < 0){
								bankrupt_function(board,current_player);
							}
						}
						else if(board[current_player->current_block_id].owner.type == current_enemy->type){
							printf("\nYou are on the enemy property! You have paid rent!\n");
							current_player->account -= board[current_player->current_block_id].rent;
							if(board[current_player->current_block_id].house_count==1){
								current_player->account -= board[current_player->current_block_id].rent_1;
							}
							else if(board[current_player->current_block_id].house_count==2){
								current_player->account -= board[current_player->current_block_id].rent_2;
							}
							else if(board[current_player->current_block_id].house_count==3){
								current_player->account -= board[current_player->current_block_id].rent_3;
							}
							if(current_player->account < 0){
								bankrupt_function(board,current_player);
							}	
						}
						else if (board[current_player->current_block_id].owner.type == noone && board[current_player->current_block_id].type == property){
							buy_property(&board[current_player->current_block_id],current_player);
						}
						else if (board[current_player->current_block_id].owner.type == current_player->type){
							buy_property(&board[current_player->current_block_id],current_player);
						}
						printf("\nPress enter to pass turn!");
					}
					getchar();
					getchar();
				}
				else if(choice == 2){
					printf("\nYour account is: ***%d***\n\nPress enter to back gameplay menu!",current_player->account);
					getchar();
					getchar();
				}
				else if(choice == 3){
					show_my_properties(board,*current_player);
					printf("\nPress enter to back gameplay menu!");
					getchar();
					getchar();
				}
				else if(choice == 4){
					show_properties(board);
					printf("\nPress enter to back gameplay menu!");
					getchar();
					getchar();
				}
				else if(choice == 5){
					buy_property(&board[current_player->current_block_id],current_player);
					printf("\nPress enter to back gameplay menu!");
					getchar();
					getchar();
				}
				else if(choice == 6){
					if(board[current_player->current_block_id].owner.type != current_player->type && board[current_player->current_block_id].type == property){
						printf("\nYou must be the owner of the porperty to buy house!\n");
					}
					else{
						buy_property(&board[current_player->current_block_id],current_player);
					}
					printf("\nPress enter to back gameplay menu!");
					getchar();
					getchar();
				}
				else if(choice == 7){
					sell_property(board,current_player);
				}
				
				else  {
					printf("\nInvalid choice!\n");
				}		
			}while(choice !=1  );
		}
		if(current_player->account < 0){
			gameover_flag = TRUE;
			printf("\n\n*\n**\n%s has bankrupted and %s won the game !\n**\n*\n",current_player->name,current_enemy->name);
		}
		else {
			temp=current_player;
			current_player=current_enemy;
			current_enemy=temp;
		}	
	}
}

void buy_property(block *current_block,player *current_player){
	int i,choice,flag=FALSE;
	char yes_or_no;
	if(current_block->owner.type == noone && current_block->type == property ){
		printf("\nDo you want to buy %s ? \nIf you want, press Y/y. Else press anything:",current_block->name);
		getchar();
		scanf("%c",&yes_or_no);
		if(yes_or_no == 'Y' || yes_or_no == 'y'){
			if(current_block->price < current_player->account){
				current_player->account -= current_block->price;
				current_block->owner.type = current_player->type;
				printf("\nYou bought the property!\n");
				if(current_block->house_price < current_player->account) printf("You can buy house now!\n");
			}
			else{
				printf("\nYou dont have enough money to buy this property.\n");
			}
		}
			
	}
	
	else if(current_block->type != property){
		printf("\nThis block connot be bought\n");
	}
	
	else if (current_block->owner.type != noone && current_block->owner.type != current_player->type && current_block->type == property ){
		printf("\nYou cant buy this property.This property has already bought!\n");
	}
	
	if(current_block->owner.type == current_player->type){
		do{
			printf("\nPlease enter the number of houses do you want to buy.(0/1/2/3)\nChoice:");
			scanf("%d",&choice);
			if((current_block->house_count + choice) > MAX_HOUSE){
				printf("You cant have more then %d house on the property.\n",MAX_HOUSE);
			}
			else if((current_block->house_price * choice) > current_player->account){
				printf("You dont have enough money to buy %d house(s).\n",choice);
			}
			else if(choice == 0) flag==TRUE;
			
			else{
				current_player->account -= (current_block->house_price * choice);
				current_block->house_count += choice;
				printf("You bought the house(s)!\n");
				flag=TRUE;
			}
		}while(choice <0 && choice > MAX_HOUSE && flag==FALSE);
	}
	
}

void sell_property( block board[20], player *current_player){
	int i,choice,total;
	printf("\n**YOUR PROPERTIES**\n");
	printf("\nPlease select a property to sell:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		if(board[i].owner.type==current_player->type){
			printf("%2d -\t%s\n",i,board[i].name);
		}
	}
	printf("\n%2d -\t%s\n",0,"Exit");
	do{
		printf("Choice:");
		scanf("%d",&choice);
		if(board[choice].owner.type == current_player->type){
			total= board[choice].price + (board[choice].house_count * board[choice].house_price);
			current_player->account += (total / 2 );
			board[choice].house_count = 0;
			board[choice].owner.type = noone;
			printf("\nThe property has been sold!You earned %d$ \n",(total/2));
			printf("\nPlease select a property to sell:\n");
			for(i=0; i < MAX_BLOCK ; i++){
				if(board[i].owner.type==current_player->type){
					printf("%2d -\t%s\n",i,board[i].name);
				}
			}
			printf("\n%2d -\t%s\n",0,"Exit");
		}
		
		else if(choice!=0 && board[choice].owner.type!=current_player->type){
			printf("Invalid Choice!\n");
		}
	}while(choice != 0);
	
}

void bankrupt_function( block board[20], player *current_player){
	int flag=FALSE,i;
	printf("\n\n**\nYou are in debt!\nYou have to sell property.\nIf you cant afford the debt although you sold all of your properties , you will bankrupt at end of your turn.\n**\n ");
	while(current_player->account < 0 && flag== FALSE){
		sell_property(board,current_player);
		flag=TRUE;
		for(i=0; i <MAX_BLOCK ; i++){
			if(board[i].owner.type == current_player->type ) flag=FALSE;
		}
		if(current_player->account < 0 && flag== FALSE){
			printf("You didnt sold all of your properties!\n");
		}
		else if (current_player->account > 0){
			printf("\nDone! You are not in debt anymore!");
		}
		else if(current_player->account < 0 && flag== TRUE){
			printf("Sorry! You will bankrupt and at end of your turn!\n");
		}
	}
}

void show_properties(block board[20]){
	int i,choice;
	printf("\nPlease select a property to see details:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		if(board[i].type==property){
			printf("%2d -\t%s\n",i,board[i].name);
		}
	}
	printf("%2d -\t%s\n",0,"Exit");	
	do{
		printf("Choice:");
		scanf("%d",&choice);
		if(board[choice].type==property){
			printf("---------------------------------\n");
			printf("|\t    %-20s|\n",  board[choice].name);
			printf("---------------------------------\n");
			printf("|\t%-8s\t%-5d$  |\n", "Rent",board[choice].rent);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 1 H",board[choice].rent_1);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 2 H",board[choice].rent_2);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 3 H",board[choice].rent_3);
			printf("---------------------------------\n");
			printf("|\t%-11s     %-5d$  |\n", "House Price",board[choice].price);
			printf("---------------------------------\n");
		}
		else if(board[choice].type!=property && choice!=0){
			printf("Invalid Choice!\n");
		}
	}while(choice != 0);
}

void show_my_properties(block board[20], player current_player){
	int i,choice;
	printf("\n**YOUR PROPERTIES**\n");
	printf("\nPlease select a property to see details:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		if(board[i].owner.type==current_player.type){
			printf("%2d -\t%s\n",i,board[i].name);
		}
	}
	printf("%2d -\t%s\n",0,"Exit");
	do{
		printf("Choice:");
		scanf("%d",&choice);
		if(board[choice].type==property && board[choice].owner.type==current_player.type){
			printf("---------------------------------\n");
			printf("|\t    %-20s|\n",  board[choice].name);
			printf("---------------------------------\n");
			printf("|\t%-8s\t%-5d$  |\n", "Rent",board[choice].rent);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 1 H",board[choice].rent_1);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 2 H",board[choice].rent_2);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 3 H",board[choice].rent_3);
			printf("---------------------------------\n");
			printf("|\t%-11s     %-5d$  |\n", "House Price",board[choice].price);
			printf("---------------------------------\n");
		}
		else if(choice!=0 && board[choice].owner.type!=current_player.type){
			printf("Invalid Choice!\n");
		}
	}while(choice != 0 );	
}

int roll_the_dice(){
	int r;
	r=rand()%6;
	if(r==0) r=6;
	return r;
}

void printf_gameplay_menu(){
	printf("\n1-Roll the dice");
	printf("\n2-Show my account");
	printf("\n3-Show my properties");
	printf("\n4-Show property deeds");
	printf("\n5-Buy property");
	printf("\n6-Buy house");
	printf("\n7-Sell property");
	printf("\nPlease select an option to continue:");
}

void init_the_players(player *player_one,player *player_two){
	int choice,i ;	
	printf("Please select for Player 1. Other one will be of Player 2\n1-Cap\n2-Car");
	do{
		printf("\nChoice:");
		scanf("%d",&choice);
		if(choice != 1 && choice !=2){
			printf("Invalid choice!\n");
		}
	}while(choice != 1 && choice !=2);
	if(choice==1){
		player_one->type=cap;
		player_two->type=car;
		player_one->name="Cap";
		player_two->name="Car";
	}
	else{
		player_one->type=car;
		player_two->type=cap;
		player_one->name="Car";
		player_two->name="Cap";
	}
	
	player_one->current_block_id=0;
	player_two->current_block_id=0;
	
	for(i=0 ; i < 11 ; i++){
		player_one->owned_block_ids[i]=EMPTY;
	}
	for(i=0 ; i < 11 ; i++){
		player_two->owned_block_ids[i]=EMPTY;
	}
	
	player_one->account=100000; // start money
	player_two->account=100000;
	
	player_one->turn_to_wait=0;
	player_two->turn_to_wait=0;
}



void show_board( block board[20], player player_one, player player_two){
	int i,k,l;
	int block_id_left,block_id_right;
	
	for(i=0; i < BOARD_ROW ; i++){
		if( i == 0){  //top side of the board
			printf("-");
			for(k=0 ; k < COLUMN ; k++){	//printing - for length
				for(l=0 ; l < BLOCK_LENGTH ; l++){
					printf("-");
				}
				printf("-");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){	// printing names of blocks
				printf("    %-16s",board[k].name);
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){ // printing prices. there are ifs because we put the prices of texes into .rent(as in excel file). So i need to do that.
				if(board[k].type == property ){
					printf("    %-6d$         ",board[k].price); 
				}
				else if(board[k].type == tax){
					printf("    %-6d$         ",board[k].rent);
				}
				else{
					for(l=0 ; l<BLOCK_LENGTH ; l++){
						printf(" ");
					}
				}
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){ // printing who is there
				if(player_one.current_block_id == k && player_two.current_block_id == k){ // checking that if both of them is there
					printf("    %-6s %-6s   ",player_one.name,player_two.name);
				}
				else if(player_one.current_block_id == k){ // checking that if just player one is there
					printf("    %-16s",player_one.name);
				}
				else if(player_two.current_block_id == k){ // checking that if just player two is there
					printf("    %-16s",player_two.name);
				}
				else{
					for(l=0 ; l<BLOCK_LENGTH ; l++){
						printf(" ");
					}
				}
				printf("|");
			}
			printf("\n");
			printf("-");
			for(k=0 ; k < COLUMN ; k++){ // printing (-----)
				for(l=0 ; l < BLOCK_LENGTH ; l++){
					printf("-");
				}
				printf("-");
			}
			printf("\n");
		}
		else if( i == BOARD_ROW-1){ // this has same loops with first if. difference is block id decided by "MAX_BLOCK - i - k"
			printf("|");
			for(k=0 ; k < COLUMN ; k++){
				printf("    %-16s",board[MAX_BLOCK - i - k].name);
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){
				if(board[MAX_BLOCK - i - k].type == property ){
					printf("    %-6d$         ",board[MAX_BLOCK - i - k].price);
				}
				else if(board[MAX_BLOCK - i - k].type == tax){
					printf("    %-6d$         ",board[MAX_BLOCK - i - k].rent);
				}
				else{
					for(l=0 ; l<BLOCK_LENGTH ; l++){
						printf(" ");
					}
				}
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){
				if(player_one.current_block_id == MAX_BLOCK - i - k && player_two.current_block_id == MAX_BLOCK - i - k){
					printf("    %-6s %-6s   ",player_one.name,player_two.name);
				}
				else if(player_one.current_block_id == MAX_BLOCK - i - k){
					printf("    %-16s",player_one.name);
				}
				else if(player_two.current_block_id == MAX_BLOCK - i - k){ 
					printf("    %-16s",player_two.name);
				}
				else{
					for(l=0 ; l<BLOCK_LENGTH ; l++){
						printf(" ");
					}
				}
				printf("|");
			}
			printf("\n");
			printf("-");
			for(k=0 ; k < COLUMN ; k++){
				for(l=0 ; l < BLOCK_LENGTH ; l++){
					printf("-");
				}
				printf("-");
			}
			printf("\n");		
		}
		else{  	// this has similiar loops like second but there are two differences. first difference is block id decided by block_id_left and block_id_right
				// second difference is there are spece( ) loops for center of board.
			block_id_left= MAX_BLOCK - i ;
			block_id_right= COLUMN + i -1 ;
			
			printf("|    %-16s|",board[block_id_left].name);
			
			
			for(k=0; k < ((COLUMN-2) * BLOCK_LENGTH) + COLUMN - 3 ; k++ ){  // spece( ) loop for center of board.
				printf(" ");
			}
			
			
			printf("|    %-16s|\n|",board[block_id_right].name);
			
			
			if(board[block_id_left].type == property ){
					printf("    %-6d$         ",board[block_id_left].price);
				}
			else if(board[block_id_left].type == tax){
					printf("    %-6d$         ",board[block_id_left].rent);
				}
			else{
				for(l=0 ; l<BLOCK_LENGTH ; l++){
					printf(" ");
				}
			}
			printf("|");
			
			
			for(k=0; k < ((COLUMN-2) * BLOCK_LENGTH) + COLUMN - 3 ; k++ ){	// spece( ) loop for center of board.
				printf(" ");
			}
			
			
			printf("|");
			if(board[block_id_right].type == property ){
				printf("    %-6d$         ",board[block_id_right].price);
			}
			else if(board[block_id_right].type == tax){
					printf("    %-6d$         ",board[block_id_right].rent);
				}
			else{
				for(l=0 ; l<BLOCK_LENGTH ; l++){
					printf(" ");
				}
			}
			printf("|\n|");
			
			
			if(player_one.current_block_id == block_id_left && player_two.current_block_id == block_id_left){
				printf("    %-6s %-6s   ",player_one.name,player_two.name);
			}
			else if(player_one.current_block_id == block_id_left){
				printf("    %-16s",player_one.name);
			}
			else if(player_two.current_block_id == block_id_left){ 
					printf("    %-16s",player_two.name);
				}
			else{
				for(l=0 ; l<BLOCK_LENGTH ; l++){
					printf(" ");
				}
			}
			printf("|");
			
			
			for(k=0; k < ((COLUMN-2) * BLOCK_LENGTH) + COLUMN - 3 ; k++ ){	// spece( ) loop for center of board.
				printf(" ");
			}
			
			
			printf("|");
			if(player_one.current_block_id == block_id_right && player_two.current_block_id == block_id_right){
				printf("    %-6s %-6s   ",player_one.name,player_two.name);
			}
			else if(player_one.current_block_id == block_id_right){
				printf("    %-16s",player_one.name);
			}
			else if(player_two.current_block_id == block_id_right){ 
				printf("    %-16s",player_two.name);
			}
			else{
				for(l=0 ; l<BLOCK_LENGTH ; l++){
					printf(" ");
				}
			}
			printf("|\n");
			if(i== BOARD_ROW-2){
				for(k=0; k < (COLUMN * BLOCK_LENGTH) + COLUMN + 1 ; k++ ){
					printf("-");
				}
				printf("\n");
			}
			else{
				for(l=0 ; l<BLOCK_LENGTH+2 ; l++){
					printf("-");
				}
				for(k=0; k < ((COLUMN-2) * BLOCK_LENGTH) + COLUMN - 3 ; k++ ){
					printf(" ");
				}
				for(l=0 ; l<BLOCK_LENGTH+2 ; l++){
					printf("-");
				}
				printf("\n");
			}	
		}
	}	
}



void init_the_board(block board[20]){
	board[0].name="Start";
	board[0].block_id=0 ;
	board[0].price=0 ;
	board[0].rent=0 ;
	board[0].rent_1=0 ;
	board[0].rent_2=0 ;
	board[0].rent_3=0 ;
	board[0].house_price=0 ;
	board[0].house_count=0 ;
	board[0].type=start ;
	board[0].owner.type=noone; 
	
	board[1].name="Esenyurt";
	board[1].block_id=1 ;
	board[1].price=16000 ;
	board[1].rent=800 ;
	board[1].rent_1=4000 ;
	board[1].rent_2=9000 ;
	board[1].rent_3=25000 ;
	board[1].house_price=10000 ;
	board[1].house_count=0 ;
	board[1].type=property ; 
	board[1].owner.type=noone; 
	
	board[2].name="Car Park";
	board[2].block_id=2 ;
	board[2].price=0 ;
	board[2].rent=1500 ;
	board[2].rent_1=0 ;
	board[2].rent_2=0 ;
	board[2].rent_3=0 ;
	board[2].house_price=0 ;
	board[2].house_count=0 ;
	board[2].type=tax ;  
	board[2].owner.type=noone; 
	
	board[3].name="Tuzla";
	board[3].block_id=3 ;
	board[3].price=16500 ;
	board[3].rent=850 ;
	board[3].rent_1=4250 ;
	board[3].rent_2=9500 ;
	board[3].rent_3=26000 ;
	board[3].house_price=12000 ;
	board[3].house_count=0 ;
	board[3].type=property ;  
	board[3].owner.type=noone; 
	
	board[4].name="Arnavutkoy";
	board[4].block_id=4 ;
	board[4].price=17000 ;
	board[4].rent=875 ;
	board[4].rent_1=4500 ;
	board[4].rent_2=10000 ;
	board[4].rent_3=28000 ;
	board[4].house_price=12000 ;
	board[4].house_count=0 ;
	board[4].type=property ;  
	board[4].owner.type=noone; 
	
	board[5].name="Wait 2 Turn";
	board[5].block_id=5 ;
	board[5].price=0 ;
	board[5].rent=2 ;
	board[5].rent_1=0 ;
	board[5].rent_2=0 ;
	board[5].rent_3=0 ;
	board[5].house_price=0 ;
	board[5].house_count=0 ;
	board[5].type=punish ;  
	board[5].owner.type=noone; 

	board[6].name="Catalca";
	board[6].block_id=6 ;
	board[6].price=20000 ;
	board[6].rent=1000 ;
	board[6].rent_1=5000 ;
	board[6].rent_2=12000 ;
	board[6].rent_3=30000 ;
	board[6].house_price=13000 ;
	board[6].house_count=0 ;
	board[6].type=property ;  
	board[6].owner.type=noone; 
	
	board[7].name="Beykoz";
	board[7].block_id=7 ;
	board[7].price=23000 ;
	board[7].rent=1100 ;
	board[7].rent_1=5500 ;
	board[7].rent_2=12500 ;
	board[7].rent_3=33000 ;
	board[7].house_price=13000 ;
	board[7].house_count=0 ;
	board[7].type=property ;  
	board[7].owner.type=noone; 
	
	board[8].name="Car Fix";
	board[8].block_id=8 ;
	board[8].price=0 ;
	board[8].rent=1750 ;
	board[8].rent_1=0 ;
	board[8].rent_2=0 ;
	board[8].rent_3=0 ;
	board[8].house_price=0 ;
	board[8].house_count=0 ;
	board[8].type=tax ;  
	board[8].owner.type=noone; 
	
	board[9].name="Maltepe";
	board[9].block_id=9 ;
	board[9].price=30000 ;
	board[9].rent=1350 ;
	board[9].rent_1=7000 ;
	board[9].rent_2=15000 ;
	board[9].rent_3=40000 ;
	board[9].house_price=15000 ;
	board[9].house_count=0 ;
	board[9].type=property ;  
	board[9].owner.type=noone; 
	
	board[10].name="Bills";
	board[10].block_id=10 ;
	board[10].price=0 ;
	board[10].rent=2000 ;
	board[10].rent_1=0 ;
	board[10].rent_2=0 ;
	board[10].rent_3=0 ;
	board[10].house_price=0 ;
	board[10].house_count=0 ;
	board[10].type=tax ;  
	board[10].owner.type=noone; 
	
	board[11].name="Sisli";
	board[11].block_id=11 ;
	board[11].price=33000 ;
	board[11].rent=1500 ;
	board[11].rent_1=8000 ;
	board[11].rent_2=16000 ;
	board[11].rent_3=42000 ;
	board[11].house_price=16000 ;
	board[11].house_count=0 ;
	board[11].type=property ;  
	board[11].owner.type=noone; 
	
	board[12].name="Oil";
	board[12].block_id=12 ;
	board[12].price=0 ;
	board[12].rent=2250 ;
	board[12].rent_1=0 ;
	board[12].rent_2=0 ;
	board[12].rent_3=0 ;
	board[12].house_price=0 ;
	board[12].house_count=0 ;
	board[12].type=tax ;  
	board[12].owner.type=noone; 
	
	board[13].name="Atasehir";
	board[13].block_id=13 ;
	board[13].price=35000 ;
	board[13].rent=1600 ;
	board[13].rent_1=8500 ;
	board[13].rent_2=17000 ;
	board[13].rent_3=45000 ;
	board[13].house_price=17000 ;
	board[13].house_count=0 ;
	board[13].type=property ;  
	board[13].owner.type=noone; 
	
	board[14].name="Sariyer";
	board[14].block_id=14 ;
	board[14].price=40000 ;
	board[14].rent=1750 ;
	board[14].rent_1=9500 ;
	board[14].rent_2=19000 ;
	board[14].rent_3=48000 ;
	board[14].house_price=19000 ;
	board[14].house_count=0 ;
	board[14].type=property ;  
	board[14].owner.type=noone; 
	
	board[15].name="Wait 1 Turn";
	board[15].block_id=15 ;
	board[15].price=0 ;
	board[15].rent=1 ;
	board[15].rent_1=0 ;
	board[15].rent_2=0 ;
	board[15].rent_3=0 ;
	board[15].house_price=0 ;
	board[15].house_count=0 ;
	board[15].type=punish ;  
	board[15].owner.type=noone; 
	
	board[16].name="Kadikoy";
	board[16].block_id=16 ;
	board[16].price=43000 ;
	board[16].rent=1900 ;
	board[16].rent_1=11000 ;
	board[16].rent_2=21500 ;
	board[16].rent_3=55000 ;
	board[16].house_price=23000 ;
	board[16].house_count=0 ;
	board[16].type=property ;  
	board[16].owner.type=noone; 
	
	board[17].name="Besiktas";
	board[17].block_id=17 ;
	board[17].price=60000 ;
	board[17].rent=2500 ;
	board[17].rent_1=15000 ;
	board[17].rent_2=28000 ;
	board[17].rent_3=60000 ;
	board[17].house_price=30000 ;
	board[17].house_count=0 ;
	board[17].type=property ;  
	board[17].owner.type=noone; 
	
	board[18].name="Vocation";
	board[18].block_id=18 ;
	board[18].price=0 ;
	board[18].rent=5000 ;
	board[18].rent_1=0 ;
	board[18].rent_2=0 ;
	board[18].rent_3=0 ;
	board[18].house_price=0 ;
	board[18].house_count=0 ;
	board[18].type=tax ;  
	board[18].owner.type=noone; 
	
	board[19].name="Bebek";
	board[19].block_id=19 ;
	board[19].price=70000 ;
	board[19].rent=3500 ;
	board[19].rent_1=20000 ;
	board[19].rent_2=35500 ;
	board[19].rent_3=65000 ;
	board[19].house_price=35000 ;
	board[19].house_count=0 ;
	board[19].type=property ;  
	board[19].owner.type=noone; 
	
}

