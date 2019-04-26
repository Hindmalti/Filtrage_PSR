#include <stdio.h>
#include <string.h>
#include <libnet.h>

#include "comInterface.h"

#define COMMANDE_MASK 0xE000
#define DATA_MASK 0x1FFF

#define GET_STATUS 0b000
#define RET_STATUS 0b001
#define SET_STATUS 0b010
#define GET_COMMANDE 0b011
#define RET_COMMANDE 0b100
#define SET_COMMANDE 0b101

#define MODE_EVEIL 1
#define MODE_SOMMEIL 0

#define REQUETE_LENGTH 2

#define IP_ADDR_CHAR_LENGTH 50

int etats[NBR_INTERFACES];
char ips[NBR_INTERFACES][IP_ADDR_CHAR_LENGTH];
int commandes[NBR_INTERFACES];
void initTableauxInterface(){
    for(int i=0; i<NBR_INTERFACES; i++){
        etats[i] = -1;
        commandes[i] = -1;
        for(int j=0; j<IP_ADDR_CHAR_LENGTH; j++)
            ips[i][j] = '\0';
    }
}
int getEtatInterface(int index){
    return etats[index];
}
char *getIpInterface(int index){
    return ips[index];
}
int getCommandeInterface(int index){
    return commandes[index];
}

void broadCastGetStatus(){
    char message[REQUETE_LENGTH];
    message[0] = 0x00;
    message[1] = 0x00;
    sendUDPBroadcast(message, REQUETE_LENGTH, 2020);
}
void broadCastGetCommande(){
    char message[REQUETE_LENGTH];
    message[0] = 0x60;
    message[1] = 0x00;
    sendUDPBroadcast(message, REQUETE_LENGTH, 2020);
}

static void retStatus(int requete, char *ip_src){
    int idInterf = ((requete & DATA_MASK) >> 1) - 1; //-1 : [1, 13] => [0, 12]
    int mode = requete & 0x01;

    // sauvegarde etat
    if(idInterf < 0 || idInterf > NBR_INTERFACES)
        return;
    etats[idInterf] = mode;

    // sauvegarde addr ip
    strcpy(ips[idInterf], ip_src);
}

static void retCommande(int requete, char *ip_src){
    int commande = requete & DATA_MASK;

    // recherche à qui est cette addr ip
    for(int i=0; i<NBR_INTERFACES; i++){
        if(strcmp(ips[i], ip_src) == 0){
            // enregistre la commande
            commandes[i] = commande;
            break;
        }
    }
}

void traiteRequete(int requete, char *ip_src){
    // determination de la commande
    int commandeId = (requete & COMMANDE_MASK) >> 13;

    switch(commandeId){
        case GET_STATUS:
            // on ignore
            break;
        case RET_STATUS:
            retStatus(requete, ip_src);
            break;
        case SET_STATUS:
            // on ignore
            break;
        case GET_COMMANDE:
            // on ignore
            break;
        case RET_COMMANDE:
            retCommande(requete, ip_src);
            break;
        case SET_COMMANDE:
            // on ignore
            break;
        default:
            // on ignore
            break;
    }
}
