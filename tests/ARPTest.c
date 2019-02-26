#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>

#define SIZE_ETHERNET 14

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

/* Ethernet header */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
    u_short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
    u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
    u_char  ip_tos;                 /* type of service */
    u_short ip_len;                 /* total length */
    u_short ip_id;                  /* identification */
    u_short ip_off;                 /* fragment offset field */
    #define IP_RF 0x8000            /* reserved fragment flag */
    #define IP_DF 0x4000            /* dont fragment flag */
    #define IP_MF 0x2000            /* more fragments flag */
    #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
    u_char  ip_ttl;                 /* time to live */
    u_char  ip_p;                   /* protocol */
    u_short ip_sum;                 /* checksum */
    struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
    #define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
    #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};

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
    for(int i=0; i<6; i++)
        printf("%02x:", arp->arp_dhost[i]);
    printf("\nSrc :");
    for(int i=0; i<6; i++)
        printf("%02x:", arp->arp_shost[i]);
    printf("\n");
    
    // close session
    pcap_close(handle);
    return 0;
}
