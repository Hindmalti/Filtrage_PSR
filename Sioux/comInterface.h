#ifndef COM_INTERFACE
#define COM_INTERFACE

#define NBR_INTERFACES 13
#define MIN_REQ_BYTE 2

void initTableauxInterface();
int getEtatInterface(int index);
char *getIpInterface(int index);
int getCommandeInterface(int index);

void broadCastGetStatus();
void broadCastGetCommande();

void traiteRequete(int requete, char *ip_src);

#endif
