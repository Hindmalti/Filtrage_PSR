#ifndef LIBNET_H
#define LIBNET_H
int sendUDPBroadcast(char *message, int taille_message, int port);
int sendUDPUnicast(char *address, char *message, int taille_message, int port);
int initialisationServeur(char *service);
int boucleServeur(int socket, void (*traitement)(void *));

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);

#endif
