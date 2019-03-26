#ifndef LIBNET_H
#define LIBNET_H
int sendUDPBroadcast(char *message, int taille_message, int port);
int sendUDPUnicast(char *address, char *message, int taille_message, int port);
int initialisationServeur(char *service);
int boucleServeur(int socket, void (*traitement)(void *));
#endif
