#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <libnet.h>
#include <libthrd.h>

#include "pagehtml.h"
#include "comInterface.h"

#define MAX_CONNEXIONS 10
#define BUFFER_SIZE 1024

#define MAX_REQUEST_LENGTH 20
#define MAX_PATH_LENGTH 200
#define MAX_VERSION_LENGTH 20

#define MAX_CHAR_PAGE 1024

#define PORT_INTERFACE "2020"

// ----- Serveur HTTP -----

void gestionClientHTTP(int s, char *ip_src){
    (void) ip_src;

    FILE *dialogue = fdopen(s, "a+");
    if(dialogue==NULL){ perror("gestionClientHTTP.fdopen"); exit(-1); }
    
    char line[BUFFER_SIZE];
    char *success = fgets(line, BUFFER_SIZE, dialogue);
    if(success == NULL){
        fclose(dialogue);
        return;
    }
    
    char request[MAX_REQUEST_LENGTH];
    char path[MAX_PATH_LENGTH];
    char version[MAX_VERSION_LENGTH];
    int status = sscanf(line, "%s %s %s", request, path, version);
    if(status != 3) {
        fprintf(dialogue, "Requete HTTP invalide !");
        fclose(dialogue);
        return;
    }
    
    char page[MAX_CHAR_PAGE];
    for(int i=0; i<MAX_CHAR_PAGE; i++)
        page[i] = '\0';
    
    getPage(request, path, page);
    fprintf(dialogue, "%s", page);
    
    fclose(dialogue);
    return;
}

// ----- Serveur TCP -----
void gestionClientTCP(int s, char *ip_src){
    FILE *dialogue = fdopen(s, "a+");
    if(dialogue == NULL){ perror("gestionClientTCP.fdopen"); exit(-1); }
    
    char line[BUFFER_SIZE];
    char *success = fgets(line, BUFFER_SIZE, dialogue);
    if(success == NULL){
        fclose(dialogue);
        return;
    }

    int requete = ((line[0] & 0xFF) << 8) + (line[1] & 0xFF);
    traiteRequete(requete, ip_src);
    
    fclose(dialogue);
    return;
}

// ----- Serveur UDP -----
void gestionClientUDP(unsigned char *message, int message_length, char *ip_src){
    if(message_length < MIN_REQ_BYTE)
        return;

    int requete = ((message[0] & 0xFF) << 8) + (message[1] & 0xFF);
    traiteRequete(requete, ip_src);
}


void _boucleServeurHTTP(void *arg){
    int sHTTP = *((int *) arg);
    boucleServeurTCP(sHTTP, gestionClientHTTP);
}

void _boucleServeurUDP(void *arg){
    int sUDP = *((int *) arg);
    boucleServeurUDP(sUDP, gestionClientUDP);
}

int main(int argc, char *argv[]) {
    (void) argc;

    // --- Serveur HTTP (sonde <-> PC) ---
    int sHTTP = initialisationServeurTCP(argv[1]);
    lanceThread(_boucleServeurHTTP, (void *) (&sHTTP), sizeof(sHTTP));

    // --- Serveur UDP (sonde <-> interface) ---
    int sUDP = initialisationServeurUDP(PORT_INTERFACE);
    lanceThread(_boucleServeurUDP, (void *) (&sUDP), sizeof(sUDP));
    
    // --- Serveur TCP (sonde <-> interface) ---
    int sTCP = initialisationServeurTCP(PORT_INTERFACE);
    boucleServeurTCP(sTCP, gestionClientTCP);
}
