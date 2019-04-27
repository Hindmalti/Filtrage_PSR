#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <libnet.h>
#include <libthrd.h>

#include "sniffARP.h"
#include "pagehtml.h"
#include "comInterface.h"

#define MAX_CONNEXIONS 10
#define BUFFER_SIZE 1024

#define MAX_REQUEST_LENGTH 20
#define MAX_PATH_LENGTH 200
#define MAX_VERSION_LENGTH 20

#define MAX_CHAR_PAGE 1024

#define PORT_INTERFACE "2020"

#define ID_INTERFACE 8

// ----- Serveur HTTP -----

void gestionClientHTTP(int s, char *ip_src){
    (void) ip_src;

    // recuperation de la requete
    FILE *dialogue = fdopen(s, "a+");
    if(dialogue==NULL){ perror("gestionClientHTTP.fdopen"); exit(-1); }
    
    char line[BUFFER_SIZE];
    char *success = fgets(line, BUFFER_SIZE, dialogue);
    if(success == NULL){
#ifdef DEBUG
        printf("Impossible de lire les données TCP\n");
#endif
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
    
#ifdef DEBUG
    printf("HTTP IN : %s", line); // don't put \n, line have that
#endif

    // generation et envoi de la page HTTP
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
    
    // recuperation de la requete
    char line[BUFFER_SIZE];
    char *success = fgets(line, BUFFER_SIZE, dialogue);
    if(success == NULL){
        fclose(dialogue);
        return;
    }
    int requete = ((line[0] & 0xFF) << 8) + (line[1] & 0xFF);

#ifdef DEBUG
    printf("TCP IN %s : 0x%04x\n", ip_src, requete);
#endif

    // traitement de la requete
    traiteRequete(requete, ip_src);
    
    fclose(dialogue);
    return;
}

// ----- Serveur UDP -----
void gestionClientUDP(unsigned char *message, int message_length, char *ip_src){
    if(message_length < MIN_REQ_BYTE)
        return;

    // recuperation de la requete
    int requete = ((message[0] & 0xFF) << 8) + (message[1] & 0xFF);

#ifdef DEBUG
    printf("UDP IN %s : 0x%04x\n", ip_src, requete);
#endif

    // traitement de la requete
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


void sniffer_result(int nARP){
    uniCastSetCommande(ID_INTERFACE - 1, nARP);
}
void _lanceSniffer(void * args){
    char *dev = (char *) args;

    sniff(sniffer_result, dev);
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("%s usage :\n%s <http_server_port> <device_to_sniff>\n", argv[0], argv[0]);
        exit(-1);
    }
    char *port_http = argv[1];
    char *pcap_dev = argv[2];

#ifdef DEBUG
    printf("Initialisation\n");
#endif

    initTableauxInterface();

#ifdef DEBUG
    printf("Lancement du sniffeur\n");
#endif

    // --- Sniffing packet ARP ---
    lanceThread(_lanceSniffer, (void *) pcap_dev, strlen(pcap_dev));
    
#ifdef DEBUG
    printf("Lancement du serveur HTTP\n");
#endif

    // --- Serveur HTTP (sonde <-> PC) ---
    int sHTTP = initialisationServeurTCP(port_http);
    lanceThread(_boucleServeurHTTP, (void *) (&sHTTP), sizeof(sHTTP));
    
#ifdef DEBUG
    printf("Lancement du serveur UDP\n");
#endif

    // --- Serveur UDP (sonde <-> interface) ---
    int sUDP = initialisationServeurUDP(PORT_INTERFACE);
    lanceThread(_boucleServeurUDP, (void *) (&sUDP), sizeof(sUDP));
    
#ifdef DEBUG
    printf("Lancement du serveur TCP\n");
#endif
    
    // --- Serveur TCP (sonde <-> interface) ---
    int sTCP = initialisationServeurTCP(PORT_INTERFACE);
    boucleServeurTCP(sTCP, gestionClientTCP);
}
