#ifndef LIBTHRD_H
#define LIBTHRD_H
void lanceThread(void (*fonction)(void *), void *params, int taille_params);
#endif
