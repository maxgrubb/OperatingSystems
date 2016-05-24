/************************************************
Name: Max Grubb 
File: opt_enc.c
Date: 3/14/2016
Description: Client program to perform encryption 
**************************************************/
#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>     
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/types.h>


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

//check to validate command line arguments
	
    if (argc < 4)
    {
        printf("Usage: otp_enc <plaintextfile> <keyfile> port\n");
        exit(1);
    }

//open file for encryption	

    fp = open(argv[1], O_RDONLY);

  
		if (fp < 0)
		{
			printf("Error: cannot open your plaintext file\n");
			exit(1);
		}

		tl = read(fp, buf1, BUFSIZE);
//check for bad characters 
    for (i = 0; i < tl - 1; i++)
    {
        if ((int) buf1[i] > 90 || ((int) buf1[i] < 65 && (int) buf1[i] != 32))
        {
            printf("Unable to encrypt certain characters in your text file. Please use UPPERCASE");
            exit(1);
        }
    }

    close(fp);
//open key file and check for errors 
    fp = open(argv[2], O_RDONLY);

    if (fp < 0)
    {
        printf("Error: cannot open key file %s\n", argv[2]);
        exit(1);
    }

    kl = read(fp, buf2, BUFSIZE);

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

//Zero Memory and get localhost name
    memset(&serv_addr, '\0', sizeof(serv_addr));

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        printf("Unable to connect\n");
        exit(2);
    }    
  
  //IPv4 selection
    serv_addr.sin_family = AF_INET;

//set up address structures 

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);         
		 
//get port from commandline and add to address structure 		 
	sscanf(argv[3], "%d", &portNum);
    serv_addr.sin_port = htons(portNum);

//connect to encryption server	
    if (connect(grubbSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Error\n");
        exit(2);
    }

//write text to socket 	
    bytesSent = write(grubbSocket, buf1, tl - 1);
    if (bytesSent < tl - 1)
    {
        printf("Sending Error\n");
        exit(2);
    }

	
    memset(ackresp, 0, 1);
//receive acknowledgement from server
    bytesReceived = read(grubbSocket, ackresp, 1);
    if (bytesReceived < 0)
    {
       printf("Receive Error\n");
       exit(2);
    }
//write key to socket, to be sent to server
    bytesSent = write(grubbSocket, buf2, kl - 1);
    if (bytesSent < kl - 1)
    {
        printf("Sending Error\n");
        exit(2);
    }

	
//zero memory then read info from socket 	
    memset(buf1, 0, BUFSIZE);

    do
    {
     
        bytesReceived = read(grubbSocket, buf1, tl - 1);
    }
    while (bytesReceived > 0);

    if (bytesReceived < 0)
    {
       printf("Receipt Error\n");
       exit(2);
    }
//print details to stdout
    
    for (i = 0; i < tl - 1; i++)
    {
        printf("%c", buf1[i]);
    }

    printf("\n");

//close socket and free structure
	
    close(grubbSocket);
	return 0;
}