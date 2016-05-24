/************************************************
Name: Max Grubb 
File: otp_denc_d.c
Date: 3/14/2016
Description: Server program to perform decryption 
**************************************************/

#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>
#include <fcntl.h>     
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 32768


int main(int argc, char** argv)
{
	socklen_t clilen;
    struct sockaddr_in server;
    struct sockaddr_in client;
    int grubbSocket, clientfd;
    int i;
    int kl, cl, bytesSent;
    int grubbpid;
    int portNum;
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    char buf3[BUFSIZE];

//argument validataion
    if (argc < 2)
    {
        printf("Usage: otp_dec_d PORT\n");
        exit(1);
    }
   
  //Create socket, bind and listen 
    if ((grubbSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket Error\n");
        exit(1);
    }
//clear memory for address information to be stored in struture 
    memset(&server, '\0', sizeof(server));
//get command line argument and pass to portNum
    sscanf(argv[1], "%d", &portNum);
//add address information to structure 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portNum);

// bind structure information to socket, then listen
    if (bind(grubbSocket, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Binding Error\n");
        exit(2);
    }


    if (listen(grubbSocket, 5) == -1)
    {
        printf("Listening Error\n");
        exit(2);
    }

    clilen = sizeof(client);

	
//loop for multiple process handling 	
    for(;;)
    {
		//acceptance of connection
        clientfd = accept(grubbSocket, (struct sockaddr *) &client, &clilen);
        if (clientfd < 0)
        {
            printf("Acceptance Error\n");
            continue;
        }

//Spawn Process - Receive, Decrypt and display text
		grubbpid = fork();

        if (grubbpid < 0)
        {
            perror("Process Fork Error\n");
        }

			int a, b, decrypted;
        if (grubbpid == 0)
        {
//clear data from buffer then read info from socket 
           memset(buf1, 0, BUFSIZE);

           cl = read(clientfd, buf1, BUFSIZE);
            if (cl < 0)
            {
                printf("Unable to read on port\n");
                exit(2);
            }

//write acknowledgement to socket
            bytesSent = write(clientfd, "1", 1);
            if (bytesSent < 0)
            {
                printf("Acknowledgement Error");
                exit(2);
            }

//clear data from buffer then read info from socket 
			
			memset(buf2, 0, BUFSIZE);

            kl = read(clientfd, buf2, BUFSIZE);
            if (kl < 0)
            {
                printf("Cannot Read On Port\n");
                exit(2);
            }

           
//Check for bad characters and file lengths 
			if (kl < cl)
			{ 
                printf("Need Longer Key\n");
                exit(1);
            }
	
            
            for (i = 0; i < cl; i++)
            {
                if ((int) buf1[i] > 90 || ((int) buf1[i] < 65 && (int) buf1[i] != 32))
                {
                    printf("Unable to Decrypt. TRY ALL CAPS\n");
                    exit(1);
                }
            }

            for (i = 0; i < kl; i++)
            {
                if ((int) buf2[i] > 90 || ((int) buf2[i] < 65 && (int) buf2[i] != 32))
                {
                    printf("Unable to decrypt. TRY ALL CAPS\n");
                    exit(1);
                }
            }
//Loop to handle decryption and space characters
           
            for (i = 0; i < cl; i++)
            {
                if (buf1[i] == ' ')
                {
                    buf1[i] = '@';
                }
                if (buf2[i] == ' ')
                {
                    buf2[i] = '@';
                }

                a = (int) buf1[i];
                b = (int) buf2[i];

                a = a - 64;
                b = b - 64;

                decrypted = a - b;
                if (decrypted < 0) 
                {
                    decrypted = decrypted + 27;
                }

                decrypted = decrypted + 64;

                buf3[i] = (char) decrypted + 0;

                if (buf3[i] == '@')
                {
                    buf3[i] = ' ';
                }
            }

//write decrypted information back to socket 

            bytesSent = write(clientfd, buf3, cl);
            if (bytesSent < cl)
            {
                printf("Socket write error\n");
                exit(2);
            }


            // close sockets
            close(clientfd);
            close(grubbSocket);

            exit(0);
        }

        else close(clientfd);
    } 

    return 0;
}
