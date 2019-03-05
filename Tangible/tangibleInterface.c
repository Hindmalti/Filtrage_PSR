#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

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
    
    uint8_t ip_src[] = {172, 26, 145, 44};
    while(1){
        uint8_t data[BUFFER_SIZE];
        int i;
        for(i=0; i<BUFFER_SIZE; i++)
            data[i] = 0;
        
        status = recvfrom(s, data, BUFFER_SIZE, ip_src, 2020);
        if(status <= 0){ printf("Error: echec de la reception\n"); }
        
        printf("Recu :%s:\n", data);
        _delay_ms(1000);
    }
    return 0;
}

/*
 * uint16 recvfrom(
	SOCKET s, 	/**< the socket number 
	uint8 * buf, 	/**< a pointer to copy the data to be received 
	uint16 len, 	/**< the data size to read 
	uint8 * addr, 	/**< a pointer to store the peer's IP address 
	uint16 *port	/**< a pointer to store the peer's port number. 
	)
*/
