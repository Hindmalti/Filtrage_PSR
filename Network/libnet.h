#ifndef LIBNET_H
#define LIBNET_H
int sendUDPBroadcast(char *message, int port);
int sendUDPUnicast(char *address, char *message, int port);
int boucleServeur(int, void (*)(int));
#define MAX_TCP_CONNEXION 20
#endif
