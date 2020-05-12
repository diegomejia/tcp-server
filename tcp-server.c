/*
    -- Author: Diego Mejia
    Copyright 2020 RedWall Studios LLC ©
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>

#define MAX 15 // Default values.
#define DEFAULT_PORT 11111
#define DEFAULT_BACKGLOG_SIZE 5

#define ERROR -1

void infinite(int connectionFD)
{
    char stringBuffer[MAX+1]; // Leave space for null.
    int  bytesRead = 0;
    int  msgCount  = 0;
    for(;;)
    {
        bytesRead = 0;
        bzero(stringBuffer, sizeof(stringBuffer));
        bytesRead = read (connectionFD, stringBuffer, sizeof(stringBuffer));

        if( bytesRead == ERROR){
          printf("read attempt failed\r\n");
          exit(0);
        } else {
          printf("from client: %s\r\n", stringBuffer);
          printf("echoing %lu bytes... [%d]\r\n", sizeof(char) * bytesRead, msgCount++);

          write(connectionFD, stringBuffer, sizeof(char) * bytesRead);

          if((strncmp(stringBuffer, "exit\r\n", 4)) == 0)
          {
              printf("tcp-server is shutting down...\r\n");
              break;
          }
        }
    }
}

int main(int argc, char *argv[])
{
    int socketFD, incomingConnectionFD;
    unsigned int  incomingAddressLength;
    struct sockaddr_in serverAddress;
    struct sockaddr_in incomingAddress;

    // Socket File Descriptor creation and verification.
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketFD == ERROR ){
        printf("socket creation failed...\r\n");
        exit(0);
    }
    else{
        printf("socket successfully created.\r\n");
    }

    bzero( &serverAddress, sizeof(serverAddress) );

    // Assign IP, PORT
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming address OK.
    serverAddress.sin_port        = htons(DEFAULT_PORT);

    // Connect the client socket to server socket.
    if ( bind(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != 0 ){
        printf("bind attempt failed...\r\n");
        exit(0);
    }
    else{
        printf("port bound!\r\n");
        printf("attempting to listen for connection attempts...\r\n");
        // Now server is ready to listen and verification
        if (listen(socketFD, DEFAULT_BACKGLOG_SIZE) == ERROR) {
          printf("listen attempt failed...\r\n");
          exit(0);
        }
        else{
          printf("server listening...\r\n");
          incomingAddressLength = sizeof(incomingAddress);
          incomingConnectionFD  = accept(socketFD,
                                         (struct sockaddr  * ) &incomingAddress,
                                         (       socklen_t * ) &incomingAddressLength);
          if ( incomingConnectionFD == ERROR ){
            printf("server accept failed...\r\n");
            exit(0);
          }else{
            printf("client connection request accepted!\r\n");
            infinite(incomingConnectionFD);
          }
        }
    }

    // close the socket
    close(socketFD);
}
