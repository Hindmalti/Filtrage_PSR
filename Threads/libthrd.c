#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct ThreadParam {
    void (*fonction)(void*); //Pointeur de fonction : void f (void* arg)
    void *params;
};

static void *_lanceThread(void *threadParam) {
    struct ThreadParam *threadParam2 = threadParam;
    threadParam2->fonction(threadParam2->params);
    free(threadParam2->params);
    free(threadParam2);
    return NULL;
}

/**
 * \fn int lanceThread(void (*fonction)(void*), void* arg, int taille)
 * \brief Fonction de lancement de thread.
 *
 * \param fonction Pointeur de fonction de la fonction à executer.
 * \param arg Pointeur générique représentant la donnée à passer au thread.
 * \param taille Taille de la valeur pointée par arg.
 * \return 0 si aucune erreur, -1 sinon.
 */
int lanceThread(void (*fonction)(void *), void *params, int taille_params) {
    pthread_t tid;
    //Malloc dans le heap pour y acceder depuis les deux threads
    struct ThreadParam *threadParam = malloc(sizeof(struct ThreadParam));
    threadParam->fonction = fonction;
    threadParam->params = malloc(taille_params);
    memcpy(threadParam->params, params, taille_params);
    if (pthread_create(&tid, NULL, _lanceThread, threadParam) != 0) {
        perror("lanceThread.pthread_create");
        return 1;
    }
    pthread_detach(tid);
    return 0;
}

