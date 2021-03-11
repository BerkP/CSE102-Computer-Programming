#include<stdio.h>

#define MIDTERM 0
#define FINAL 1

void part_one();
int doit(int (*f)(int,int),int first_number,int second_number);
int add(int number1,int number2);
int sub(int number1,int number2);
int multiplication(int number1,int number2);
int division(int number1,int number2);
int power(int number1,int number2);
int modulo(int number1,int number2);
void part_two();
int  take_grades(int lab_grades[]);
double calculate_homework(int hw_grades[]);
int take_exam_grades(int exam_grades[]);
double calculate_lab(int lab_grades[]);
double calculate_all(int avg_lab,int exam_grades[],int avg_hw);
void part_three();
int powering(int x,int y);


int main(){
	int choice,flag=0;
	while(flag==0){   /* Control flag for exiting program */
		printf("\e[2J"); /* Clear terminal */
		printf("***** MENU *****\n1. Part 1\n2. Part 2\n3. Part 3\n4. Exit\nChoice: ");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				printf("Part one started!\nYou need to use these operators for calculation.\nDo not forget to press 'space' after the operator!\n(+)Addition\t\t(-)Subtraction\n(*)Multiplication\t(/)Division\n");
				printf("(**)Powering\t\t(%%)Modulo\nExample >>  I: + 5 6  >>  O: 11  >>  I:/ 2  >>  O: 5\nPlease enter x/X after calculations to back to menu\n");
				part_one();
				break;
			case 2:
				part_two();
				printf("\n \nPress enter to back to menu");
				getchar(); /* Need for return to the menu*/
				getchar(); 
				break;
			case 3:
				part_three();
				printf("\n \nPress enter to back to menu");
				getchar(); /* Need for return to the menu*/
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

void part_one(){
	getchar();
	int temp_number[2],first_number,second_number,result,i,flag=1;
	char operator_sign[2],nextline_checker;
	while(flag==1){  /* Loop until "x" entered */
		nextline_checker=' ';
		temp_number[0]=0;
		temp_number[1]=0;
		scanf("%c%c",&operator_sign[0],&operator_sign[1]);
		if(operator_sign[0]=='x' || operator_sign[0]=='X'){
			flag=0;
		}
		if(flag==1){	
			for(i=0;i<2 && nextline_checker!='\n';i++){   /* Cheking for "enter input" */
				scanf("%d%c", &temp_number[i], &nextline_checker);
			}
			if(i==1){
				second_number=temp_number[0];
			}
			else{
				first_number=temp_number[0];
				second_number=temp_number[1];
			}
				
			if(operator_sign[0]=='+' && operator_sign[1]==' '){
				result=doit (add, first_number, second_number);
			}
			else if(operator_sign[0]=='-' && operator_sign[1]==' '){
				result=doit (sub, first_number, second_number);
			}
			else if(operator_sign[0]=='*' && operator_sign[1]==' '){
				result=doit (multiplication, first_number, second_number);
			}
			else if(operator_sign[0]=='/' && operator_sign[1]==' '){
				result=doit (division, first_number, second_number);
			}
			else if(operator_sign[0]=='*' && operator_sign[1]=='*'){
				result=doit (power, first_number, second_number);
			}
			else if(operator_sign[0]=='%' && operator_sign[1]==' '){
				result=doit (modulo, first_number, second_number);
			}
			else{
				printf("Wrong operator or input!Try again.\n");		
			}
			
			
			printf("%d\n",result);
			first_number=result;
		}
	}
}

int doit(int (*f)(int,int),int first_number,int second_number){
	int result;
	result=(*f)(first_number,second_number);
	return result;
}

int add(int number1,int number2){
	return number1+number2;
}
int sub(int number1,int number2){
	return number1-number2;
}
int  multiplication(int number1,int number2){
	return number1*number2;
}
int division(int number1,int number2){
	if(number2==0){
		printf("Error!Divisor cannot be zero!\n");
		return number1;
	}
	return number1/number2;
}
int power(int number1,int number2){
	return powering(number1,number2);
}
int modulo(int number1,int number2){
	return number1%number2;
}

void part_two(){
	int lab_grades[10],hw_grades[10],exam_grades[2],i;
	double avg_hw,avg_lab,weighted_average;
	printf("Please enter the lab grades.\n");
	take_grades(lab_grades);
	printf("Please enter the homework grades.\n");
	take_grades(hw_grades);
	take_exam_grades(exam_grades);
	avg_hw=calculate_homework(hw_grades);
	avg_lab=calculate_lab(lab_grades);
	weighted_average=calculate_all(avg_lab,exam_grades,avg_hw);
	printf("%f",weighted_average);
}

int  take_grades(int grades[]){
	int i;
	for(i=0;i<10;i++){
		scanf("%d",&grades[i]);
	}
}

int take_exam_grades(int exam_grades[]){
	printf("Please enter the midterm grade");
	scanf("%d",&exam_grades[MIDTERM]);
	printf("Please enter the final grade");
	scanf("%d",&exam_grades[FINAL]);
}

double calculate_homework(int hw_grades[]){
	int total,i;
	double avg;
	total=0;
	for(i=0;i<10;i++){
		total+=hw_grades[i];
	}
	avg=total/(double)10;
	return avg;
}
double calculate_lab(int lab_grades[]){
	int total,i;
	double avg;
	total=0;
	for(i=0;i<10;i++){
		total+=lab_grades[i];
	}
	avg=total/(double)10;
	return avg;
}

double calculate_all(int avg_lab,int exam_grades[],int avg_hw){
	double weighted_average;
	weighted_average=(avg_lab/(double)10)*2+(avg_hw/(double)10)+exam_grades[MIDTERM]/(double)10*3+exam_grades[FINAL]/(double)10*4;
	return weighted_average;
}

void part_three(){
	int line_number,row_number,height;
	scanf("%d",&height);
	for(line_number=0 ; line_number < height ; line_number++){ /* Loop for top side of the shape */
		for(row_number=0 ; row_number < height-line_number-1 ; row_number++){
			printf(" ");
		}
		printf("/");
		for(row_number=0 ; row_number < line_number ; row_number++){
			printf("**");
		}
		printf("\\ \n");
	}
	for(line_number=0 ; line_number < height ; line_number++){ /* Loop for the bottom side of the shape */
		for(row_number=0 ; row_number < line_number ; row_number++){
			printf(" ");
		}
		printf("\\");
		for(row_number=0 ; row_number < height-line_number-1 ; row_number++){
			printf("**");
		}
		printf("/ \n");
	}
}

int powering(int x,int y){   /* POW */
	int i,sum=1;
	if(x==0) return 0;
	else if( y==0) return 1;
	
	else if( y > 0){
		for(i=0;i<y;i++){
			sum *= x;
		}
	}
	return sum;
}

