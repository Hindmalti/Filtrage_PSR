#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <libnet.h>
#include "pagehtml.h"

#define MAX_CONNEXIONS 10
#define BUFFER_SIZE 1024

#define MAX_REQUEST_LENGTH 20
#define MAX_PATH_LENGTH 200
#define MAX_VERSION_LENGTH 20

#define MAX_CHAR_PAGE 1024

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
    
    char page[MAX_CHAR_PAGE];
    for(int i=0; i<MAX_CHAR_PAGE; i++)
        page[i] = '\0';
    
    getPage(request, path, page);
    fprintf(dialogue, page);
    
    fclose(dialogue);
    return;
}

static void _boucleServeur(void *args) { gestionClient(*((int *) args)); }

int main(int argc, char *argv[]) {
    /* Initialisation du serveur */
    int s = initialisationServeur(argv[1]);
    
    /* Lancement de la boucle d'ecoute */
    boucleServeur(s, _boucleServeur);
}
