/************************************************
Name: Max Grubb 
File: opt_enc_d.c
Date: 3/14/2016
Description: Server program to perform encryption 
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
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t clilen;
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
        printf("Usage: otp_enc_d PORT\n");
        exit(1);
    }
   
    // create socket 
    if ((grubbSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket Error\n");
        exit(1);
    }

//clear memory of address structures then put argument into portNum	
    memset(&server, '\0', sizeof(server));
    sscanf(argv[1], "%d", &portNum);

//Set structure to hode address information	
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portNum);

//Bind structure to socket 
    if (bind(grubbSocket, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Binding Error\n");
        exit(2);
    }


//Listen, with 5 potential connections 
    if (listen(grubbSocket, 5) == -1)
    {
        printf("Listening Error\n");
        exit(2);
    }

    clilen = sizeof(client);

//loop for multiple connections	
    for(;;)
    {
        //accept connection
		clientfd = accept(grubbSocket, (struct sockaddr *) &client, &clilen);
        if (clientfd < 0)
        {
            printf("Acceptance Error\n");
            continue;
        }

		grubbpid = fork();

        if (grubbpid < 0)
        {
            perror("Process Fork Error\n");
        }
//Spawn process to handle recipt of text and key, and resend encrypted text 
        if (grubbpid == 0)
        {
		   int a, b, encrypted;  //ints for encryption
           memset(buf1, 0, BUFSIZE);
//read from socket 
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
       
            memset(buf2, 0, BUFSIZE);
//read key file from socket 
            kl = read(clientfd, buf2, BUFSIZE);
            if (kl < 0)
            {
                printf("Cannot Read On Port\n");
                exit(2);
            }

           
 //check again for bad characters and good file lengths 
			if (kl < cl)
            { 
                printf("Need Longer Key\n");
                exit(1);
            }
            
			for (i = 0; i < cl; i++)
            {
                if ((int) buf1[i] > 90 || ((int) buf1[i] < 65 && (int) buf1[i] != 32))
                {
                    printf("Unable to Encrypt Character Set. Try all CAPS\n");
                    exit(1);
                }
            }

            for (i = 0; i < kl; i++)
            {
                if ((int) buf2[i] > 90 || ((int) buf2[i] < 65 && (int) buf2[i] != 32))
                {
                    printf("Unable to Encrypt Character Set. Try all CAPS\n");
                    exit(1);
                }
            }

          
//loop to handle spaces, type conversion, and char reset 
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

                encrypted = (a + b) % 27;

                encrypted = encrypted + 64;

                buf3[i] = (char) encrypted + 0;

                if (buf3[i] == '@')
                {
                    buf3[i] = ' ';
                }
            }

//write encrypted string back to socket to be sent to client 
            bytesSent = write(clientfd, buf3, cl);
            if (bytesSent < cl)
            {
                printf("Encryption return error");
                exit(2);
            }

//shut down sockets
            close(clientfd);
            close(grubbSocket);

            exit(0);
        }

        else close(clientfd);
    } 

    return 0;
}
