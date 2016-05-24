/************************************************
Name: Max Grubb 
File: opt_denc.c
Date: 3/14/2016
Description: Client program to perform decryption 
**************************************************/

#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>     
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 32768

int main(int argc, char** argv)
{
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    char ackresp[1];
    int fp;
    int i;
    int kl;
    int bytesReceived;
    int bytesSent;
    int tl;
    int grubbSocket;
	int portNum;
    struct sockaddr_in serv_addr;
    struct hostent *server;



    if (argc < 4)
    {
        printf("Usage: otp_enc <plaintextfile> <keyfile> port\n");
        exit(1);
    }

//Open file from argument 1 -- textfile 
    fp = open(argv[1], O_RDONLY);

  
		if (fp < 0)
		{
			printf("Cannot open your plaintext file\n");
			exit(1);
		}

		tl = read(fp, buf1, BUFSIZE);

//checking for bad characters 
		
    for (i = 0; i < tl - 1; i++)
    {
        if ((int) buf1[i] > 90 || ((int) buf1[i] < 65 && (int) buf1[i] != 32))
        {
            printf("Unable to encrypt certain characters in your text file. Please use UPPERCASE");
            exit(1);
        }
    }

    close(fp);

//opening key file 
    fp = open(argv[2], O_RDONLY);

    if (fp < 0)
    {
        printf("Cannot open key file\n");
        exit(1);
    }

    kl = read(fp, buf2, BUFSIZE);

//checking for bad characters 
	
    for (i = 0; i < kl - 1; i++)
    {
        if ((int) buf2[i] > 90 || ((int) buf2[i] < 65 && (int) buf2[i] != 32))
        {
            printf("Key is corrupt\n");
            exit(1);
        }
    }


    close(fp);

    if (kl < tl)
    {
        printf("You need a longer key for effective encryption", argv[2]);
  
	}

/*Create Soccket and Connect to Encryption Server*/ 

    grubbSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (grubbSocket < 0)
    {
        printf("Socket Error\n");
        exit(2);
    }

//Clearing memory of structure so we can add address information 	
    memset(&serv_addr, '\0', sizeof(serv_addr));

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        printf("Unable to connect\n");
        exit(2);
    }    
  
  
  //Set to Ipv4
      serv_addr.sin_family = AF_INET;

 //Populate address structures
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);         

//Get port num and add to structure 		 
	sscanf(argv[3], "%d", &portNum);
    serv_addr.sin_port = htons(portNum);


//Connecting to server 
	
    if (connect(grubbSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Error\n");
        exit(2);
    }
//writing information into socket to be sent to server
    bytesSent = write(grubbSocket, buf1, tl - 1);
    if (bytesSent < tl - 1)
    {
        printf("Sending Error\n");
        exit(2);
    }

    memset(ackresp, 0, 1);

//Reading from socket acknowledgment. 	
    bytesReceived = read(grubbSocket, ackresp, 1);
    if (bytesReceived < 0)
    {
       printf("Receive Error\n");
       exit(2);
    }

//Write key information to socket to be sent to server
    bytesSent = write(grubbSocket, buf2, kl - 1);
    if (bytesSent < kl - 1)
    {
        printf("Sending Error\n");
        exit(2);
    }

    memset(buf1, 0, BUFSIZE);

    do
    {
 //read encrypted response from socket, sent by server    
        bytesReceived = read(grubbSocket, buf1, tl - 1);
    }
    while (bytesReceived > 0);

    if (bytesReceived < 0)
    {
       printf("Receipt Error\n");
       exit(2);
    }

 //print one character at a time then follow with a newline 
    for (i = 0; i < tl - 1; i++)
    {
        printf("%c", buf1[i]);
    }

    printf("\n");

    // close socket
    close(grubbSocket);
    return 0;
}
