#include<stdio.h>
#include<libnet.h>

int main(void){
    sendUDPBroadcast("coucou", 4430);

    return 0;
}