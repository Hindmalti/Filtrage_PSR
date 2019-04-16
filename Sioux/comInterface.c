#include <stdio.h>
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

int etats[NBR_INTERFACES];
int ips[NBR_INTERFACES];
void initTableauxInterface(){
    for(int i=0; i<NBR_INTERFACES; i++){
        etats[i] = -1;
        ips[i] = -1;
    }
}
int getEtatInterface(int index){
    return etats[index];
}
int getIpInterface(int index){
    return ips[index];
}

void broadCastGetStatus(){
    char message[REQUETE_LENGTH];
    message[0] = 0x00;
    message[1] = 0x00;
    sendUDPBroadcast(message, REQUETE_LENGTH, 2020);
}

static void retStatus(int requete){
    int idInterf = ((requete & DATA_MASK) >> 1) - 1; //-1 : [1, 13] => [0, 12]
    int mode = requete & 0x01;

    if(idInterf < 0 || idInterf > NBR_INTERFACES)
        return;
    etats[idInterf] = mode;
    //TODO recup ip ips[idInterf] = ip_src[3];
}

void traiteRequete(int requete){
    // determination de la commande
    int commandeId = (requete & COMMANDE_MASK) >> 13;
    
    switch(commandeId){
        case GET_STATUS:
            // on ignore
            break;
        case RET_STATUS:
            retStatus(requete);
            break;
        case SET_STATUS:
            // on ignore
            break;
        case GET_COMMANDE:
            // on ignore
            break;
        case RET_COMMANDE:
            
            break;
        case SET_COMMANDE:
            // on ignore
            break;
        default:
            // on ignore
            break;
    }
}
