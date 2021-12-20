#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);  //creates IPV4 TCP socket
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("142.250.217.110");
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );

    //Connect to remote server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    puts("Connected...\n");

    //Sending data 
    message = "GET / HTTP/1.1\r\n\r\n";
    if (send(socket_desc, message, strlen(message), 0) <0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data sent!\n");

    //Get reply from server
    if (recv(socket_desc, server_reply, 2000, 0) < 0)   
    {
        puts("Receive failed");
    }
    puts("Reply received!\n");
    puts(server_reply);
    

    return 0;
}
