#include<stdio.h>

#define MIN_DIVISOR 2
#define MIN_DIGIT_C 10


void part1();
int common(int number1,int number2,int divisor);
void part2();
void sort_func(int array[],int start_p,int end_p);
void part3();
void part_3_func(int number);
void part4();
int part4_func(int number,int digit_c,int *digit_length);
void part5();
char find_uppercase(char string[]);
int powerf(int n,int p);

int main(){
	int choice;
	int flag=0;
	while(flag==0){
		printf("\n\n***** MENU *****\n1. Part 1\n2. Part 2\n3. Part 3\n4. Part 4\n5. Part 5\n6. Exit\nChoice: ");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				part1();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 2:
				part2();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 3:
				part3();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 4:
				part4();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 5:
				part5();
				printf("\n \nPress enter to back to menu");
				getchar();
				getchar();
				break;
			case 6:
				printf("\n\nExiting...");
				flag=1;
				break;
			default:
				printf("\nThis is an invalid choice.Try again!\n");		
		}
	}
	
	return 0;
}

void part1(){
	int number1,number2,sum;
	printf("Please enter two numbers.");
	scanf("%d %d",&number1,&number2);
	sum=common(number1,number2,MIN_DIVISOR);
	printf("\n Output: %d ",sum);
}

int common(int number1,int number2,int divisor){
	int r=0;
	if( number1 % divisor == 0 && number2 % divisor == 0){ // this finds common divisor
		r = divisor * common( number1/divisor, number2/divisor , divisor);
	}
	else if(number1 % divisor != 0 && number2 % divisor == 0){ // this finds uncommon divisor
		r= common( number1, number2/divisor , divisor);
	}
	else if(number1 % divisor == 0 && number2 % divisor != 0){ // this finds uncommon divisor
		r= common( number1/divisor , number2 , divisor);
	}
	else if(number1 == 1 || number2 == 1){ // end of dividing
		r=1;
	}
	else{ // this finds the number which is not dividing both of two numbers.
		r= common( number1, number2 , divisor+1);
	}
	return r;
}

void part2(){
	int array[1000]; //we dont use malloc or realloc so i need to do this with big number
	int array_size;
	int i;
	printf("Enter the length of the list: ");
	scanf("%d",&array_size);
	printf("Enter the elements of list: ");
	for(i=0 ; i<array_size ; i++){
		scanf("%d",&array[i]);
	}
	
	sort_func(array,0,array_size-1); // 0 is start point
	
	printf("Sorted array is : ");  
 	for(i=0;i<array_size;i++){
    	printf("%d ",array[i]);
	}
}

void sort_func(int array[],int start_p,int end_p){
	int merged_array[1000],unmerged_array_1[1000],unmerged_array_2[1000];
	int i,counter_1=0,counter_2=0;
	int middle_p;
	middle_p=(start_p+end_p) / 2;
	if( (end_p - start_p) > 0  ){ // stops when there is no chance to divide two anymore
		
		sort_func( array , start_p , middle_p );  // this is recursive part
		for(i=0 ; i< middle_p-start_p+1 ; i++ ){	// storing the half of tha array in unmerged array
			unmerged_array_1[i]=array[start_p+i];
		}
		
		sort_func( array , middle_p + 1, end_p ); // this is recursive part
		for(i=0 ; i< end_p-middle_p ; i++ ){		// storing the half of tha array in unmerged array
			unmerged_array_2[i]=array[middle_p + 1 + i];
		}
	
		for(i=0 ; i <= end_p-start_p ; i++){		// this is merging part. this part is not recursive. i asked you about this.
			if( (unmerged_array_1[counter_1] <= unmerged_array_2[counter_2]  &&  (counter_1 < (middle_p-start_p+1)) ) || counter_2 == (end_p-middle_p) ){
				merged_array[i]=unmerged_array_1[counter_1];
				counter_1++;
			}
			else{
				merged_array[i]=unmerged_array_2[counter_2];
				counter_2++;
			}
		}
		for(i=0 ; i <= end_p-start_p ; i++){  // copying the merged array to the original array.
			array[start_p + i]=merged_array[i];
		}
	}
	else return;  // return if there is no more chance to divide two
}

void part3(){
	int input;
	printf("\nInput:");
	scanf("%d",&input);
	printf("Output:");
	part_3_func(input);
}
void part_3_func(int number){
	printf("%d ",number);
	if(number%2==0)			part_3_func(number/2);
	else if(number==1) 		return;
	else if(number%2==1)	part_3_func((number*3)+1);
}

void part4(){
	int number,sum,digit_length=0;
	printf("Input: ");
	scanf("%d",&number);
	sum=part4_func(number , MIN_DIGIT_C ,&digit_length); // MIN_DIGIT_C is needed to find digits. its starting from 10 and goes(*10*10*10...).
	if(sum==1){
		printf("Output: Equal\n");
	}
	else {
		printf("Output: Not Equal\n");
	}
}

int part4_func(int number,int digit_c,int *digit_length){
	int digit,r;
	if(number%digit_c < number){
		(*digit_length)++;
		digit=(number%digit_c)/(digit_c/10);
		r=part4_func(number,digit_c*10,digit_length) + powerf(digit,*digit_length);
		if(digit_c==10 && r==number)	return 1; // special situation for numbers which has only one digit.
		else if(digit_c==10)			return 0;
		else							return r;
	}
	else{
		(*digit_length)++;
		digit=(number%digit_c)/(digit_c/10);
		r=powerf(digit,*digit_length);
		return r;
	}
}

void part5(){
	char input_s[100];
	char output_c;
	printf("Input:");
	scanf("%s",input_s);
	output_c=find_uppercase(input_s);
	if(output_c=='0'){
		printf("Output: *There is no uppercase!*");
	}
	else{
		printf("Output:%c",output_c);
	}
}

char find_uppercase(char string[]){
	char r;
	if(string[0] > 64 && string[0] < 91){
		return string[0];
	}
	else if(string[0]=='\0'){
		return '0';
	}
	else{
		r=find_uppercase(&string[1]);
	}
	return r;
}

int powerf(int n,int p){  // not effective as pow(math.h func) but it is OK for this code.
	int i,temp_n;
	temp_n=1;
	if(n==0){
		return 0;
	}
	else if(n==1){
		return 1;
	}
	else{
		for (i=0; i<p ; i++){
			temp_n=n*temp_n;
		}
		return temp_n;
	}
}
