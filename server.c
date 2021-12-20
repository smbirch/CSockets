#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket!\n");
    }

    //Prepare sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc,(struct sockaddr *) &server, sizeof(server)) < 0)    
    {
        puts("Bind failed!\n");
    }
    puts("Bind complete\n");

    //Listen
    listen(socket_desc, 3);

    // Accept incoming connections
    puts("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*)&c);

    if (new_socket < 0)
    {
        perror("accept failed\n");
    }

    puts ("Connection accepted\n");

    // Reply to client
    message = "Hello Client! Your connection has been received, but I have to go now. Goodbye\n";
    write(new_socket, message, strlen(message));

    return 0;

    
}