#ifndef COM_INTERFACE
#define COM_INTERFACE

#define NBR_INTERFACES 13

void initTableauxInterface();
int getEtatInterface(int index);
int getIpInterface(int index);
void broadCastGetStatus();
void traiteRequete(int requete);

#endif