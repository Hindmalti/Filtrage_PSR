#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>

#define SIZE_ETHERNET 14
#define ETHER_ADDR_LEN 6

struct sniff_arp {
    u_char arp_material[2];  //type de matériel (2 octets)
    u_char arp_proto[2];     //type de protocole (2 octets)
    u_char arp_mlen[1];      //longueur des adresse matériels (codé sur 1 octet)
    u_char arp_plen[1];      //longueur des adresses ip (codé sur 1 octet)
    uint16_t arp_operation;  //opération (2 octets)
    u_char arp_shost[ETHER_ADDR_LEN];//adresse MAC source (6 octets)
    struct in_addr arp_sip;  //adresse IP source (4 octets)
    u_char arp_dhost[ETHER_ADDR_LEN];//adresse MAC destination (6 octets)
    struct in_addr arp_dip;  //adresse IP destination (4 octets)
};

int main(void){
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    bpf_u_int32 mask;
    bpf_u_int32 net;
    char filter_exp[] = "arp";

    // search for default device
    dev = "bridge"; //pcap_lookupdev(errbuf);
    if(dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 2;
    }
    
    printf("Dev :%s:\n", dev);
    
    // open device
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 2;
    }
    int status = pcap_lookupnet(dev, &net, &mask, errbuf);
    if(status == -1){
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }
    
    // filtering
    status = pcap_compile(handle, &fp, filter_exp, 0, net);
    if(status == -1){
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 2;
    }
    status = pcap_setfilter(handle, &fp);
    if(status == -1){
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 2;
    }
    
    // sniffing
    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(handle, &header);
    const struct sniff_arp *arp = (struct sniff_arp*) (packet + SIZE_ETHERNET);

    // print
    printf("Jacked a packet with length of [%d]\n", header.len);
    printf("Dest :");
    for(int i=0; i<ETHER_ADDR_LEN; i++)
        printf("%02x:", arp->arp_dhost[i]);
    printf("\nSrc :");
    for(int i=0; i<ETHER_ADDR_LEN; i++)
        printf("%02x:", arp->arp_shost[i]);
    printf("\n");
    
    // close session
    pcap_close(handle);
    return 0;
}
