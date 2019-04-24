#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "ethernet.h"
#include "w5100.h"
#include "socket.h"

#define MAC_SIZE 6
#define IPV4_SIZE 4
#define DEFAULT_PORT 2020

#define BUFFER_SIZE 4
#define TRAME_SIZE 2
#define INTERFACE_ID 8

#define COMMANDE_MASK 0xE000
#define DATA_MASK 0x1FFF

#define GET_STATUS 0b000
#define RET_STATUS 0b001
#define SET_STATUS 0b010
#define GET_COMMANDE 0b011
#define RET_COMMANDE 0b100
#define SET_COMMANDE 0b101

#define MODE_EVEIL 1
#define MODE_SOMMEIL 0
uint8_t status = MODE_EVEIL;

uint16_t commande = 0;
uint8_t next_socket_id = 1;

void sendTCP(SOCKET s, uint16_t message, uint8_t *ip_dest){
    // connect
    int status = connect(s, ip_dest, DEFAULT_PORT);
    if(status <= 0){ printf("Error: echec du connect\n"); }
    
    // send
    uint8_t buff[TRAME_SIZE];
    buff[0] = (message & 0xFF00) >> 8;
    buff[1] = message & 0x00FF;
    status = send(s, buff, TRAME_SIZE);
    if(status <= 0){ printf("Error: echec du send\n"); }
    
    // disconnect
    disconnect(s);
}

void traitementTrame(uint16_t data, uint8_t *ip_src, SOCKET sTCP){
    // determination de la commande
    int commandeId = (data & COMMANDE_MASK) >> 13;
    
    switch(commandeId){
        case GET_STATUS:
            sendTCP(sTCP,
                (RET_STATUS << 13)
                | (INTERFACE_ID << 1)
                | status, ip_src);
            break;
        case RET_STATUS:
            // on ignore
            break;
        case SET_STATUS:
            if((data & DATA_MASK) == MODE_EVEIL)
                status = MODE_EVEIL;
            else if((data & DATA_MASK) == MODE_SOMMEIL)
                status = MODE_SOMMEIL;
            else
                printf("Erreur setStatus : status inconnu :%d:", data & DATA_MASK);
            break;
        case GET_COMMANDE:
            sendTCP(sTCP,
                (RET_COMMANDE << 13)
                | (commande & DATA_MASK), ip_src);
            break;
        case RET_COMMANDE:
            // on ignore
            break;
        case SET_COMMANDE:
            commande = data & DATA_MASK;
            break;
        default:
            // on ignore
            break;
    }
}

int main(void){
    init_printf();

    // Ethernet
    uint8_t mac[] = {0x00, 0x20, 0x20, 0x20, 0x20, 0x08};
    uint8_t ip[] = {172, 26, 145, 208}; //{10, 0, 0, 2};
    uint8_t gateway[] = {172, 26, 145, 44}; //{10, 0, 0, 1};
    uint8_t mask[] = {255, 255, 255, 0};
    ethernet_init(mac, ip, gateway, mask);

    // socket UDP
    SOCKET sUDP = next_socket_id;
    next_socket_id++;
    int status = socket(sUDP, Sn_MR_UDP, DEFAULT_PORT, 0);
    if(status <= 0){ printf("Error: echec de la creation de la socket UDP\n"); }

    // socket TCP
    SOCKET sTCP = next_socket_id;
    next_socket_id++;
    status = socket(sTCP, Sn_MR_TCP, DEFAULT_PORT, 0);
    if(status <= 0){ printf("Error: echec de la creation de la socket reponse TCP\n"); }
    
    int recu;
    while(1){
        // Traitement UDP
        uint8_t data[BUFFER_SIZE];
        uint8_t ip_src[4];
        uint16_t port;
        int i;
        for(i=0; i<BUFFER_SIZE; i++)
            data[i] = 0;
        
        recu = recvfrom(sUDP, data, BUFFER_SIZE, ip_src, &port);
        if(recu > 0 && recu <= 4){
            printf("IP :%d.%d.%d.%d:\n", ip_src[0], ip_src[1], ip_src[2], ip_src[3]);
            printf("Recu : 0x%02x 0x%02x\n", data[0], data[1]);
            
            uint16_t data16 = (data[0] << 8) + data[1];
            traitementTrame(data16, ip_src, sTCP);
        }

        _delay_ms(100);
    }
    return 0;
}
