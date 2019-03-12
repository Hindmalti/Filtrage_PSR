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

    uint8_t mac[] = {0x00, 0x20, 0x20, 0x20, 0x20, 0x08};
    uint8_t ip[] = {172, 26, 145, 208};
    uint8_t gateway[] = {172, 26, 145, 44};
    uint8_t mask[] = {255, 255, 255, 0};
    ethernet_init(mac, ip, gateway, mask);

    SOCKET s = 1;
    DDRB = 0x20;
    int status = socket(s, Sn_MR_UDP, 2020, NULL);
    if(status <= 0){ printf("Error: echec de la creation de la socket\n"); }
    
    while(1){
        /*-------- SEND TO
        uint8_t ip_src[] = {172, 26, 145, 255};
        uint8_t data[BUFFER_SIZE] = "ST WICHMANN PENDANT 4H DEMAIN A 8H";
        int i;
        
        status = sendto(s, data, strlen(data), ip_src, 2020);
        if(status <= 0){ printf("Error: echec de la reception\n"); }
        
        printf("Envoyé !\n");
        _delay_ms(1000);*/
        
        /*-------- RECV FROM
        uint8_t ip_src[4];
        uint8_t data[BUFFER_SIZE];
        int i;
        for(i=0; i<BUFFER_SIZE; i++)
            data[i] = 0;
        
        status = recvfrom(s, data, BUFFER_SIZE, ip_src, 2020);
        if(status <= 0){ printf("Error: echec de la reception\n"); }
        
        printf("IP :%d.%d.%d.%d:\n", ip_src[0], ip_src[1], ip_src[2], ip_src[3]);
        printf("Recu :%s:\n", data);
        _delay_ms(100);*/
    }
    return 0;
}
