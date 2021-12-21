#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h> 
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <pthread.h> //threading

void *connection_handler(void *);

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c, *new_sock;
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
        return 1;
    }
    puts("Bind complete\n");

    //Listen
    listen(socket_desc, 3);

    // Accept incoming connections
    puts("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*)&c)))
    {
        puts ("Connection accepted\n");
        
        // Reply to client
        message = "Hello Client! Your connection has been received. Now assigning you a handler...\n";
        write(new_socket, message, strlen(message));

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) <0)
        {
            perror("Could not create thread\n");
            return 1;
        }

        pthread_join(sniffer_thread, NULL);
        puts("Handler assigned");      

    }

    if (new_socket < 0)
    {
        perror("accept failed\n");
    }

    return 0;
}

//This will handle connection for each client
void *connection_handler(void *socket_desc)
{
    //get socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message, client_message[2000];

    //send messages to client
    message = "Hello! I am your connection handler, ";
    write(sock, message, strlen(message));

    message = " and it is my duty to communicate with you\n";
    write(sock, message, strlen(message));

    message = "Type something and I will repeat it back to you\n";
    write(sock, message, strlen(message));

    //Receive message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)   
    {
        //Send message back to client
        write(sock, client_message, strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    
    

    //free socket pointer
    free(socket_desc);

    return 0;
}
