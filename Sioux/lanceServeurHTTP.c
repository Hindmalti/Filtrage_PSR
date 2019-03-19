#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <libthrd.h>
#include "pageHTML.h"

#define MAX_CONNEXIONS 10
#define BUFFER_SIZE 1024

#define MAX_REQUEST_LENGTH 20
#define MAX_PATH_LENGTH 200
#define MAX_VERSION_LENGTH 20

int initialisationServeur(short int *port) {
    int s;
    struct sockaddr_in adresse;
    socklen_t taille = sizeof adresse;
    int statut;

    /* Creation d'une socket */ 
    s = socket(PF_INET,SOCK_STREAM, 0);
    if(s<0){
        perror("initialisationServeur.socket");
        exit(-1);
    }

    /* Specification de l'adresse de la socket */
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(*port);
    statut = bind(s, (struct sockaddr *)&adresse, sizeof(adresse));
    if(statut<0) return -1;

    /* On recupere le numero du port utilise */
    statut = getsockname(s, (struct sockaddr *)&adresse, &taille);
    if(statut<0){
        perror("initialisationServeur.getsockname");
        exit(-1);
    }
    *port = ntohs(adresse.sin_port);

    /* Taille de la queue d'attente */
    statut = listen(s, MAX_CONNEXIONS);
    if(statut<0) return -1;

    return s;
}

void gestionClient(int s){
    FILE *dialogue = fdopen(s, "a+");
    if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(-1); }
    
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
    
    fprintf(dialogue, getPage(request, path));
    
    fclose(dialogue);
    return;
}

static void _threadClient(void *args) { gestionClient(*((int *) args)); }
int boucleServeur(int socket_ecoute) {
    int dialogue;
    while(1){
        /* Attente d'une connexion */
        if((dialogue = accept(socket_ecoute, NULL, NULL)) < 0){ perror("accept error\n"); return -1; }
        /* Traitement */
        lanceThread(_threadClient, (void *) (&dialogue), 1);
    }
}

int main(int argc,char *argv[]) {
    if(argc < 2) { printf("Need one argument !\n"); exit(-1); }
    short port = 0;
    for(unsigned int i=0; i<strlen(argv[1]); i++){
        if(argv[1][i] < '0' || argv[1][i] > '9') { printf("Argument must be a number !\n"); exit(-1); }
        port = port * 10 + (argv[1][i] - '0');
    }
    
    int s;
    
    /* Initialisation du serveur */
    s = initialisationServeur(&port);
    
    /* Lancement de la boucle d'ecoute */
    boucleServeur(s);
}
