/****************************
 * Name: Max Grubb
 * Date: 2/8/2916
 * Program: adventure 
 * Description: Command-line room traversal game
 * *********************************************/



#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <string.h>


//Forward declarations
void fileBuild(char* rms[], char* rt[], char* lbl[]); 
void _addRoomName(char* rms[], char* lbl[]);
void _addRoomConnections(char* rms[], char* lbl[], char* rt[]);
void _addRoomType(char* rms[], char* lbl[], char* rt[]);
void arrayRandomizer(char* arr[], int size); 
int randomInt(int key, int key2); 
void _randomRooms(char* conarr[], char* rmarr[], int a, int d);
int lineCount(char* filename);
void setTempRoom(char* curRoom, FILE* tmp); 
void printCurRoom(FILE* tmpf, int e, int* check); 
int checkRooms(FILE* curFile, char chce[]);



int main(){
	srand((unsigned)time(NULL));
	char* rooms[10] = {"Room1", "Room2", "Room3", "Room4", "Room5", "Room6", "Room7", "Room8", "Room9", "Room10"};
	char* rmtype[3] = {"START_ROOM", "MID_ROOM", "END_ROOM"};
	char* labels[3] = {"ROOM NAME: ", "CONNECTION: ", "ROOM TYPE: "};
	FILE* tempFile; 
	FILE* path;
	int lines; 	
	int check = 0; 
	int ckRm = 0;
	int counter = 0;
	char choice[10];
	char roomType[15];
	char pathRoom[10];
	char pth;
	arrayRandomizer(rooms, 10); 
	fileBuild(rooms, rmtype, labels);    // pass arrays to build initial fileset 
mkdir("/rooms", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//Game Start
path = tmpfile();  // to hold room path
lines = lineCount(rooms[0]); 
tempFile = tmpfile(); 
setTempRoom(rooms[0], tempFile);
printCurRoom(tempFile, lines, &check); 

//Game Play Loop 
do{	
	printf("\nWHERE TO?> ");
	scanf("%s", &choice); 
	
	
	ckRm = checkRooms(tempFile, choice);
	
	if(ckRm == 1)		
	{
		close(tempFile); 
		fprintf(path, "%s\n", choice); 
		tempFile = tmpfile();
		lines = lineCount(choice); 
		setTempRoom((char *)choice, tempFile);
		printCurRoom(tempFile, lines, &check); 
		counter++;
	}
	else 
	{
		printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		printCurRoom(tempFile, lines, &check); 
		rewind(tempFile);  
	}


}while(check != 1);

//game end printout
printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
printf("YOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", counter);
	
rewind(path); 
               
       		 pth = fscanf(path, "%s", pathRoom);
	        do {
			printf("%s\n", pathRoom); 
       		        pth = fscanf(path, "%s", pathRoom);
			
		 } while (pth != EOF);


//free tempfiles

	close(tempFile);
	close(path);

	return 0;  
}











 


/***************************************************
 * Function: fileBuild()
 * Parameters: char pointer arrays 
 * Description: Builds files that will be read for 
 * gameplay
 * Preconditions: char arrays declared
 * Post-conditions: files  properly created in 
 * directory
 * ************************************************/

void fileBuild(char* rms[], char* rt[], char* lbl[]){

		_addRoomName(rms, lbl); 
		_addRoomConnections(rms, rt, lbl); 
		_addRoomType(rms, rt, lbl); 
}


/***************************************************
 * Function: _addRoomName()
 * Parameters: char pointer array 
 * Description: creates room files and adds name 
 * Preconditions: char arrays declared
 * Post-conditions:Files with names are created
 * ************************************************/
void _addRoomName(char* rms[], char* lbl[]){


	FILE* gFile;
	int a;
	int b = 7; 
	for (a = 0; a < b; a++){

		gFile = fopen(rms[a], "w"); 
		fprintf(gFile, "%s%s\n", lbl[0], rms[a]); 
		fclose(gFile);  
		

	}

} 


/***************************************************
 * Function: _addRoomConnections()
 * Parameters: char pointer arrays 
 * Description: adds connections to foom files 
 * Preconditions: char arrays declared and files 
 * opened.
 * Post-conditions:connections addded to room files
 * ************************************************/

void _addRoomConnections(char* rms[], char* rt[], char* lbl[]){

	char* connects[7]; 	
	char* connects2[10];
	FILE* gFile1;
	int a, c, d, e, g, h, i;
	int b = 7;
	for(h=0;h<7;h++){

	connects[h] = rms[h];
	} 
	for (a = 0; a < b; a++){
		
		arrayRandomizer(connects, 7);
		c = randomInt(5, 0); 
		e = 0;
		for(d = 0; d <= c; d++){
			
		_randomRooms(connects, rms, a, d);

		}		
		
		
	}	

}


/***************************************************
 * Function: _addRoomType()
 * Parameters: char pointer arrays 
 * Description: adds Room type to foom files 
 * Preconditions: char arrays declared and files 
 * opened.
 * Post-conditions:types addded to room files
 * ************************************************/
void _addRoomType(char* rms[], char* lbl[], char* rt[]){


	FILE* gFile;
	int a;
	int b = 7; 
	for (a = 0; a < b; a++){

		gFile = fopen(rms[a], "a"); 
		if(a == 0)
			fprintf(gFile, "%s%s\n", rt[2], lbl[0]); 
		else if(a > 0 && a < 6)
			fprintf(gFile, "%s%s\n", rt[2], lbl[1]); 
		else if(a == 6) 
			fprintf(gFile, "%s%s\n", rt[2], lbl[2]); 

		fclose(gFile);  
		
	}

}

/***************************************************
 * Function: arrayRandomizer()
 * Parameters: char pointer array 
 * Description: Randomizes room array to create 
 * random gameplay
 * Preconditions: char arrays declared
 * Post-conditions:array is randomized
 * ************************************************/

void arrayRandomizer(char* arr[], int size){

	char* e;
	int f; 
	int g; 
	int h; 
	
	for(f = 0; f < 51; f++){

		g = randomInt(size, 0);
		h = randomInt(size, 0); 
			e = arr[g]; 
			arr[g] = arr[h]; 
			arr[h] = e;    
			
	}   


}


/***************************************************
 * Function: randomInt()
 * Parameters: int key 
 * Description: creates random integer
 * Preconditions: int declared
 * Post-conditions: random int generated
 * ************************************************/

int randomInt(int key, int key2){

	return (rand() % key) + key2; 

} 
/***************************************************
 * Function: _randomRooms()
 * Parameters: char pointer arrays and ints
 * Description: randomizes connections and checks to
 * ensure there is path between rooms  
 * Preconditions: char arrays declared and files 
 * opened.
 * Post-conditions:types and path addded to room files
 * ************************************************/
void _randomRooms(char* conarr[], char* rmarr[], int a, int d){

	int t, r;
	FILE* gFile;
	FILE* gFile1;
	char f, j; 
	char word[30], word1[30];	
			gFile = fopen(rmarr[a], "a+"); 
 	
			t = 0;	
			do {
				f = fscanf(gFile,"%s",word); 
						if (strcmp(word, conarr[d]) == 0)
						{
							t = 1; 	
						}
				
			} while (f != EOF );             
					if((conarr[d] != rmarr[a]) && (t == 0))
							{
								fprintf(gFile, "%s%s\n", "CONNECTION: ", conarr[d]); 
							  	gFile1 = fopen(conarr[d], "a+");
								r = 0;
									do{
										j = fscanf(gFile1,"%s",word1); 
										if(strcmp(word1, rmarr[a]) == 0)
										{
			 								r = 1;
										}
										
								}while (j != EOF); 		
										if(r == 0)
										{
											fprintf(gFile1, "%s%s\n", "CONNECTION: ", rmarr[a]);
										}								
								fclose(gFile1);
							}
						
						
			fclose(gFile);


}


/***************************************************
 * Function: lineCount() 
 * Parameters: char pointr
 * Description: counts number of lines in a file  
 * Preconditions: Files and file names exist
 * Post-conditions:proper line count returned
 * ************************************************/
int lineCount(char* filename){

FILE* fp; 
fp = fopen(filename, "r");

int lines = 0; 
char ch;  
while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    lines++;
  }
}
fclose(fp); 
return lines; 
}
 
/***************************************************
 * Function: setTempRoom()
 * Parameters: char pointer and FILE pointer 
 * Description: creates temp file to use during game-
 * play
 * Preconditions: char arrays declared and FILE 
 * opened
 * Post-conditions: file  properly created in 
 * directory with proper data stored in it 
 * ************************************************/

void setTempRoom(char* curRoom, FILE* tmp){

        FILE* fp;
        FILE* fp1;
        FILE* fp2;
        char str[80];
        char temp[80];
        char temp1[80];
        char token, tk;
        char* tester;
        int i;
	int j;

        fp = fopen(curRoom, "r");

        fp2 = tmpfile();

	j = lineCount(curRoom);

        for(i = 0; i < j; i++){

                 fgets(str, 80, fp);

                fp1 = tmpfile();
                fprintf(fp1, "%s", str);

                rewind(fp1);
	        do {
                        token = fscanf(fp1, "%s", temp);
                } while (token != EOF);

                fprintf(fp2, "%s\n", temp);
                close(fp1);

        }

        rewind(fp2);

  
	tk = fscanf(fp2, "%s", temp1);
       	do{	
		fprintf(tmp, "%s ", temp1); 
		tk = fscanf(fp2, "%s", temp1);
	}while(tk != EOF);


	fclose(fp);
        fclose(fp2);

}

/***************************************************
 * Function: checkRooms()
 * Parameters: FILE pointer and char string 
 * Description: checks to see if user entry is 
 * a possible connection
 * Preconditions: file opened and char string set. 
 * Post-conditions: check digit properly set. 
 * ************************************************/

int checkRooms(FILE* curFile, char chce[]){


char token;
char temp[10];
int check = 0;

rewind(curFile); 
               
	        do {
       		        token = fscanf(curFile, "%s", temp);

			if(strcmp(chce, temp) == 0)
				check = 1; 
			
		 } while (token != EOF);
	return check; 
}

/***************************************************
 * Function: RrintCurRoom()
 * Parameters: FILE and int pointers, int 
 * Description: prints current room and checks
 * to see if current room is end.
 * Preconditions: file opened and line count 
 * set. 
 * Post-conditions: file contents properly displayed
 * and check digit properly set 
 * ************************************************/

void printCurRoom(FILE* tempf, int e, int* check){

char temp1[80];
char end[8] = "END_ROOM";
char tk;
int d; 
    	 
	rewind(tempf); 
	for(d=0; d<e; d++){
 		if(d == 0)
		{
        		tk = fscanf(tempf, "%s", temp1);
	       		printf("\nCURRENT ROOM: %s\nPOSSIBLE CONNECTIONS: ", temp1);
		}
		else if(d > 0 && d < e - 1)
       		{
			tk = fscanf(tempf, "%s", temp1);
			printf("%s ", temp1);
		}
		else if(d == e-1)
       	 	{
			tk = fscanf(tempf, "%s", temp1);
   		//	printf("\nROOM TYPE: %s\n", temp1);
		}	
	if(*temp1 == *end)
	{
		*check = 1;
	}      
 }
	

}
