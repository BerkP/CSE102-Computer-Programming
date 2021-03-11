#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYER 2
#define MAX_BLOCK 24
#define MAX_HOUSE 3
#define BOARD_ROW 7
#define COLUMN 7
#define BLOCK_LENGTH 20
#define BLOCK_ROW 3
#define EMPTY -1
#define TRUE 1
#define FALSE 0
#define NUMBER_OF_PROPERTIES 12

typedef enum type_p{
	noone , cap , car
}player_type;

typedef enum type_b{
	start , property , tax , punish , fortune
}block_type;

typedef enum type_fc{
	free_house , time_travel , garnishment ,generosity , treasure_hunter
}fortune_card_type;

typedef struct{
	player_type type;
	int current_block_id;
	int owned_block_ids[11];
	int account;
	int turn_to_wait;
	char *name;
}player;

typedef struct block_s{
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
	struct block_s *next;
}block;

typedef struct fcard_s{
	fortune_card_type card;
	char *name;
	struct fcard_s *next_card;
}fortune_card;

void init_the_board( block *board);
void init_the_players(player *player_one,player *player_two);
void show_board( block *board, player player_one, player player_two);
block block_finder(block *board_head , int id );
void gameplay ( block *board, player player_one, player player_two);
void buy_property_for_computer(block *current_block,player *current_player,int avarage,int *property_count);
void sell_property_for_computer(block *board, player *current_player);
void buy_house_for_computer(block *current_block, player *current_player,int *property_count);
void printf_gameplay_menu();
int roll_the_dice();
void show_my_properties(block *board, player current_player);
void show_properties(block *board);
block *pointer_block_finder(block *board_head , int id );
void buy_property(block *current_block, player *current_player);
void sell_property( block *board, player *current_player); 
void bankrupt_function( block *board, player *current_player); 
void fortune_card_func(player *current_player , player *enemy_player ,block *board,int computer_property_count);
void init_fortune_cards(fortune_card *cards);

int main(){
	int choice;
	player player_one,player_pc;
	block *board_head=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	block *board,current_block;
	board=board_head;
	srand(time(NULL));
	printf("Welcome to the MONOPOLY GAME!\n");	
	init_the_board(board);
	init_the_players(&player_one,&player_pc);
	gameplay (board_head,player_one,player_pc);
	return 0;
}

void gameplay (block *board,player player_one,player player_pc){
	int choice,dice,flag;
	int gameover_flag=FALSE;
	int dice_situation;
	int computer_property_count=0, avarage_property_price=0;
	player *current_player=&player_one; //player 1 starts.
	player *current_enemy=&player_pc;
	player *temp;
	block *current_block;	
	current_block=board;
	while(current_block->next != NULL){
		if(current_block->type == property){
			avarage_property_price += current_block->price;
		}
		current_block=current_block->next;	
	}
	avarage_property_price += current_block->price;
	avarage_property_price = avarage_property_price /  NUMBER_OF_PROPERTIES ;
	
	while(gameover_flag==FALSE){
		dice_situation=FALSE; // reset dice situation
		show_board( board, player_one, player_pc); 
		current_block=pointer_block_finder(board,current_player->current_block_id);
		
		printf("\nNow turn is on %s\n",current_player->name);
		if(current_player->turn_to_wait > 0){
			printf("\n%s has punishment!Press enter to pass the turn.\n",current_player->name);
			current_player->turn_to_wait -=1;
			getchar();
		}
		else if (current_player == &player_one){
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
						if(current_player->current_block_id > 23){
							current_player->current_block_id = current_player->current_block_id % 24;
							current_player->account += 10000;
						}
						current_block=pointer_block_finder(board,current_player->current_block_id);
						show_board( board, player_one, player_pc);
						printf("\nDice has rolled! %d is your lucky number! \n",dice);
						if(current_block->type==punish){
							printf("\nYou are  on the punish block! You have to wait %d turn.\n",current_block->rent);
							current_player->turn_to_wait += current_block->rent;
						}
						else if(current_block->type==tax){
							printf("\nYou are  on the tax block! You have paid %d $.\n",current_block->rent);
							current_player->account -= current_block->rent;
							if(current_player->account < 0){
								bankrupt_function(board,current_player);
							}
						}
						else if(current_block->owner.type == current_enemy->type){
							printf("\nYou are on the enemy property! You have paid rent!\n");
							current_player->account -= current_block->rent;
							if(current_block->house_count==1){
								current_player->account -= current_block->rent_1;
							}
							else if(current_block->house_count==2){
								current_player->account -= current_block->rent_2;
							}
							else if(current_block->house_count==3){
								current_player->account -= current_block->rent_3;
							}
							if(current_player->account < 0){
								bankrupt_function(board,current_player);
							}	
						}
						else if (current_block->owner.type == noone && current_block->type == property){
							buy_property(current_block,current_player);
						}
						else if (current_block->owner.type == current_player->type){
							buy_property(current_block,current_player);
						}
						else if(current_block->type == fortune){
							fortune_card_func(current_player , current_enemy , board, computer_property_count);
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
					buy_property(current_block,current_player);
					printf("\nPress enter to back gameplay menu!");
					getchar();
					getchar();
				}
				else if(choice == 6){
					if(current_block->owner.type != current_player->type && current_block->type == property){
						printf("\nYou must be the owner of the porperty to buy house!\n");
					}
					else{
						buy_property(current_block,current_player);
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
		else{ // gameplay for computer
			sell_property_for_computer(board,current_player);
			
			dice=roll_the_dice();
			dice_situation=TRUE;
			current_player->current_block_id +=dice;
			if(current_player->current_block_id > 23){
				current_player->current_block_id = current_player->current_block_id % 24;
				current_player->account += 10000;
			}
			current_block=pointer_block_finder(board,current_player->current_block_id);
			printf("\nComputer rolled the dice: %d \n",dice);
			if(current_block->owner.type == noone && current_block->type == property ){
				buy_property_for_computer(current_block,current_player, avarage_property_price,&computer_property_count);
			}
			else if(current_block->owner.type == current_player->type){
				buy_house_for_computer(current_block, current_player,&computer_property_count);
			}
			//
			else if(current_block->type==punish){
				printf("\nComputer is  on the punish block! Computer have to wait %d turn.\n",current_block->rent);
				current_player->turn_to_wait += current_block->rent;
			}
			else if(current_block->type==tax){
				printf("\nComputer is  on the tax block! Computer has paid %d $.\n",current_block->rent);
				current_player->account -= current_block->rent;
				if(current_player->account < 0){
					sell_property_for_computer(board,current_player);
				}
			}
			else if(current_block->owner.type == current_enemy->type){
				printf("\nComputer is on your property! Computer has paid rent!\n");
				current_player->account -= current_block->rent;
				if(current_block->house_count==1){
					current_player->account -= current_block->rent_1;
				}
				else if(current_block->house_count==2){
					current_player->account -= current_block->rent_2;
				}
				else if(current_block->house_count==3){
					current_player->account -= current_block->rent_3;
				}
				if(current_player->account < 0){
					sell_property_for_computer(board,current_player);
				}	
			}
			else if(current_block->type == fortune){
				fortune_card_func(current_player , current_enemy , board, computer_property_count);
			}
			printf("\nPress enter to pass turn!\n");
			getchar();
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

void fortune_card_func(player *current_player , player *enemy_player ,block *board, int computer_property_count){
	int dice,i;
	block *lucky_block;
	block *current_block;
	fortune_card *card_head=(fortune_card*)malloc(sizeof(fortune_card) + (sizeof(char)*20));
	fortune_card *current_card;
	current_card=card_head;
	init_fortune_cards(current_card);
	current_card=card_head;
	lucky_block=board;
	
	dice=roll_the_dice();
	if(dice == 6) dice =1; //roll dice func can return 6 but we have 5 card.
	for(i=1 ; i<dice ; i++ ){
		current_card=current_card->next_card;
	}
	printf("%s is on the fortune block!\n%s's Fortune card is %s\n",current_player->name,current_player->name,current_card->name);
	
	if(current_card->card == free_house ){
		while(lucky_block->owner.type != current_player->type && lucky_block->next != NULL){
			lucky_block=lucky_block->next;
		}
		if(lucky_block->owner.type == car || lucky_block->owner.type == cap){
			printf("\n%s has one more house now! \n",lucky_block->name);
			lucky_block->house_count += 1;
		}
		else{
			printf("\n%s doesnt have any property to build a house\n",current_player->name);
		}
	}
	else if(current_card->card == time_travel ){  // this card have more things because it has to behave like dice rolling
		dice=roll_the_dice();
		if(dice >0 && dice < 4){
			current_player->current_block_id +=2;
			printf("%s moved 2 block forward.\n",current_block->name);
		} 
		else{
			current_player->current_block_id -=2;
			printf("%s moved 2 block backward.\n",current_block->name);
		}  
		current_block=pointer_block_finder(board,current_player->current_block_id);
		
		if(current_player->type == car){
			if(current_block->type==punish){
				printf("\nYou are  on the punish block! You have to wait %d turn.\n",current_block->rent);
				current_player->turn_to_wait += current_block->rent;
			}
			else if(current_block->type==tax){
				printf("\nYou are  on the tax block! You have paid %d $.\n",current_block->rent);
				current_player->account -= current_block->rent;
				if(current_player->account < 0){
					bankrupt_function(board,current_player);
				}
			}
			else if(current_block->owner.type == enemy_player->type){
				printf("\nYou are on the enemy property! You have paid rent!\n");
				current_player->account -= current_block->rent;
				if(current_block->house_count==1){
					current_player->account -= current_block->rent_1;
				}
				else if(current_block->house_count==2){
					current_player->account -= current_block->rent_2;
				}
				else if(current_block->house_count==3){
					current_player->account -= current_block->rent_3;
				}
				if(current_player->account < 0){
					bankrupt_function(board,current_player);
				}	
			}
			else if (current_block->owner.type == noone && current_block->type == property){
				buy_property(current_block,current_player);
			}
			else if (current_block->owner.type == current_player->type){
				buy_property(current_block,current_player);
			}
		}
		else{
			if(current_block->type==punish){
				printf("\nComputer is  on the punish block! Computer have to wait %d turn.\n",current_block->rent);
				current_player->turn_to_wait += current_block->rent;
			}
			else if(current_block->type==tax){
				printf("\nComputer is  on the tax block! Computer has paid %d $.\n",current_block->rent);
				current_player->account -= current_block->rent;
				if(current_player->account < 0){
					sell_property_for_computer(board,current_player);
				}
			}
			else if(current_block->owner.type == enemy_player->type){
				printf("\nComputer is on your property! Computer has paid rent!\n");
				current_player->account -= current_block->rent;
				if(current_block->house_count==1){
					current_player->account -= current_block->rent_1;
				}
				else if(current_block->house_count==2){
					current_player->account -= current_block->rent_2;
				}
				else if(current_block->house_count==3){
					current_player->account -= current_block->rent_3;
				}
				if(current_player->account < 0){
					sell_property_for_computer(board,current_player);
				}	
			}
		}
	}
	else if(current_card->card == garnishment ){
		printf("\n%s paid 5.000 to bank!\n",current_player->name);
		current_player->account -=5000;
	}
	else if(current_card->card == generosity ){
		printf("\n%s paid 10.000 to %s!\n",current_player->name,enemy_player->name);
		current_player->account -=10000;
		enemy_player->account +=10000;
	}
	else if(current_card->card == treasure_hunter ){
		printf("\n%s gained 20.000 from bank!\n",current_player->name);
		current_player->account +=20000;
	}
	if(current_player->type == car && current_player->account < 0)  bankrupt_function(board,current_player);
	else if(current_player->type == cap && current_player->account < 0)  sell_property_for_computer(board,current_player);
}

void sell_property_for_computer(block *board, player *current_player){ // this function includes bunkrupt function for computer
	int total,flag=TRUE;
	block *max_value_block,*current_block;
	while(current_player->account < 0 && flag==TRUE){
		current_block=board;
		max_value_block=board;
		while(current_block != NULL){
			if(current_block->price > max_value_block->price ){
				max_value_block=current_block;
			}
			current_block=current_block->next;
		}
		if(max_value_block->owner.type == current_player->type){
			total=max_value_block->price + (max_value_block->house_count * max_value_block->house_price);
			current_player->account += (total / 2 );
			max_value_block->house_count = 0;
			max_value_block->owner.type = noone;
			printf("\nComputer sold %s to avoid bunkrupt!\n",max_value_block->name);
		}
		else flag=FALSE;
	}
}

void buy_property_for_computer(block *current_block,player *current_player,int avarage,int *property_count){
	int dice;
	dice=roll_the_dice();
	if(current_block->price < avarage || dice > 0 && dice <4){
		if(current_block->price < current_player->account){
			current_player->account -= current_block->price;
			current_block->owner.type = current_player->type;
			printf("\nComputer bought %s!\n",current_block->name);
			property_count++;
		}
		else{
			printf("\nComputer doesnt have enough money to buy %s.\n",current_block->name);
		}	
	}	
}

void buy_house_for_computer(block *current_block, player *current_player,int *property_count){
	int dice;
	if(*property_count >= (NUMBER_OF_PROPERTIES / 3) && current_player->account > current_block->house_price && current_block->house_count < MAX_HOUSE){
		dice=roll_the_dice();
		if (dice < 4 && dice >0 ){
			current_player->account -= current_block->house_price;
			current_block->house_count ++;
			printf("Computer bought a house on %s!\n",current_block->name);
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

void sell_property( block *board, player *current_player){
	int i,choice,total;
	block *current_block;
	printf("\n**YOUR PROPERTIES**\n");
	printf("\nPlease select a property to sell:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		current_block=pointer_block_finder(board,i);
		if(current_block->owner.type==current_player->type){
			printf("%2d -\t%s\n",i,current_block->name);
		}
	}
	printf("\n%2d -\t%s\n",0,"Exit");
	do{
		printf("Choice:");
		scanf("%d",&choice);
		current_block=pointer_block_finder(board,choice);
		if(current_block->owner.type == current_player->type){
			total= current_block->price + (current_block->house_count * current_block->house_price);
			current_player->account += (total / 2 );
			current_block->house_count = 0;
			current_block->owner.type = noone;
			printf("\nThe property has been sold!You earned %d$ \n",(total/2));
		}
		else if(choice!=0 && current_block->owner.type != current_player->type){
			printf("Invalid Choice!\n");
		}
		printf("\nPlease select a property to sell:\n");
		for(i=0; i < MAX_BLOCK ; i++){
			if(current_block->owner.type==current_player->type){
				printf("%2d -\t%s\n",i,current_block->name);
			}
		}
		printf("\n%2d -\t%s\n",0,"Exit");
	}while(choice != 0);
}

void bankrupt_function( block *board, player *current_player){
	block *current_block;
	int flag=FALSE,i;
	printf("\n\n**\nYou are in debt!\nYou have to sell property.\nIf you cant afford the debt although you sold all of your properties , you will bankrupt at end of your turn.\n**\n ");
	while(current_player->account < 0 && flag== FALSE){
		sell_property(board,current_player);
		flag=TRUE;
		for(i=0; i <MAX_BLOCK ; i++){
			current_block=pointer_block_finder(board,i);
			if(current_block->owner.type == current_player->type ) flag=FALSE;
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

void show_properties(block *board){
	int i,choice;
	block current_block;
	printf("\nPlease select a property to see details:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		current_block=block_finder(board,i);
		if(current_block.type==property){
			printf("%2d -\t%s\n",i,current_block.name);
		}
	}
	printf("%2d -\t%s\n",0,"Exit");	
	do{
		printf("Choice:");
		scanf("%d",&choice);
		current_block=block_finder(board,choice);
		if(current_block.type==property){
			printf("---------------------------------\n");
			printf("|\t    %-20s|\n",  current_block.name);
			printf("---------------------------------\n");
			printf("|\t%-8s\t%-5d$  |\n", "Rent",current_block.rent);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 1 H",current_block.rent_1);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 2 H",current_block.rent_2);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 3 H",current_block.rent_3);
			printf("---------------------------------\n");
			printf("|\t%-11s     %-5d$  |\n", "House Price",current_block.price);
			printf("---------------------------------\n");
		}
		else if(current_block.type!=property && choice!=0){
			printf("Invalid Choice!\n");
		}
	}while(choice != 0);
} 

void show_my_properties(block *board, player current_player){
	int i,choice;
	block current_block;
	printf("\n**YOUR PROPERTIES**\n");
	printf("\nPlease select a property to see details:\n");
	for(i=0; i < MAX_BLOCK ; i++){
		current_block=block_finder(board,i);
		if(current_block.owner.type==current_player.type){
			printf("%2d -\t%s\n",i,current_block.name);
		}
	}
	printf("%2d -\t%s\n",0,"Exit");
	do{
		printf("Choice:");
		scanf("%d",&choice);
		current_block=block_finder(board,choice);
		if(current_block.type==property && current_block.owner.type==current_player.type){
			printf("---------------------------------\n");
			printf("|\t    %-20s|\n",  current_block.name);
			printf("---------------------------------\n");
			printf("|\t%-8s\t%-5d$  |\n", "Rent",current_block.rent);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 1 H",current_block.rent_1);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 2 H",current_block.rent_2);
			printf("|\t%-8s\t%-5d$  |\n", "Rent 3 H",current_block.rent_3);
			printf("---------------------------------\n");
			printf("|\t%-11s     %-5d$  |\n", "House Price",current_block.price);
			printf("---------------------------------\n");
		}
		else if(choice!=0 && current_block.owner.type!=current_player.type){
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

void init_fortune_cards(fortune_card *cards){
	
	cards->name="Free House";
	cards->card=free_house;
	cards->next_card=(fortune_card*)malloc(sizeof(fortune_card) + (sizeof(char)*20));
	cards=cards->next_card;
	
	cards->name="Time Travel ";
	cards->card=time_travel;
	cards->next_card=(fortune_card*)malloc(sizeof(fortune_card) + (sizeof(char)*20));
	cards=cards->next_card;
	
	cards->name="Garnishment";
	cards->card=garnishment;
	cards->next_card=(fortune_card*)malloc(sizeof(fortune_card) + (sizeof(char)*20));
	cards=cards->next_card;
	
	cards->name="Generosity";
	cards->card=generosity;
	cards->next_card=(fortune_card*)malloc(sizeof(fortune_card) + (sizeof(char)*20));
	cards=cards->next_card;
	
	cards->name="Treasure Hunter";
	cards->card=treasure_hunter;
	cards->next_card=NULL;
}

void init_the_players(player *player_one,player *player_two){
	int choice,i ;	
	printf("\nYou are  Car and computer is Cap\n\n");
	player_one->type=car;
	player_two->type=cap;
	player_one->name="Car";
	player_two->name="Cap";
	
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



void show_board( block *board_head, player player_one, player player_two){
	int i,k,l;
	int block_id_left,block_id_right;
	block current_block,current_block2;
	
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
				current_block=block_finder(board_head , k);
				printf("    %-16s",current_block.name);
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){ // printing prices. there are ifs because we put the prices of texes into .rent(as in excel file). So i need to do that.
				current_block=block_finder(board_head , k);
				if(current_block.type == property ){
					printf("    %-6d$         ",current_block.price); 
				}
				else if(current_block.type == tax){
					printf("    %-6d$         ",current_block.rent);
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
				current_block=block_finder(board_head , MAX_BLOCK - i - k);
				printf("    %-16s",current_block.name);
				printf("|");
			}
			printf("\n");
			printf("|");
			for(k=0 ; k < COLUMN ; k++){
				current_block=block_finder(board_head , MAX_BLOCK - i - k);
				if(current_block.type == property ){
					printf("    %-6d$         ",current_block.price);
				}
				else if(current_block.type == tax){
					printf("    %-6d$         ",current_block.rent);
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
			current_block=block_finder(board_head , block_id_left);
			current_block2=block_finder(board_head , block_id_right);
			
			printf("|    %-16s|",current_block.name);
			
			
			for(k=0; k < ((COLUMN-2) * BLOCK_LENGTH) + COLUMN - 3 ; k++ ){  // spece( ) loop for center of board.
				printf(" ");
			}
			
			
			printf("|    %-16s|\n|",current_block2.name);
			
			
			if(current_block.type == property ){
					printf("    %-6d$         ",current_block.price);
				}
			else if(current_block.type == tax){
					printf("    %-6d$         ",current_block.rent);
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
			if(current_block2.type == property ){
				printf("    %-6d$         ",current_block2.price);
			}
			else if(current_block2.type == tax){
					printf("    %-6d$         ",current_block2.rent);
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



void init_the_board(block *board){
	board->name="Start";
	board->block_id=0 ;
	board->price=0 ;
	board->rent=0 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=start ;
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Esenyurt";
	board->block_id=1 ;
	board->price=16000 ;
	board->rent=800 ;
	board->rent_1=4000 ;
	board->rent_2=9000 ;
	board->rent_3=25000 ;
	board->house_price=10000 ;
	board->house_count=0 ;
	board->type=property ; 
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Car Park";
	board->block_id=2 ;
	board->price=0 ;
	board->rent=1500 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=tax ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Fortune Card";
	board->block_id=3 ;
	board->price=0 ;
	board->rent=0 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=fortune ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Tuzla";
	board->block_id=4 ;
	board->price=16500 ;
	board->rent=850 ;
	board->rent_1=4250 ;
	board->rent_2=9500 ;
	board->rent_3=26000 ;
	board->house_price=12000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Arnavutkoy";
	board->block_id=5 ;
	board->price=17000 ;
	board->rent=875 ;
	board->rent_1=4500 ;
	board->rent_2=10000 ;
	board->rent_3=28000 ;
	board->house_price=12000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Wait 2 Turn";
	board->block_id=6 ;
	board->price=0 ;
	board->rent=2 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=punish ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;

	board->name="Catalca";
	board->block_id=7 ;
	board->price=20000 ;
	board->rent=1000 ;
	board->rent_1=5000 ;
	board->rent_2=12000 ;
	board->rent_3=30000 ;
	board->house_price=13000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Beykoz";
	board->block_id=8 ;
	board->price=23000 ;
	board->rent=1100 ;
	board->rent_1=5500 ;
	board->rent_2=12500 ;
	board->rent_3=33000 ;
	board->house_price=13000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone;
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Fortune Card";
	board->block_id=9 ;
	board->price=0 ;
	board->rent=0 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=fortune ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Car Fix";
	board->block_id=10 ;
	board->price=0 ;
	board->rent=1750 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=tax ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Maltepe";
	board->block_id=11 ;
	board->price=30000 ;
	board->rent=1350 ;
	board->rent_1=7000 ;
	board->rent_2=15000 ;
	board->rent_3=40000 ;
	board->house_price=15000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Bills";
	board->block_id=12 ;
	board->price=0 ;
	board->rent=2000 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=tax ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Sisli";
	board->block_id=13 ;
	board->price=33000 ;
	board->rent=1500 ;
	board->rent_1=8000 ;
	board->rent_2=16000 ;
	board->rent_3=42000 ;
	board->house_price=16000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Oil";
	board->block_id=14 ;
	board->price=0 ;
	board->rent=2250 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=tax ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Fortune Card";
	board->block_id=15 ;
	board->price=0 ;
	board->rent=0 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=fortune ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Atasehir";
	board->block_id=16 ;
	board->price=35000 ;
	board->rent=1600 ;
	board->rent_1=8500 ;
	board->rent_2=17000 ;
	board->rent_3=45000 ;
	board->house_price=17000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Sariyer";
	board->block_id=17 ;
	board->price=40000 ;
	board->rent=1750 ;
	board->rent_1=9500 ;
	board->rent_2=19000 ;
	board->rent_3=48000 ;
	board->house_price=19000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Wait 1 Turn";
	board->block_id=18 ;
	board->price=0 ;
	board->rent=1 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=punish ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Kadikoy";
	board->block_id=19 ;
	board->price=43000 ;
	board->rent=1900 ;
	board->rent_1=11000 ;
	board->rent_2=21500 ;
	board->rent_3=55000 ;
	board->house_price=23000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Besiktas";
	board->block_id=20 ;
	board->price=60000 ;
	board->rent=2500 ;
	board->rent_1=15000 ;
	board->rent_2=28000 ;
	board->rent_3=60000 ;
	board->house_price=30000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Fortune Card";
	board->block_id=21 ;
	board->price=0 ;
	board->rent=0 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=fortune ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Vocation";
	board->block_id=22 ;
	board->price=0 ;
	board->rent=5000 ;
	board->rent_1=0 ;
	board->rent_2=0 ;
	board->rent_3=0 ;
	board->house_price=0 ;
	board->house_count=0 ;
	board->type=tax ;  
	board->owner.type=noone; 
	board->next=(block*)malloc(sizeof(block) + (sizeof(char)*20));
	board=board->next;
	
	board->name="Bebek";
	board->block_id=23 ;
	board->price=70000 ;
	board->rent=3500 ;
	board->rent_1=20000 ;
	board->rent_2=35500 ;
	board->rent_3=65000 ;
	board->house_price=35000 ;
	board->house_count=0 ;
	board->type=property ;  
	board->owner.type=noone; 
	board->next=NULL;
}

block block_finder(block *board_head , int id ){
	int i;
	block *current_block;
	current_block=board_head;
	while(current_block->block_id != id){
		current_block=current_block->next;	
	}
	return *current_block;
}
block *pointer_block_finder(block *board_head , int id ){
	int i;
	block *current_block;
	current_block=board_head;
	while(current_block->block_id != id){
		current_block=current_block->next;	
	}
	return current_block;
}
