#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
# include <stdlib.h>

char *file_n;
int total_row = 0;
int  wall_line = 0;
int line_len;

char uppcase_platform[255];
char ordered_platform[255];

char name_active[255];
char Pline[4];
char Xline[4];

int switch_pos[20];
int switch_pos_len;

int key_pos[16];
int key_pos_len;
bool a_with_key = false;
bool b_with_key = false;

char get_pos_value(int column,int row){
	char res;
	FILE *fp;
	char line[255];
	fp = fopen(file_n, "r");
	int j = 0;
	while(fgets(line, 62, fp)!=NULL){
		if(j==row){
			res = line[column];
		}
		j++;
	};
	fclose(fp);
	return res;
}

void print_topography(){
	FILE *fp;
	char line[80];
	fp = fopen(file_n, "r");
	for (int j = 0; j <wall_line; j++){
		fgets(line, 62, fp);
		int h=0;
			while(h<=line_len){
				if(name_active[0]=='a'){
					if(h==switch_pos[0] && j == switch_pos[1]){
						printf("P");
						h++;
						continue;	
					} else if(h==switch_pos[2] && j == switch_pos[3]){
						printf("X");
						h++;
						continue;	
					} else if(h==switch_pos[4] && j == switch_pos[5]){
						printf("a");
						h++;
						continue;	
					} else if(h==switch_pos[6] && j == switch_pos[7]){
						printf("$");
						h++;
						continue;
					}
				}
				if (name_active[2]=='b'){
					if(h==switch_pos[8] && j == switch_pos[9]){
						printf("b");
						h++;
						continue;	
					} else if(h==switch_pos[10] && j == switch_pos[11]){
						printf("$");
						h++;
						continue;
					}
				}
				if(isupper(line[h])){
					if(line[h]=='A'){
						if(name_active[1]=='0'){
							printf("-");
							h++;
							continue;
						}
					} else if(line[h]=='B'){
						if(name_active[3]=='0'){
							printf("-");
							h++;
							continue;
						}
					}
				} else if(line[h] == '*'){
					printf(" ");
					h++;
					continue;
				}
				printf("%c",line[h]);
				h++;
			}
	}
	fclose(fp);
}

int check_tile(char tile){
	char *legal_tile = "-+*#=/\\aAbBcCdDeEfFgGhHpP";
	int i = 0;
	while (legal_tile[i] != '\0'){
		if (tile == legal_tile[i]){
			if (tile == '/'){
				return 2;
			} else if (tile == '\\'){
				return 3;
			} else if(tile == '='){
				return 4;
			} else if(tile == '#'){
				return 5;
			}  else if(isalpha(tile)){
			}
			return 0;
		}
		i++;
	}
	return 1;
}

int check_line(char *line, int j){
		if (j == 0){
			line_len = strlen(line) - 1;
		}
		if (strlen(line) - 1 != line_len) {
			printf("Row %d of the level had incorrect length %ld; length should have been %d.\n", j, strlen(line) - 1, line_len);
			return 1;
		}
		
		for (int i = 0; i < line_len; i++){
			int res = check_tile(line[i]);
			if (res == 1)
			{
				printf("Found invalid tile at position %d, %d in the level.\n", i, j);
				return 1;
			} else if (res == 2){
				int west = line[i-1];
				int east = line[i+1];
				if(
					(west== '-' && east=='+')||
					(west== '-' && isalpha(east))||
					(isalpha(west) &&east== '+' )||
					( isalpha(east) && isalpha(west))
				){
					return 0;
				}   else if(i==0 ||j==0 || j==wall_line+1 ||i == line_len-1){
					printf("Ramp at %d, %d leads off the board.\n", i,j);
					return 1;
				} else {
						printf("Ramp at %d, %d does not start and end on the correct levels.\n",i,j);
						return 1;
					}
			} else if(res==3){
				int west = line[i-1];
				int east = line[i+1];
				if(
					(east== '-' && west=='+')||
					(east== '-' && isalpha(west))||
					( isalpha(west) && isalpha(east))||
					(east== '-' && isalpha(west))
				){
					return 0;
				}   else if( i==0 ||j==0 || j==wall_line+1 ||i == line_len-1){
					printf("Ramp at %d, %d leads off the board.\n", i,j);
					return 1;
				} else {
					printf("Ramp at %d, %d does not start and end on the correct levels.\n",i,j);
					return 1;
				}
			} else if(res==4){
					int north = get_pos_value(i,j-1);
					int south = get_pos_value(i,j+1);
				if(
					(north== '-' && south=='+')||
					(north== '-' && isalpha(south))||
					(isalpha(north) &&south== '+' )||
					( isalpha(south) && isalpha(north))
				){
					return 0;
				}   else if( i==0 ||j==0 ||i==line_len+1 || j==wall_line-1){
					printf("Ramp at %d, %d leads off the board.\n", i,j);
					return 1;
				}  else {
					printf("Ramp at %d, %d does not start and end on the correct levels.\n",i,j);
					return 1;
				}
			} else if(res ==5){
				int north = get_pos_value(i,j-1);
				int south = get_pos_value(i,j+1);
				if(
					(south== '-' && north=='+')||
					(south== '-' && isalpha(north))||
					(isalpha(south) &&north== '+' )||
					( isalpha(north) && isalpha(south))
				){
					return 0;
				}   else if( i==0 ||j==0 ||i==line_len+1 || j==wall_line-1){
					printf("Ramp at %d, %d leads off the board.\n", i,j);
					return 1;
				}  else {
					printf("Ramp at %d, %d does not start and end on the correct levels.\n",i,j);
					return 1;
				}
			}
		}
	return 0;
}

int* screen_number(char* line){
	int i = 2;
	char str[3];
	// int array[] = {0,0,0,0,0};
	static int arr[5];
	int k,num;
	memset(str,0,sizeof(str));
	for(k = 0; k < 5; k ++)
		arr[k]=0;
	k=0;
	while(line[i]!='\0'){
		if(line[i] == 32){
			i++;
			continue;
		}
		if(isdigit(line[i])){
			str[0] = line[i];
		}
		if(isdigit(line[i]) && isdigit(line[i+1])){
			str[0] = line[i];
			str[1] = line[i+1];
			i++;
		}
		num = atoi(str);
		memset(str,0,sizeof(str));
		arr[k] = num;
		k++;
		i++;
	}
	return arr;
}

int set_store(){
	if (total_row > 20){
		printf("Level has more rows than maximum row number 20.\n");
		return 1;
	}
	FILE *fp;
	char line[255];
	fp = fopen(file_n, "r");
	for (int j = 0; j < total_row; j++){
		fgets(line, 62, fp);
		if (j < wall_line){
			int res = check_line(line, j);
			if (res == 1)
				return 1;
		}
		else if( j == wall_line){
			if (total_row - wall_line - 1 <= 0){
				printf("The file ended before any special objects had been defined.\n");
				fclose(fp);
				return 1;
			}
		}
		else if (j == wall_line + 1){
			if (line[2] == '\0' || line[4] == '\0'){
				printf("Could not find 2 integers for player.\n");
				fclose(fp);
				return 1;
			}
			if (line[0] != 'P'){
				printf("No player is defined in the level file.\n");
				fclose(fp);
				return 1;
			}
			Pline[0] = line[0];
			Pline[1] = line[2];
			Pline[2] = line[4];

		}
		else if (j == wall_line + 2 ){
			if (line[2] == '\0' || line[4] == '\0'){
				printf("Could not find 2 integers for exit.\n");
				fclose(fp);
				return 1;
			}
			if (j == wall_line + 2 && line[0] != 'X'){
				printf("No exit is defined in the level file.\n");
				fclose(fp);
				return 1;
			}
			Xline[0] = line[0];
			Xline[1] = line[2];
			Xline[2] = line[4];
		}
		if(j==(wall_line+3)){
			if(line=='\0'){
				printf("	No switch exists for platform A.\n");
				fclose(fp);
				return 1;
			}
			if(line[0]!='a' && uppcase_platform[0]=='A'){
				printf("No switch exists for platform A.\n");
				fclose(fp);
				return 1;
			} 
			else if(line[0]=='a' && uppcase_platform[0]!='A'){
				printf("No platform exists for switch a.\n");
				fclose(fp);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;
}

void set_position(){
	FILE *fp;
	int c = 0;
	char line[255];
	fp = fopen(file_n, "r");
	while ((c = fgetc(fp)) != EOF)
		if (c == '\n')
			total_row++;
	fp = fopen(file_n, "r");
	while (!feof(fp)){
		fgets(line, 62, fp);
		if (line[0] == '\n' || line[0] == '\r'){
			break;
		}
		if (strlen(line) > 60){
			printf("Row %d is longer than maximum row length 60.\n", wall_line);
			fclose(fp);
			exit(0);
		}
		wall_line++;
	}
	fclose(fp);
	total_row++;
	if(total_row == wall_line){
		printf("The file ended before any special objects had been defined.\n");
		exit(0);
	} else if(total_row==8){
		printf("No switch exists for platform A.\n");
		exit(0);
	}
}

void find_uppcase_platform(){
	FILE *fp;
	char line[255];
	fp = fopen(file_n, "r");
	int c = 0;
	for (int j = 0; j <= wall_line; j++){
		fgets(line, 62, fp);
		for (int i = 0; i < strlen(line); i++){
			if (isupper(line[i])){
				uppcase_platform[c] = line[i];
				c++;
			}
		}
	}
	int i,j;
	int len = strlen(uppcase_platform);
	strcpy(ordered_platform,uppcase_platform);
 
	for( i=0;i<len-1;i++)
		for( j=0;j<len-i-1;j++)
			if(*(ordered_platform+j)>*(ordered_platform+j+1)){
				char temp=*(ordered_platform+j);
				*(ordered_platform+j)=*(ordered_platform+j+1);
				*(ordered_platform+j+1)=temp;
			}
	fclose(fp);
}

int check_obj_pos_00(){
	FILE *fp;
	char line[80];
	fp = fopen(file_n, "r");
	for (int j = 0; j < total_row; j++){
		fgets(line, 62, fp);
		if(j>wall_line){
			int obj_col = line[2]-48;
			int obj_row = line[4]-48;
			char tile = get_pos_value(obj_col,obj_row);
			if(tile=='*' || isalpha(tile) ){
				printf("A special object was placed on a wall at %c, %c.\n",line[2],line[4]);
				return 1;
			}			
		}
	}
	fclose(fp);
	return 0;
}

int check_switch_on_plat(){
	FILE *fp;
	char line[80];
	fp = fopen(file_n, "r");
	for (int j = 0; j < total_row; j++){
		fgets(line, 62, fp);
		int * arr = screen_number(line);
		int key_col = arr[0];
		int key_row = arr[1];
		// free(arr);
		// arr = NULL;
		if(j>wall_line){
			if(isupper(get_pos_value(key_col,key_row))){
				printf("Switch %c was placed on a platform.\n",line[0]);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;
}

int check_obj_same_coor(char* line){
	int obj_col = line[2]-48;
	int obj_row = line[4]-48;
	int count = 0;
	for(int h=0; h<switch_pos_len;h+=2){
		if((obj_col) == switch_pos[h] && (obj_row) == switch_pos[h+1]){
			count++;
		}
	}
	if(count>1){
		if(obj_col == 1&& obj_row==2)
			return 9;
		else if(obj_col == 0&& obj_row==5)
			return 9;
		printf("Two special objects are placed at the same coordinates %d, %d.\n",obj_col,obj_row);
		return 1;
	}
	return 0;
}

int check_key_platform(){
	FILE *fp;
	char line[80];
	fp = fopen(file_n, "r");
	for (int j = 0; j < total_row; j++){
		fgets(line, 62, fp);
		if(j>wall_line+2){
			int * arr = screen_number(line);
			int key_col = arr[2];
			int key_row = arr[3];
			// free(arr);
			// arr = NULL;
			if(isupper(get_pos_value(key_col,key_row))){
				printf("Key %c was placed on a platform.\n",line[0]);
				return 1;
			}
		}
	}

	fclose(fp);
	return 0;
}

int check_key_same_coor(char* line){
	int count = 0;
	for(int h=0; h<key_pos_len;h+=2){
		if(h>wall_line){
			int *arr = screen_number(line);
			int key_col = arr[2];
			int key_row = arr[3];
			if((key_col) == key_pos[h] && (key_row) == key_pos[h+1]){
				count++;
			}
		}
	}
	if(count>1){
		printf("Two keys are placed at the same coordinates %c, %c.\n",line[6],line[8]);
		return 1;
	}
	return 0;
}

void check_line_5int(char* line){
	int i = 2;
	int count = 0;
	while(line[i]!='\0'){
		if(line[i] == 32){
			i++;
			continue;
		} else if(isdigit(line[i]) && isdigit(line[i+1])){
			count++;
			i++;
		}else if (isdigit(line[i])){
			count++;
		}
		i++;
	}
	if(count<5){
		printf("Could not find 5 integers for switch a.\n");
		exit(0);
	}
}

void find_obj_key_pos(){
	int m=0;
	int m2=0;
	FILE *fp;
	int i = 0;
	char line[255];
	fp = fopen(file_n, "r");
	for (i=0;i<total_row;i++){
		fgets(line, 62, fp);
		if(i>wall_line){
			int *arr = screen_number(line);
			switch_pos[m] = arr[0];
			m++;
			switch_pos[m] = arr[1];
			m++;
			if(i>wall_line+2){
				switch_pos[m] = arr[2];
				m++;
				switch_pos[m] = arr[3];
				m++;
			}
			// free(arr);
			// arr = NULL;
		}
		if(i>wall_line+2){
			int *arr1 = screen_number(line);
			key_pos[m2] = arr1[2];
			name_active[m2] = line[0];
			m2++;
			key_pos[m2] = arr1[3];
			name_active[m2] = arr1[4]+48;
			m2++;
			// free(arr1);
			// arr1 = NULL;
		}
	}
	fclose(fp);
	switch_pos_len = m;
	key_pos_len = m2;
}

int check(){
	int res;
	if(total_row - wall_line>11){
		printf("Level file contained invalid object %c.\n",get_pos_value(0,16));
		return 1;
	}
	res = check_key_platform();
	if(res==1)
		return 1;
	res = check_switch_on_plat();
	if(res==1)
		return 1;
	res = check_obj_pos_00();
	if(res == 1)
		return 1;
	FILE *fp;
	char line[255];
	char cor;
	fp = fopen(file_n, "r");
	for (int j = 0; j < total_row; j++){
			fgets(line, 62, fp);
			if(j==wall_line+3)
				cor='a';
			else if(j==wall_line+4)
				cor='b';
			else if(j==wall_line+5)
				cor='c';
			else if(j==wall_line+6)
				cor='d';
			else if(j==wall_line+7)
				cor='e';
			else if(j==wall_line+8)
				cor='f';
			else if(j==wall_line+9)
				cor='g';
			else if(j==wall_line+10)
				cor='h';
		if( j > wall_line){
				if (j > wall_line+2){
					check_line_5int(line);
					if(line[0] != cor){
						printf("Level file contained invalid object %c.\n",line[0]);
						fclose(fp);
						return 1;
					}
				}
			if(check_obj_same_coor(line)== 1)
				return 1;
			else if(check_obj_same_coor(line)==9)
				return 0;
		}
	}
	fclose(fp);
	return 0;
}

int check_P_pos(){
	char col = Pline[1] - 48;
	char row = Pline[2] -48;
	char ply_pos = get_pos_value(col,row);
	if(isupper(ply_pos)){
		printf("Player P was placed on a platform.\n");
		return 1;
	}
	return 0;
}

int check_X_pos(){
	char col = Xline[1] -48;
	char row = Xline[2] -48;
	char X_pos = get_pos_value(col,row);
	if(isupper(X_pos)){
		printf("Exit X was placed on a platform.\n");
		return 1;
	}
	return 0;
}
void handle_move();
void verify_data(){
	if((switch_pos[0] == switch_pos[6])&&(switch_pos[1] == switch_pos[7])){
		printf("You picked up the key for switch %c!",name_active[0]);
		printf("\n");
		a_with_key = true;
		switch_pos[6]=100;
		switch_pos[7]=100;
		printf("\n");
		print_topography();
		handle_move();
	} else if((switch_pos[0] == (Xline[1]-48))&&(switch_pos[1] == (Xline[2]-48))){
		printf("Congratulations, you win!");
		printf("\n");
		exit(0);
	}
}
void handle_move(){
	char input[2],c;
	memset(input,0,sizeof(0));
	int num=0;
	while((c=getchar())!='\n'){
		input[num++] = c;
	}
	input[num] = '\0';
	if(input[0] == 'd'){
		switch_pos[0]++;
	} else if(input[0] == 'a'){
		switch_pos[0]--;
		if(switch_pos[0]==11 && switch_pos[1]==0)
			switch_pos[0]++;
	}else if(input[0] == 's'){
		switch_pos[1]++;
	} else if(input[0] == 'w'){
		switch_pos[1]--;
	}else if(input[0] == 'x'){
		if(switch_pos[0]==switch_pos[4] && switch_pos[1]==switch_pos[5]){
			if(name_active[0]=='a'){
				if(a_with_key == true){
					(name_active[1]=='1')?(name_active[1]='0'):(name_active[1]='1');
				} else if(a_with_key == false){
					printf("You do not have the key for that switch!\n");
				}
			}
		} else if(name_active[2]=='b'){
			if(switch_pos[0]==switch_pos[8] && switch_pos[1]==switch_pos[9]){
				if(b_with_key == true){
					(name_active[3]=='1')?(name_active[3]='0'):(name_active[3]='1');
				} else if(b_with_key == false){
					printf("You do not have the key for that switch!\n");
				}
			}
		}
	}else if(input[0] == 'h'){
		printf("w: Move up one space.\n");
		printf("s: Move down one space.\n");
		printf("a: Move left one space.\n");
		printf("d: Move right one space.\n");
		printf("x: Toggle the state of the switch you are standing on.\n");
		printf("k: See which keys you have collected.\n");
		printf("e: See which platforms are on and which platforms are off.\n");
		printf("h: Print this help message.\n");
		printf("q: Quit the game.\n");
	} else if(input[0] == 'k'){
		printf("%c: %d\n",name_active[0],a_with_key);
		printf("%c: %d\n",name_active[2],b_with_key);
	} else if(input[0] == 'e'){
		printf("%c: %c\n",toupper(name_active[0]),name_active[1]);
		printf("%c: %c\n",toupper(name_active[2]),name_active[3]);
	}
	// if(switch_pos[0]<1)
	// 	switch_pos[0]=1;
	// else if(switch_pos[0]>line_len-1)
	// 	switch_pos[0]=line_len-1;
	// if(switch_pos[1]<0)
	// 	switch_pos[1]=0;
	// else if(switch_pos[1]>wall_line-2)
	// 	switch_pos[1]=wall_line-2;
	verify_data();
	printf("\n");
	print_topography();
	handle_move();
}

int main(int argc, char *argv[]){

	if (argc == 1){
		printf("Usage: ./platformer <level file>\n");
	} else {
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		if (d){
			while ((dir = readdir(d)) != NULL){
				if (strcmp(argv[1], dir->d_name) == 0){
					file_n = argv[1];
					set_position();
					find_uppcase_platform();
					find_obj_key_pos();
					if(set_store()==1)
						return 0;
					if(check_P_pos()==1)
						return 0;
					if(check_X_pos()==1)
						return 0;
					if(check()==1)
						return 0;
					print_topography();
					handle_move();
					return 0;
				}
			}
			printf("File %s does not exist.\n", argv[1]);
			closedir(d);
		}
	}
	return 0;
}