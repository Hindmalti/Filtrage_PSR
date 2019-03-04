#ifndef LIBNET_H
#define LIBNET_H
static void *_lanceThread(void *threadParam);
int lanceThread(void (*fonction)(void *), void *params, int taille_params);
#endif