#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRUE 1
#define FALSE 0
#define DICT_SIZE 15
#define WORD_LEN 10
#define LINE_LEN 18
#define MAX_ROW 15
#define MAX_COLUMN 15

void randomize_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW]);
void print_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW]);
void fill_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW],int coord[DICT_SIZE][4],char *dict[DICT_SIZE]);
void scan_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW],int coord[DICT_SIZE][4],char *dict[DICT_SIZE],int input_x,int input_y,char input_string[],int *current_word_left);
void make_upercase(char puzzle[MAX_COLUMN][MAX_ROW],int x1,int y1,int x2,int y2);

int get_line_size(char *line) {
	char *ch_iter = line; // so as not to lose beginning of line
	int counter = 0;
	// go until you see new line or null char
	while(*ch_iter != '\n' && *ch_iter != '\0') {
		ch_iter++; // next char
		counter++; // increment counter
	}
	
	return counter;
}

void copy_string(char *source, char *destination) {
	// get iterators over original pointers
	char *src_iter = source;
	char *dst_iter = destination;
	// until null char
	while (*src_iter != '\0' ) {
		// copy pointers
		*dst_iter = *src_iter;
		// advance to next char
		src_iter++;
		dst_iter++;
   }
   // terminate string
   *dst_iter = '\0';
}

void remove_newline(char *line) {
	char *ch_iter = line;
	// go until you see new line
	while(*ch_iter != '\n') {
		ch_iter++; // next char
	}
	*ch_iter = '\0'; // overwrite new line
}

void print_dictionary(char *dict[]) {
	int i;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		printf("%s\n", dict[i]);
	}
}

void print_coord(int coord[DICT_SIZE][4]) {
	int i, j;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		for(j = 0 ; j < 4 ; j++) {
			printf("%d ", coord[i][j]);
		}
		printf("\n");
	}
}

int string_equality_checker(char string_1[],char string_2[]){
	int i,string1_length,string2_length,common_length;
	string1_length=0;
	string2_length=0;
	common_length=0;
	for(i=0;string_1[i]!= '\0';i++){		//strlen for string1
		string1_length++;
	}
	for(i=0;string_2[i]!= '\0';i++){		//strlen for string 2
		string2_length++;
	}
	for(i=0;string_1[i]==string_2[i] && i<string1_length;i++){ //common length calculator loop
		common_length++;
	}
	if(string1_length==common_length && string2_length== string1_length){
		return TRUE;
	}
	else{
		return FALSE;
	}		
}

int string_length_finder(char string[]){
	int i,string_length;
	string_length=0;
	for(i=0;string[i]!= '\0';i++){		//strlen for string1
		string_length++;
	}
	return string_length;
}

int main(){
	int input_x,input_y,current_word_left=DICT_SIZE;
	int gameover=FALSE;
	char *dict[DICT_SIZE];
    int coord[DICT_SIZE][4];    
    char line[LINE_LEN];
    char puzzle[MAX_COLUMN][MAX_ROW];
    char input_string[11];  //max word length + 1 ( '\0' )
    srand(time(NULL));
	FILE *fp = fopen("word_hunter.dat", "r");
	
	int line_counter = 0;
	int dict_counter = 0;
	while(fgets(line, LINE_LEN, fp) != NULL) {
		if(line_counter%5 == 0) {
			dict[dict_counter] = (char*) malloc(sizeof(char) * get_line_size(line));
			remove_newline(line);
			copy_string(line, dict[dict_counter]);
		} else if (line_counter%5 == 1){
			coord[dict_counter][0] = atoi(line);
		} else if (line_counter%5 == 2){			
			coord[dict_counter][1] = atoi(line);		
		} else if (line_counter%5 == 3){
			coord[dict_counter][2] = atoi(line);
		} else if (line_counter%5 == 4){
			coord[dict_counter][3] = atoi(line);
			dict_counter++;
		}
		line_counter++;
	}
	
	fclose(fp);
//	print_dictionary(dict);
//	print_coord(coord);
	randomize_the_puzzle(puzzle);
	fill_the_puzzle(puzzle, coord, dict);
	printf("*****Welcome to the puzzle game!*****\n***Please type 'exit game' to exit***\n\n\n");
	while(gameover==FALSE){
		input_string[0]='\n';
		print_the_puzzle(puzzle);
		printf("\n%d words left.\n",current_word_left);
		printf("Please enter the word!\n>>");
		gets(input_string);
		if(string_equality_checker(input_string,"exit game")==TRUE){
			printf("\nexiting...");
		}
		else{
			printf("Please enter the coordinates.\nX=");
			scanf("%d",&input_x);
			printf("Y=");
			scanf("%d",&input_y);
			scan_the_puzzle(puzzle,coord,dict,input_x,input_y,input_string,&current_word_left);
		}
		if(current_word_left==0){
			gameover=TRUE;
			print_the_puzzle(puzzle);
			printf("\nYou found all words!\n");
		}
		getchar(); // to optimize gets()
			
	}
	
	
	return 0;
}

void randomize_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW]){
	int row,column;	
	for(row=0 ; row < MAX_ROW ;row++){  					 // line number changing in this loop.
		for(column=0 ; column < MAX_COLUMN ;column++){		 // column number changing in this loop
			puzzle[column][row]= 97 + (rand()%26); 			// in ascii table 'a' is 97 and last char is '122'. to do rondom 97 + rand().
		}
	}
}

void print_the_puzzle(char puzzle[MAX_ROW][MAX_COLUMN]){
	int row,column;	
	for(row=0 ; row < MAX_ROW ;row++){						// line number changing in this loop.
		for(column=0 ; column < MAX_COLUMN ;column++){ 		// column number changing in this loop
			printf("%c ",puzzle[row][column]);
		}
		printf("\n");
	}
}

void fill_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW],int coord[DICT_SIZE][4],char *dict[DICT_SIZE]){
	int x1,y1,x2,y2,i,count;	
	for(i=0;i< DICT_SIZE ; i++){
		x1=coord[i][0];
		y1=coord[i][1];
		x2=coord[i][2];
		y2=coord[i][3];
		count=0;
		
		if(x1==x2){ 				//checking that is word horizantal
			if(y1 < y2){
				for(y1 ; y1 <= y2 ; y1++){
					puzzle[x1][y1]=dict[i][count];
					count++;
				}
			}
			else{
				for(y1 ; y1 >= y2 ; y1--){
					puzzle[x1][y1]=dict[i][count];
					count++;
				}	
			}
		}
		else if(y1==y2){			//checking that is word vertical
			if(x1 < x2){
				for(x1 ; x1 <= x2 ; x1++){
					puzzle[x1][y1]=dict[i][count];
					count++;
				}
			}
			else{
				for(x1 ; x1 >= x2 ; x1--){
					puzzle[x1][y1]=dict[i][count];
					count++;
				}	
			}
		}
		else{						//diagonal
			if(y1 > y2 && x2 > x1){ 	// northeast diagonal filling
				while(y1 >= y2 && x2 >= x1){
					puzzle[x1][y1]=dict[i][count];
					y1--;
					x1++;
					count++;
				}
			}
			else if(y1 < y2 && x2 > x1){ 	// southeast diagonal filling
				while(y1 <= y2 && x2 >= x1){
					puzzle[x1][y1]=dict[i][count];
					y1++;
					x1++;
					count++;
				}
			}
			else if(y1 < y2 && x2 < x1){ 	// southwest diagonal filling
				while(y1 <= y2 && x2 <= x1){
					puzzle[x1][y1]=dict[i][count];
					y1++;
					x1--;
					count++;
				}
			}
			else if(y1 > y2 && x2 < x1){ 	// northwest diagonal filling
				while(y1 >= y2 && x2 <= x1){
					puzzle[x1][y1]=dict[i][count];
					y1--;
					x1--;
					count++;
				}
			}
		}
	}
}

void scan_the_puzzle(char puzzle[MAX_COLUMN][MAX_ROW],int coord[DICT_SIZE][4],char *dict[DICT_SIZE],int input_x,int input_y,char input_string[],int *current_word_left){
	int word_found=FALSE,i,k,input_string_length,temp_x,temp_y;
	char scanned_word[11],scanned_word_reverse[11];
	
	input_string_length=string_length_finder(input_string); 
	
	if(word_found==FALSE){				//checking that is word found
		temp_x=input_x;					//moving with temp_x / temp_y
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){		// scaning loop. storing in another string(scanned_word / scanned_word_reverse).
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_y++;							// this is needed to moving way. other "if"s same but only this line different.
		}
		scanned_word[input_string_length]='\0';		//finising the scanned word.
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE ){  //checking that is scanned word same with input word.
			word_found==TRUE;				//blocking the other "if"s if it found
			make_upercase(puzzle,input_x,input_y,temp_x,temp_y-1); 
			*current_word_left-=1;			// decreasing the word count
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){  //checking that is reverse scanned word same with input word.
			word_found==TRUE;				//blocking the other "if"s if it found
			make_upercase(puzzle,input_x,input_y,temp_x,temp_y-1);
			*current_word_left-=1; // decreasing the word count
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_y--;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x,temp_y+1);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x,temp_y+1);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x++;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE ){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x--;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x++;
			temp_y--;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y+1);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y+1);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x++;
			temp_y++;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y-1);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE ){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x-1,temp_y-1);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){		//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x--;
			temp_y++;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y-1);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE ){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y-1);
			*current_word_left-=1;
		}
	}
	if(word_found==FALSE){			//same with first if. only move lines different.
		temp_x=input_x;
		temp_y=input_y;
		for(i=0 ; i<input_string_length ; i++){
			if(puzzle[temp_x][temp_y] >64 && puzzle[temp_x][temp_y]<91){
				scanned_word[i]=puzzle[temp_x][temp_y]+32;
				scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y]+32;
			}
			else{
			scanned_word[i]=puzzle[temp_x][temp_y];
			scanned_word_reverse[input_string_length-1-i]=puzzle[temp_x][temp_y];
			}
			temp_x--;
			temp_y--;
		}
		scanned_word[input_string_length]='\0';
		scanned_word_reverse[input_string_length]='\0';
		if(string_equality_checker(scanned_word,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y+1);
			*current_word_left-=1;
		}
		else if(string_equality_checker(scanned_word_reverse,input_string)==TRUE){
			word_found==TRUE;
			make_upercase(puzzle,input_x,input_y,temp_x+1,temp_y+1);
			*current_word_left-=1;
		}
	}
}

void make_upercase(char puzzle[MAX_COLUMN][MAX_ROW],int x1,int y1,int x2,int y2){
	int count=0;
	if(x1==x2){ 				//checking that is word vertical
		if(y1 < y2){
			for(y1 ; y1 <= y2 ; y1++){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				count++;
			}
		}
		else{
			for(y1 ; y1 >= y2 ; y1--){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				count++;
			}	
		}
	}
	else if(y1==y2){			//checking that is word horizantal
		if(x1 < x2){
			for(x1 ; x1 <= x2 ; x1++){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				count++;
			}
		}
		else{
			for(x1 ; x1 >= x2 ; x1--){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				count++;
			}	
		}
	}
	else{						//diagonal
		if(y1 > y2 && x2 > x1){ 	// northeast diagonal 
			while(y1 >= y2 && x2 >= x1){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				y1--;
				x1++;
				count++;
			}
		}
		else if(y1 < y2 && x2 > x1){ 	// southeast diagonal 
			while(y1 <= y2 && x2 >= x1){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				y1++;
				x1++;
				count++;
			}
		}
		else if(y1 < y2 && x2 < x1){ 	// southwest diagonal 
			while(y1 <= y2 && x2 <= x1){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				y1++;
				x1--;
				count++;
			}
		}
		else if(y1 > y2 && x2 < x1){ 	// northwest diagonal 
			while(y1 >= y2 && x2 <= x1){
				if(puzzle[x1][y1]<123 && puzzle[x1][y1]>96){
					puzzle[x1][y1]=puzzle[x1][y1]-32;
				}
				y1--;
				x1--;
				count++;
			}
		}
	}	
}
