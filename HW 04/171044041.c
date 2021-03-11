#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRUE 1
#define FALSE 0
#define EMPTY -1
#define MAX_POCKET 4
#define MAX_POKEMON 10
#define USER 0
#define PC 1
#define MAX_MOVE 2

typedef enum pocketmonsters{
	Charmander,Pikachu,Squirtle,Bulbasaur,Pidgeotto,Ratata,Mug,Caterpie,Zubat,Krabby
}pokemon;
typedef enum attacktype{
	linear,quadratic
}attack_type;

void pokedex(char Pokemon_name[10][11],int range[],attack_type type[],int attack_power[],int stamina[]);
void oaks_laboratory(char Pokemon_name[10][11], pokemon Pokemons[],pokemon *my_Pokemons);
void show_Pokemons(char Pokemon_name[10][11], pokemon Pokemons[], int pokemon_count);
void catch_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket);
void release_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket);
void battle(char Pokemon_name[10][11], int range[], attack_type type[], int attack_power[], int stamina[], pokemon user_Pokemons[]);
void show_area (char Pokemon_name[10][11],int area[8][8],int pokemon_staminas_view[8][8]);
void attack(int range[], attack_type type[], int attack_power[],int pokemon_staminas_view[8][8],int enemy,int team_areas[2][8][8],int x,int y,int area[8][8],int number_of_pokemons[2]);
void move_user(int *x,int *y);
void move_pc(int *x,int *y);
int string_equality_checker(char x[15],char s[10]);

int main(){
	int flag=1,choice;
	char Pokemon_name[10][11]={"Charmander","Pikachu","Squirtle","Bulbasaur","Pidgeotto","Ratata","Mug","Caterpie","Zubat","Krabby"};
	int range[10]={1,3,4,3,2,2,1,2,3,2};
	int attack_power[10]={500,350,300,400,250,250,350,200,350,300};
	int stamina[10]={2200,1500,1700,2500,1900,2500,3000,1200,1250,2600};
	attack_type type[10]={quadratic,linear,linear,linear,quadratic,linear,quadratic,quadratic,linear,linear};
	pokemon my_Pokemons[4]={EMPTY,EMPTY,EMPTY,EMPTY};
	pokemon Pokemons[10]={Charmander,Pikachu,Squirtle,Bulbasaur,Pidgeotto,Ratata,Mug,Caterpie,Zubat,Krabby};
	
	srand(time(NULL));
	
	while(flag==1){
		printf("\e[2J");
		printf("Please select an option to continue:\n1) Open Pokedex\n2) Go to Oaks Laboratory\n3) Enter the tournament \n4) Exit\nChoice: ");
		scanf("%d",&choice);
		if(choice==1){
			pokedex(Pokemon_name,range,type,attack_power,stamina);
		}
		else if(choice==2){
			oaks_laboratory(Pokemon_name,Pokemons,my_Pokemons);
		}
		else if(choice==3){
			battle(Pokemon_name,range,type,attack_power,stamina,my_Pokemons);
		}
		else if(choice==99){
			flag=0;
		}
		else{
			printf("\nThis is an invalid choice.Try again!\n");		
		}
	}
	return 0;
}

void pokedex(char Pokemon_name[10][11],int range[],attack_type type[],int attack_power[],int stamina[]){
	int flag1=TRUE,flag2,flag3;
	int selected_pokemon,x;
	char typed_pokemon[15];
	printf("\e[2J");
	while(flag1==TRUE){
		flag3=TRUE;
		flag2=TRUE;
		while(flag2==TRUE){
			flag2=FALSE;
			printf("\nPlease type name of the Pokemon (type exit to close Pokedex):\n");
			scanf("%s",typed_pokemon);
			if(string_equality_checker(typed_pokemon,Pokemon_name[0])==TRUE) selected_pokemon=0;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[1])==TRUE) selected_pokemon=1;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[2])==TRUE) selected_pokemon=2;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[3])==TRUE) selected_pokemon=3;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[4])==TRUE) selected_pokemon=4;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[5])==TRUE) selected_pokemon=5;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[6])==TRUE) selected_pokemon=6;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[7])==TRUE) selected_pokemon=7;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[8])==TRUE) selected_pokemon=8;
			else if(string_equality_checker(typed_pokemon,Pokemon_name[9])==TRUE) selected_pokemon=9;
			else if(string_equality_checker(typed_pokemon,"exit")==TRUE){
				flag1=FALSE;
				flag3=FALSE;
			}
			else{
				printf("Pokemon cannot found!");
				flag2=TRUE;
			}
		}
		while(flag3==TRUE){
			printf("\nName: %s \n",Pokemon_name[selected_pokemon]);	
			if(type[selected_pokemon]==quadratic){
				printf("A. Type: Quadratic\n");
			}
			else{
				printf("A. Type: Linear\n");
			}
			printf("Range: %d block\nA. Power: %d\nStamina: %d\n",range[selected_pokemon],attack_power[selected_pokemon],stamina[selected_pokemon]);
			
			flag3=0;
		}
	}	
}

void oaks_laboratory(char Pokemon_name[10][11], pokemon Pokemons[],pokemon *my_Pokemons){
	int pokemon_count=MAX_POKEMON,pocket_count=MAX_POCKET,flag=TRUE,choice; 							/* !! */
	while(flag==TRUE){
		printf("\e[2J");
		printf("\nWelcome to Laboratory of Professor Oak. How can I help you?\n1) Show Pokemons\n2) Catch a Pokemon\n3) Release a Pokemon\n4) Show my pocket\n5) Back\nChoice:");
		scanf("%d",&choice);
		if(choice==1){
			show_Pokemons(Pokemon_name,Pokemons,pokemon_count);
			printf("\n\nPlease press ENTER to back to laboratory");
			getchar();
			getchar();
		}
		else if(choice==2){
			show_Pokemons(Pokemon_name,Pokemons,pokemon_count);
			catch_a_pokemon(Pokemon_name,Pokemons,my_Pokemons);
			printf("\n\nPlease press ENTER to back to laboratory");
			getchar();
			getchar();	
		}
		else if(choice==3){
			show_Pokemons(Pokemon_name,my_Pokemons,pocket_count);
			release_a_pokemon(Pokemon_name,Pokemons,my_Pokemons);
			printf("\n\nPlease press ENTER to back to laboratory");
			getchar();
			getchar();	
		}
		else if(choice==4){
			show_Pokemons(Pokemon_name,my_Pokemons,pocket_count);
			printf("\n\nPlease press ENTER to back to laboratory");
			getchar();
			getchar();
			
		}
		else if(choice==5){
			flag=FALSE;
		}
		else{
			printf("Invalid choice! Try again!");
		}
	}
}

void show_Pokemons(char Pokemon_name[10][11], pokemon Pokemons[],int pokemon_count){
	int i;
	printf("\n");
	for(i=0;i<pokemon_count;i++){
		if(Pokemons[i]>=MAX_POKEMON || Pokemons[i]<0){
			printf("%d - EMPTY\n",i);
		}
		else{
			printf("%d - %s\n",i,Pokemon_name[Pokemons[i]]);
		}
	}
}

void catch_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket){
	int choice,flag,available_slot,cath_status,i,k;
		flag=TRUE;
		cath_status=FALSE;
		do{
			printf("Please enter ID for Pokemon you want to cath.Be sure that entered id is one of them above \n");
			scanf("%d",&choice);
		}while(choice>=MAX_POKEMON || choice < 0);
		for(i=0;i<MAX_POCKET;i++){                                              
			if(my_pocket[i]==Pokemons[choice]){
				flag=FALSE;
			}
			if(my_pocket[i]==EMPTY){
				available_slot=i;
				cath_status=TRUE;	
			}
		}
		if(flag==TRUE && cath_status==TRUE){
			my_pocket[available_slot]=Pokemons[choice];
			printf("Pokemon cathed!");
		}
		else{
			printf("\nYou cannot catch a Pokmon more than once and cannot catch more than four Pokmons.\n");
		}	
}

void release_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket){
	int choice,flag,available_slot,cath_status,i,k;
	do{
		printf("Please enter ID for Pokemon you want to release.Be sure that entered id is one of them above \n");
		scanf("%d",&choice);
	}while(choice>=MAX_POCKET || choice < 0);
	if(my_pocket[choice]!=EMPTY){
		printf("Pokemon released!");
		my_pocket[choice]=EMPTY;
	}
	else{
		printf("The slot you selected is already empty!");
	}		
}

void battle(char Pokemon_name[10][11], int range[], attack_type type[], int attack_power[], int stamina[], pokemon user_Pokemons[]){
	int i,k,x,temp_x,y,temp_y,rand_pokemon,pc_pokemons[MAX_POCKET],pc_pokemons_x[MAX_POCKET],pc_pokemons_y[MAX_POCKET],random,enemy;
	int game_over=FALSE;
	int turn=USER;
	int area[8][8]={{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},  /* Making area full empty before placements */
	{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
	{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
	{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}};
	int pokemon_staminas_view[8][8];
	int team_areas[2][8][8];
	int number_of_pokemons[2]={MAX_POCKET,MAX_POCKET};
	printf("\e[2J");
	for(i=0 ; i<MAX_POCKET ; i++){  /* Checking the number of user's pokemons */
		if(user_Pokemons[i]==EMPTY){
			printf("\nYou need four pokemon to join battle!You can cath pokemons from Oaks Lab!\nPress ENTER to back to menu!\n");
			getchar();
			getchar();
			return;
		}
	}
	printf("You need to locate your pokemons on battle field. \n");
	for(i=0;i<MAX_POCKET;i++){				/* Placement loop for user */
		printf("Please enter the starting location for %s !\n",Pokemon_name[user_Pokemons[i]]);
		do{
			printf("Your starting locations can be Y= 0 or 1 and X= 0/1/2/3/4/5/6/7\nX="); 
			scanf("%d",&x);
			printf("Y=");
			scanf("%d",&y);
			if(area[x][y]!=EMPTY){	
				printf("Another pokemon is already there or out of area! Please choose another location!\n");
			}
		}while((y!=0 && y!=1) || area[x][y]!=EMPTY); /* Checking that is location available */
		
		area[x][y]=user_Pokemons[i];								/* Placement */
		pokemon_staminas_view[x][y]=stamina[user_Pokemons[i]];		/* Placement */
		team_areas[USER][x][y]=user_Pokemons[i];					/* Placement */
		printf("Pokemon located!\n\n");
	} 
	for(i=0;i<MAX_POCKET;i++){										/* Picking random pokemons and placements loop*/
		rand_pokemon=rand()%MAX_POKEMON;
		do{
			x=rand()%8; /* x can be max 7 */
			y=(rand()%2)+6; /* y can be 6 or 7 */
		}while((y!=7 && y!=6) || area[x][y]!=EMPTY);				/* Checking that is location available */ 
		pc_pokemons[i]=rand_pokemon;
		pc_pokemons_x[i]=x;											 /*I need this for random pokemon */
		pc_pokemons_y[i]=y;											/*I need this for random pokemon */
		area[x][y]=rand_pokemon;									/* Placement */
		pokemon_staminas_view[x][y]=stamina[rand_pokemon];			/* Placement */
		team_areas[PC][x][y]=rand_pokemon;							/* Placement */
	}
	while(game_over!=TRUE ){  										/* General game loop until game over */ 
		show_area (Pokemon_name,area,pokemon_staminas_view);		/* Showing area before the turn */

		if(turn==USER){
			enemy=PC;
			printf("Please enter the location of the pokemon you want to move");
			do{														/*	Slecting pokemon to move  */
				printf("X=");
				scanf("%d",&x);
				printf("Y=");
				scanf("%d",&y);
				if(team_areas[USER][x][y]!=area[x][y] || area[x][y]==EMPTY){
					printf("You connot select this location! Please select your Pokemon.");
				}
			}while(team_areas[USER][x][y]!=area[x][y] || area[x][y]==EMPTY);	/* Checking that is there any pokemon */	
			printf("Now you can move your pokemon!\n");
			do{																	/* Moving */
				temp_x=x;
				temp_y=y;
				move_user(&temp_x,&temp_y);
				if(area[temp_x][temp_y]!=EMPTY){									/* Error message condition */
					printf("Another pokemon is already there or out of area! Please choose another location!\n");
				}
			}while(area[temp_x][temp_y]!=EMPTY && area[temp_x][temp_y]!=area[x][y]);	/* Checking that is there any pokemon blocks */
			if(area[temp_x][temp_y]!=area[x][y]){
				area[temp_x][temp_y]=area[x][y];										/* to swap locations */
				area[x][y]=EMPTY;														/* to swap locations */
				pokemon_staminas_view[temp_x][temp_y]=pokemon_staminas_view[x][y];		/* to swap locations */
				pokemon_staminas_view[x][y]=EMPTY;										/* to swap locations */
				team_areas[USER][temp_x][temp_y]=team_areas[USER][x][y];				/* to swap locations */
				team_areas[USER][x][y]=EMPTY;											/* to swap locations */
			}
			x=temp_x;
			y=temp_y;
			attack(range,type,attack_power,pokemon_staminas_view,enemy,team_areas,x,y,area,number_of_pokemons);
			if(number_of_pokemons[PC]==0){
				game_over=TRUE;
				printf("\n\nGame Over! You won the game!\nPress enter to back to menu!\n\n");
			}
			turn=PC;
		} 
		else if(turn==PC){
			enemy=USER;
			do{					/* Selecting random pokemon to move */
				rand_pokemon=rand()%MAX_POCKET;  /* choosing one of the pc's pokemons */
				x=pc_pokemons_x[rand_pokemon];
				y=pc_pokemons_y[rand_pokemon];
				printf("\n%s has choosen by program.\n",Pokemon_name[team_areas[PC][x][y]]);
			}while(team_areas[PC][x][y]!=area[x][y] || area[x][y]==EMPTY);	
			do{																		/* Random moving(random selector is in the move_pc function) */
				temp_x=x;
				temp_y=y;
				move_pc(&temp_x,&temp_y);
			}while(area[temp_x][temp_y]!=EMPTY && area[temp_x][temp_y]!=area[x][y]);
			if(area[temp_x][temp_y]!=area[x][y]){
				area[temp_x][temp_y]=area[x][y];										/* to swap locations */
				area[x][y]=EMPTY;														/* to swap locations */
				pokemon_staminas_view[temp_x][temp_y]=pokemon_staminas_view[x][y];		/* to swap locations */
				pokemon_staminas_view[x][y]=EMPTY;										/* to swap locations */
				team_areas[PC][temp_x][temp_y]=team_areas[PC][x][y];				/* to swap locations */
				team_areas[PC][x][y]=EMPTY;											/* to swap locations */
				pc_pokemons_x[rand_pokemon]=temp_x;
				pc_pokemons_y[rand_pokemon]=temp_y;
			}
			x=temp_x;
			y=temp_y;
			attack(range,type,attack_power,pokemon_staminas_view,enemy,team_areas,x,y,area,number_of_pokemons);
			if(number_of_pokemons[USER]==0){
				game_over=TRUE;
				printf("\n\nGame Over! Program won the game!\nPress enter to back to menu!\n\n");
			}
			turn=USER;
		} 
	}
}

void attack(int range[], attack_type type[], int attack_power[],int pokemon_staminas_view[8][8],int enemy,int team_areas[2][8][8],int x,int y,int area[8][8],int number_of_pokemons[2]){
	int i,k,x_range_min,x_range_max,y_range_min,y_range_max,flag=FALSE,temp_x,temp_y;
	
	if(type[area[x][y]]==linear){ 
		for(i=1 ; i <= range[area[x][y]] && flag==FALSE ; i++){			/* Starting from range 1 and goes.if attack then flag becomes true and loop will over*/
			if(area[x+i][y]!=EMPTY && area[x+i][y]==team_areas[enemy][x+i][y] && x+i <= 7){			/* 7 means max location for x */
				pokemon_staminas_view[x+i][y]-=attack_power[area[x][y]];
				flag=TRUE;
				if(pokemon_staminas_view[x+i][y] <= 0){
					team_areas[enemy][x+i][y]=EMPTY;
					area[x+i][y]=EMPTY;
					number_of_pokemons[enemy]-=1;
				}
			}
			if(area[x-i][y]!=EMPTY && area[x-i][y]==team_areas[enemy][x-i][y] && x-i >= 0){			/* 7 means max location for x */
				pokemon_staminas_view[x-i][y]-=attack_power[area[x][y]];
				flag=TRUE;
				if(pokemon_staminas_view[x-i][y] <= 0){
					team_areas[enemy][x-i][y]=EMPTY;
					area[x-i][y]=EMPTY;
					number_of_pokemons[enemy]-=1;
				}
			}
			if(area[x][y+i]!=EMPTY && area[x][y+i]==team_areas[enemy][x][y+i] && y+i <= 7){			/* 7 means max location for x */
				pokemon_staminas_view[x][y+i]-=attack_power[area[x][y]];
				flag=TRUE;
				if(pokemon_staminas_view[x][y+i] <= 0){
					team_areas[enemy][x][y+i]=EMPTY;
					area[x][y+i]=EMPTY;
					number_of_pokemons[enemy]-=1;
				}
			}
			if(area[x][y-i]!=EMPTY && area[x][y-i]==team_areas[enemy][x][y-i] && y-i >= 0){			/* 7 means max location for x */
				pokemon_staminas_view[x][y-i]-=attack_power[area[x][y]];
				flag=TRUE;
				if(pokemon_staminas_view[x][y-i] <= 0){
					team_areas[enemy][x][y-i]=EMPTY;
					area[x][y-i]=EMPTY;
					number_of_pokemons[enemy]-=1;
				}
			}
		}
	}
	else if(type[area[x][y]]==quadratic){
		x_range_min=x-range[area[x][y]];
		x_range_max=x+range[area[x][y]];
		y_range_min=y-range[area[x][y]];
		y_range_max=y+range[area[x][y]];
		if(x_range_min < 0) x_range_min=0;
		if(x_range_max > 7) x_range_max=7;
		if(y_range_min < 0) y_range_min=0;
		if(y_range_max > 7) y_range_max=7;
		for(i=y_range_min ; i<=y_range_max ; i++){
			for(k=x_range_min ; k <=x_range_max ; k++){
				if(area[k][i]!=EMPTY && area[k][i]==team_areas[enemy][k][i]){
					pokemon_staminas_view[k][i]-=attack_power[area[x][y]];
					if(pokemon_staminas_view[k][i] <= 0){
						team_areas[enemy][k][i]=EMPTY;
						area[k][i]=EMPTY;
						number_of_pokemons[enemy]-=1;
					}
				}
			}
		}
	}
}


void move_pc(int *x,int *y){
	int choice,flag=TRUE,temp_x,temp_y;
	int move_ways[9]={8,2,6,4,5,66,44,88,22};  /* I need this for random move. (up down right...) */
	while(flag==TRUE){
		flag=FALSE;
		temp_x=*x;
		temp_y=*y;
		choice=move_ways[rand()%9]; /* there are 9 way */
		if(choice==8)		temp_y-=1;	
		else if(choice==2)	temp_y+=1;
		else if(choice==6)	temp_x+=1;
		else if(choice==4)	temp_x-=1;
		else if(choice==5)	temp_x=*x;
		else if(choice==88)	temp_y-=2;
		else if(choice==22)	temp_y+=2;
		else if(choice==66)	temp_x+=2;
		else if(choice==44)	temp_x-=2;
		else{
			flag=TRUE;
		}
		if(temp_x>7 || temp_x<0 || temp_y>7 || temp_y<0){
			temp_x=*x;
			temp_y=*y;
			flag=TRUE;
		}
		if(flag==FALSE){
			*x=temp_x;
			*y=temp_y;
		}
	}
}

void move_user(int *x,int *y){
	int choice,flag=TRUE,temp_x,temp_y;
	while(flag==TRUE){
		flag=FALSE;
		temp_x=*x;
		temp_y=*y;
		printf("\nUP 8 / DOWN 2 / RIGHT 6 / LEFT 4 / DONT MOVE 5 / Enter twice to move two unit(e.g: 66 > Two times right).\nChoice=");
		scanf("%d",&choice);
		if(choice==8)		temp_y-=1;	
		else if(choice==2)	temp_y+=1;
		else if(choice==6)	temp_x+=1;
		else if(choice==4)	temp_x-=1;
		else if(choice==5)	temp_x=*x;
		else if(choice==88)	temp_y-=2;
		else if(choice==22)	temp_y+=2;
		else if(choice==66)	temp_x+=2;
		else if(choice==44)	temp_x-=2;
		else{
			flag=TRUE;
			printf("Invalid choice!\n");
		}
		if(temp_x>7 || temp_x<0 || temp_y>7 || temp_y<0){
			flag=TRUE;
			printf("Out of area!\n");
		}
		else{
			*x=temp_x;
			*y=temp_y;
		}
	}
}

void show_area (char Pokemon_name[10][11],int area[8][8],int pokemon_staminas_view[8][8]){
	int x,y;
	for(y=0;y<8;y++){			 /* 8 is max row */
		for(x=0;x<8;x++){			
			printf("-----------");
		}
		printf("\n");
		for(x=0;x<8;x++){ 		/* 8 is max column */
			if(area[x][y]!=EMPTY){
				printf("|    %c%c    ",Pokemon_name[area[x][y]][0],Pokemon_name[area[x][y]][1]); /* printing first two chars */
			}
			else{
				printf("|          ");
			}
		}
		printf("|\n");
		for(x=0;x<8;x++){ 		/* 8 is max column */
			if(area[x][y]!=EMPTY){
				printf("|%6d    ",pokemon_staminas_view[x][y]); /* printing stamina */
			}
			else{
				printf("|          ");
			}
		}
		printf("|\n");
	}
	for(x=0;x<8;x++){			
			printf("-----------");
	}
	printf("\n");	
}

int string_equality_checker(char string_1[15],char string_2[10]){
	int i,string_length,common_length;
	string_length=0;
	common_length=0;
	for(i=0;string_1[i]!=0;i++){
		string_length++;
	}
	for(i=0;string_1[i]==string_2[i] && i<string_length;i++){
		common_length++;
	}
	if(string_length==common_length){
		return TRUE;
	}
	else{
		return FALSE;
	}		
}
