#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define PI 3
#define SQUARE 1
#define RECTANGULAR 2
#define CIRCULAR 3
#define RED 0
#define YELLOW 1
#define BLUE 2
#define BLACK 3
#define WHITE 4

double CreateBody (int shape);
int SetColor (int color);
double LoadMoves(int shape,double body_size);
int SetAttackPower(int lower_bound, int upper_bound);
void ShowPokemon(int shape, double body_size, int color, double move_length, int attack_power);
void circle(int radius);

int main (){
	int shape, color, attack_power;
	double size_of_body, move_length;
 	shape=CIRCULAR;
 	size_of_body = CreateBody (shape);
 	color = 798;
 	color = SetColor(color);
 	move_length = LoadMoves(shape, size_of_body);
	attack_power = SetAttackPower (0, 150);
	ShowPokemon(shape, size_of_body, color, move_length, attack_power);
	
	return 0;
}


double CreateBody (int shape){
	double length_1,length_2;
	if(shape==1){        /* CALCULATION FOR SQUARE */
		printf("\n Please enter the length of the edge:");
		scanf("%lf",&length_1);
		return (length_1*length_1);
	}
	if(shape==2){        /* CALCULATION FOR RECTANGULAR */
		printf("\n Please enter the length of the edges:");
		scanf("%lf",&length_1);
		scanf("%lf",&length_2);
		return (length_1*length_2);
	}
	if(shape==3){        /* CALCULATION FOR CIRCULAR */
		printf("\n Please enter the length of the radius:");
		scanf("%lf",&length_1);
		return (length_1*length_1*(PI));
	}
}


int SetColor (int color){
	if ( color > 1000 && color > (-1) ){
		return 1;
	}
	else{
		return (color%5);
	}
}

double LoadMoves(int shape,double body_size){
	double length_1;
	if(shape==1){
		length_1=sqrt(body_size);
		return (4*length_1);
	}
	
	if(shape==2){
		length_1=body_size/5;
		return (10 + (2*length_1));
	}
	if(shape==3){
		length_1=sqrt(body_size / (PI));
		return (2*(PI)*length_1);
	}
}

int SetAttackPower(int lower_bound, int upper_bound){
	int difference,random;
	difference= upper_bound - lower_bound ;
	srand(time(NULL)); //RNG
	random=rand(); 	   //RNG
	random= random%difference;
	return (lower_bound + random);
}

void ShowPokemon(int shape, double body_size, int color, double move_length, int attack_power){
	
	int i,k;
	int length_1;
	printf("\n\n");
	switch(shape) {
		case 1 :
			length_1=sqrt(body_size);
			for(i=0;i<length_1;i++){			/*loop for line number */
				for(k=0;k<length_1;k++){		//loop for unit of line number */ 
					printf("X");
				}
				printf("\n");
			}
			printf("Name : Square Pokemon \n");
			break;
		case 2 :
			length_1=body_size/5;
			for(i=0;i<length_1;i++){ 			//loop for line number */
				for(k=0;k<5;k++){				//loop for unit of line number */
					printf("X");
				}
				printf("\n");
			}
			printf("Name : Rectangular Pokemon \n");
			break;
		case 3 :
			length_1=sqrt(body_size/PI);
			circle(length_1);					//function is so long. ---at line 146 */
			printf("Name : Circular Pokemon \n");
			break;
	}
	
	printf("Size : %lf \n",body_size);
	
	switch(color) {
		case YELLOW :
			printf("Color : Yellow \n");
			break;
		case BLUE :
			printf("Color : Blue \n");
			break;
		case BLACK :
			printf("Color : Black \n");
			break;
		case WHITE :
			printf("Color : White \n");
			break;
		case RED :
			printf("Color : Red \n");
			break;
	}
	
	printf("Move : %lf \n",move_length);
	printf("Attack Power : %d \n",attack_power);
}

void circle(int radius){
	int i,count,k,z;
	
	if(radius%2 == 0){											/* for odd number radius */
		
		for(i = 0; i < ((radius/2 )+1) ;i++){					/* loop for top side of circle */
			if(i==0){											/* firts line check */
				for(k=0;k<radius-1;k++){						/* loop for first line */ 
					printf("    ");								/* half tab */
				}
			}
			else{												/* loop for other lines */
				k=0;
				while( k < (radius-(i*2))){
					printf("    ");								/* half tab */
					k++;
				}
			}
			printf("X");
			if(i==0){
				printf("\n");
			}
			else{												/* loop for second x in the line */
				for(k=0;k<2*(i-1)+1;k++){
					printf("\t");
				}
				printf("X\n");
			} 	
		}
		for(i = 0; i <(radius/2 ) ;i++){						/* loop for bottom side of circle */
			if(i==radius/2-1){	
				for(k = 0; k<radius-1 ;k++){
					printf("    ");								/* half tab */
				}
				printf("X\n");
			}
			else{
				for(k=0;k<(i+1);k++){
					printf("\t");
				}
				printf("X");
				k=0;
				while(k < radius-1-( (i+1) * 2)){
					printf("\t");
					k++;
				}
				printf("X\n");
			}
		}	
	}
	if(radius%2 > 0){  											/* for odd number radius */
		count=radius-1;
		for(i = 0; i < ((radius/2 )+1) ;i++){					/* loop for top side of circle */
			k=0;
			while (k < count){
				printf("    ");									/* half tab */
				k++;
			}
			printf("X");
			if(i > 0){
				for(z=0;z<i;z++){
					printf("\t\t");
				}
				printf("X\n");
			}
			else{
				printf("\n");
			}
			count= count-2;
		}
		if(radius>1){											/* loop for bottom side of circle */ 
			for(i=0; i < radius/2 ;i++){
				for(k=0;k<=i;k++){
					printf("\t");
				}
				printf("X");	
				for(k=0;k<(radius-1)-(i+1)*2;k++){
					printf("\t");
				}
				if(i != radius/2-1){
				printf("X\n");
				}
			}
			printf("\n");
		}
		
	}
}
