#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "ethernet.h"
#include "w5100.h"
#include "socket.h"

#define MAC_SIZE	6
#define IPV4_SIZE	4

#define BUFFER_SIZE 30

int main(void){
    init_printf();

    // Ethernet
    uint8_t mac[] = {0x00, 0x20, 0x20, 0x20, 0x20, 0x08};
    uint8_t ip[] = {172, 26, 145, 208};
    uint8_t gateway[] = {172, 26, 145, 44};
    uint8_t mask[] = {255, 255, 255, 0};
    ethernet_init(mac, ip, gateway, mask);

    // socket UDP
    SOCKET sUDP = 1;
    int status = socket(sUDP, Sn_MR_UDP, 2020, NULL);
    if(status <= 0){ printf("Error: echec de la creation de la socket UDP\n"); }
    
    // socket TCP
    SOCKET sTCP = 2;
    status = socket(sTCP, Sn_MR_TCP, 2020, NULL);
    if(status <= 0){ printf("Error: echec de la creation de la socket TCP\n"); }
    
    
    while(1){
        // Traitement UDP
        /*uint8_t ip_src[4];
        uint8_t data[BUFFER_SIZE];
        int i;
        for(i=0; i<BUFFER_SIZE; i++)
            data[i] = 0;
        
        status = recvfrom(sUDP, data, BUFFER_SIZE, ip_src, 2020);
        if(status <= 0){ printf("Error: echec de la reception UDP\n"); }
        
        printf("IP :%d.%d.%d.%d:\n", ip_src[0], ip_src[1], ip_src[2], ip_src[3]);
        printf("Recu :%s:\n", data);*/
        
        // Traitement TCP
        status = listen(sTCP);
        if(status <= 0){ printf("Error: echec du listen\n"); }
        
        status = accept(sTCP);
        if(status <= 0){ printf("Error: echec de l'accept\n"); }
        
        uint8_t data[BUFFER_SIZE];
        int i;
        for(i=0; i<BUFFER_SIZE; i++)
            data[i] = 0;
        status = recv(sTCP, data, BUFFER_SIZE);
        if(status <= 0){ printf("Error: echec de la reception TCP\n"); }
        
        printf("Recu :%s:\n", data);
        
        _delay_ms(100);
    }
    return 0;
}
