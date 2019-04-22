#ifndef LIBNET_H
#define LIBNET_H

#include <stdint.h>

int sendUDPBroadcast(char *message, int taille_message, int port);
int sendUDPUnicast(char *address, char *message, int taille_message, int port);

int initialisationServeur(char *service);
int boucleServeur(int socket, void (*traitement)(int, uint32_t));

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);

#endif
