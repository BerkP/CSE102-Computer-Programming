#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#define COMPUTER_WON 0
#define USER_WON 1
#define MAX_TRIAL 10

void menu();
int game_one();
void game_two();
void game_three();
void show_score(int score_human,int score_program);
int make_a_guess(int trial,int min,int max);
void draw_hourglass (int height);
void draw_mountain_road (int length, int max_radius);
int distance_calculator(int difference);

int main(){	
	menu();
	return 0;
}

void menu(){
	srand(time(NULL));
	int choice,flag;
	int game_result,score_human,score_program;	
	score_human=0;
	score_program=0;
	flag=0;  /* flag for ending the menu while */
	while(flag==0){
		printf("\e[2J");
		printf("***** MENU *****\n1. Play Lucky Number\n2. Draw Hourglass\n3. Draw Mountain Road\n4. Exit\nChoice: ");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				game_result=game_one();
				if(game_result==COMPUTER_WON){	/* Storing the scores here */
					score_program += 1;
				}
				else{
					score_human += 1;
				}
				show_score(score_human,score_program);
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 2:
				game_two();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 3:
				game_three();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 4:
				printf("\n\nExiting...");
				flag=1;
				break;
			default:
				printf("\nThis is an invalid choice.Try again!\n");		
		}
	}
}

int game_one(){
	int trial,min,max,result,lucky_number,guess,distance,difference;
	lucky_number=rand()%1025; /* its "1025" because we want "1024" included */
	result= COMPUTER_WON; /*	default situation */
	trial=1;
	min=0;
	max=1024;
	while(trial <= MAX_TRIAL && result != USER_WON){
		guess=make_a_guess(trial,min,max);
		if(guess==lucky_number){
			result= USER_WON;
			printf("Distance: 0. You win!");
		}
		else{
			difference=lucky_number-guess;
			distance=distance_calculator(difference);
			printf("Distance: %d.",distance);
			trial +=1;
			if(guess < lucky_number){
				min=guess;
			}
			else{
				max=guess;
			}
		}
	}
	return result;
}

void game_two(){
	int height;
	do{
		printf("Plese enter positive odd number ");
		scanf("%d",&height);
		if(height%2==0 || height <0){
			printf("This number is not positive odd number. \n");
		}
		else{
			draw_hourglass(height);
		}
	}while(height%2==0 || height <0);
}

void game_three(){
	int max_radius,length;
	printf("Please enter the length ");
	scanf("%d",&length);
	printf("Please enter the maximum radius ");
	scanf("%d",&max_radius);
	draw_mountain_road(length,max_radius);
}

void show_score(int score_human,int score_program){
	printf("\nYour score: %d Program Score: %d",score_human,score_program);	
}

int make_a_guess(int trial,int min,int max){
	int guess_number;
	do{
		printf("\n(Trial: %d) Make a guess between %d and %d :",trial,min,max);
		scanf("%d",&guess_number);	
	}while(guess_number>max || guess_number<min);
	return guess_number;
}

void draw_hourglass (int height){
	int i,k,topside_h,botside_h;
	topside_h=(height+1)/2;		/*top side height*/
	botside_h=(height-1)/2;		/*bot side height*/
	for(i=0 ; i<topside_h; i++){ 	/*loop for top side (middle point included)*/
		for(k=0 ; k<i ; k++){
			printf(" ");
		}
		for(k=0 ; k<2*(topside_h-i)-1 ; k++){
			printf("*");
		}
		printf("\n");
	}
	for(i=1 ; i<=botside_h; i++){	/*loop for bot side*/
		for(k=0 ; k<(botside_h)-i ; k++){
			printf(" ");
		}
		for(k=0 ; k<2*(i)+1 ; k++){
			printf("*");
		}
		printf("\n");
	}
}

 void draw_mountain_road (int length, int max_radius){
	int i,k,length_counter,space_stack,random_r;
	space_stack=max_radius;
	for(length_counter=1 ; length_counter<=length ; length_counter++){
		random_r=rand()%(max_radius+1);
		if(length_counter%2==1){ 
			for(i=1 ; i<=random_r ; i++){ /* Loop for top side of the half circle */
				for(k=1 ; k<=space_stack ; k++){
					printf(" ");
				}
				printf("/");
				printf("\n");
				space_stack = space_stack - 1;
			}
			for(k=1 ; k<=space_stack ; k++){
				printf(" ");
			}
			if(random_r!=0){
			space_stack +=1;
			}
			printf("|\n");
			for(i=1 ; i<=random_r ; i++){	/* Loop for bottom side of the half circle */
				for(k=1 ; k<=space_stack ; k++){
					printf(" ");
				}
				printf("\\");
				printf("\n");
				space_stack = space_stack + 1;
			}	
		}
		else{
			for(i=1 ; i<=random_r ; i++){	/* Loop for top side of the half circle */
				for(k=1 ; k<=space_stack ; k++){ 
					printf(" ");
				}
				printf("\\");
				printf("\n");
				space_stack = space_stack + 1;
			}
			for(k=1 ; k<=space_stack ; k++){
				printf(" ");
			}
			if(random_r!=0){
				space_stack -=1;
			}
			printf("|\n");
			for(i=1 ; i<=random_r ; i++){	/* Loop for bottom side of the half circle */
				for(k=1 ; k<=space_stack ; k++){
					printf(" ");
				}
				printf("/");
				printf("\n");
				space_stack = space_stack - 1;
			}
		}	
	}	
}

int distance_calculator(int difference){
	int distance=0;
	if(difference <0){
		difference=difference*(-1);
	}
	do{
		distance+=1;	
	}while(difference>=pow(2,distance));
	return distance;
}























