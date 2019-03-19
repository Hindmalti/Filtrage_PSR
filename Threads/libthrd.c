#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ThreadParam {
    void (*fonction)(void*);
    void *params;
};

static void *_lanceThread(void *threadParam){
    struct ThreadParam *threadParam2 = threadParam;
    threadParam2->fonction(threadParam2->params);
    free(threadParam2->params);
    free(threadParam2);
}

void lanceThread(void (*fonction)(void *), void *params, int taille_params){
    pthread_t tid;
    
    struct ThreadParam *threadParam = malloc(sizeof(struct ThreadParam));
    threadParam->fonction = fonction;
    threadParam->params = malloc(taille_params);
    memcpy(threadParam->params, params, taille_params);
    
    pthread_create(&tid, NULL, _lanceThread, threadParam);
    pthread_detach(tid);
}

