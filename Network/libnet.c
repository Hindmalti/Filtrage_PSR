#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "libnet.h"


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
    if(sendto(s, message, strlen(message), 0, (struct sockaddr *) &broadcast_address,
            sizeof(broadcast_address)) < 0 ) {
        perror("sendUDPBroadcast.sendto");
        exit(-1);
    }
    //Envoie du message grâce à la socket
}


int sendUDPUnicast(char *address, char *message, int port) {
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


// Fonction permettant de créer le serveur 
int initialisationServeur(char *service){
    struct addrinfo precisions, *resultat, *origine;
    int statut;
    int s;

    /* Construction de la structure adresse */
    memset(&precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_STREAM;
    precisions.ai_flags = AI_PASSIVE;
    statut = getaddrinfo(NULL, service, &precisions, &origine);
    if(statut < 0){ 
        perror("initialisationSocketUDP.getaddrinfo");
        exit(EXIT_FAILURE); 
    }
    struct addrinfo *p;
    for(p = origine, resultat = origine; p != NULL; p = p->ai_next) {
        if(p->ai_family == AF_INET6){ 
            resultat = p; 
            break;
        }
    }

    /* Creation d'une socket */
    s = socket(resultat->ai_family, resultat->ai_socktype, resultat->ai_protocol);
    if(s < 0){
        perror("initialisationSocketUDP.socket");
        exit(EXIT_FAILURE);
    }

    /* Options utiles */
    int vrai = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &vrai, sizeof(vrai))<0) {
        perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
        exit(-1);
    }

    /* Specification de l'adresse de la socket */
    statut = bind(s, resultat->ai_addr, resultat->ai_addrlen);
    if(statut<0) {
        perror("initialisationServeurUDP.bind");
        exit(-1);
    }

    /* Liberation de la structure d'informations */
    freeaddrinfo(origine);

/* Taille de la queue d'attente */
    statut = listen(s, MAX_TCP_CONNEXION);
    if(statut < 0)
        return -1;
    return s;
}

int boucleServeur(int socket, void (*traitement)(int)){
    while(1){
        int socket_dialogue = accept(socket, NULL, NULL);
        if(socket_dialogue < 0){
            perror("boucleServeur.accept");
            return -1;
        }
        traitement(socket_dialogue);
        return 0;
    }
}
