/************************************************
Name: Max Grubb 
File: keygen.c
Date: 3/14/2016
Description: Key Generator
**************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


//random integer generator 
int randomInt(int key, int key2){

	return rand() % (key2-key +1) + key; 

} 
int main (int argc, char *argv[]){

//set random time seed 
srand((unsigned)time(NULL));
int a, b;
char f;   
//get argument from command line 
sscanf(argv[1], "%d", &b);

//loop to generate random key and handle spaces 
for (a = 0; a < b; a++){
	

f = (char) randomInt(64, 90); 

if(f == '@')
{
	f = ' ';
}
printf("%c", f); 
  

} 
printf("\n"); 
 

return 0;


}
