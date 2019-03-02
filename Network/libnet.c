#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<stdlib.h>
#include<netinet/udp.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>


//Fonction permettant d'envoyer en broadcast un message 
int sendUDPBroadcast(char *message, int port) {
    int broadcast_enable = 1;
    //Option broadcast ON
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if(s < 0){
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1); 
    }
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        //Mise option broadcast à la socket
        perror("sendUDPBroadcast.setsockopt");
        exit(-1);
    }
    struct sockaddr_in broadcast_address;
    //Creation structure adresse de destination
    memset(&broadcast_address, 0, sizeof(broadcast_address));
    //Mise à zéro de la structure d'adresse
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_port = htons(port);
    //Mise du port en ordre du réseau (big endian)
    broadcast_address.sin_addr.s_addr = INADDR_BROADCAST;
    //Adresse = 255.255.255.255 
    if(sendto(s, message, strlen(message), 0, (struct sockaddr *) &broadcast_address, sizeof(broadcast_address)) < 0 ) {
        perror("sendUDPBroadcast.sendto");
        exit(-1);
    }
    //Envoie du message grâce à la socket
}

int sendUDPUnicast(char *address, char *message, int port){
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if(s < 0){
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1); 
    }
    struct sockaddr_in unicast_address;
    //Creation structure adresse de destination
    memset(&unicast_address, 0, sizeof(unicast_address));
    //Mise à zéro de la structure d'adresse
    unicast_address.sin_family = AF_INET;
    unicast_address.sin_port = htons(port);
    //Mise du port en ordre du réseau (big endian)
    unicast_address.sin_addr.s_addr = inet_addr(address);
    if(sendto(s, message, strlen(message), 0, (struct sockaddr *) &unicast_address, sizeof(unicast_address)) < 0 ) {
        perror("sendUDPBroadcast.sendto");
        exit(-1);
    }
}