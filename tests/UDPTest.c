#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define SERVER_PORT = 2020
#define CLIENT_PORT = 2020
#define MAX_BUFFER = 200

int main(void){
    // socket creation
    int sock = socket(PF_INET6, SOCK_DGRAM, 0);
    
    // bind
    struct sockaddr_in6 serverAddress;
    socklen_t size = sizeof serverAddress;
    
    serverAddress.sin6_family=AF_INET6;
    serverAddress.sin6_addr=in6addr_any;
    serverAddress.sin6_port=htons(SERVER_PORT);
    serverAddress.sin6_flowinfo=0;
    serverAddress.sin6_scope_id=0;
    
    int status = bind(sock, (struct sockaddr *) &serverAddress, size);
    if(statut<0) { perror("bind"); exit(-1); }
    
    // sendto
    struct sockaddr_in6 clientAddress;
    socklen_t size = sizeof clientAddress;
    
    clientAddress.sin6_family=AF_INET6;
    clientAddress.sin6_addr=; //TODO this
    clientAddress.sin6_port=htons(CLIENT_PORT);
    clientAddress.sin6_flowinfo=0;
    clientAddress.sin6_scope_id=0;
    
    char buffer[MAX_BUFFER] = "looooooool\0";
    sendto(sock, buffer, strlen(buffer), 0, );
    return 0;
}
