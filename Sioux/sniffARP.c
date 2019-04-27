#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>
#include <netinet/in.h>
#include <libthrd.h>

pcap_t *handle;
int count_per_sec = 0;

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
    (void) args;
    (void) header;
    (void) packet;

    count_per_sec++;
}

void boucle_pcap_loop(void *args){
    (void) args;

    pcap_loop(handle, 0, got_packet, NULL);
}

void sniff(void (*sniff_result)(int), char *dev){
    struct bpf_program fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 mask;
    bpf_u_int32 net;
    char filter_exp[] = "arp";
    
#ifdef DEBUG
    printf("pcap dev :%s:\n", dev);
#endif
    
    // open device
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL) {
#ifdef DEBUG
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
#endif
        exit(-1);
    }
    int status = pcap_lookupnet(dev, &net, &mask, errbuf);
    if(status == -1){
#ifdef DEBUG
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
#endif
        net = 0;
        mask = 0;
    }
    
    // filtering
    status = pcap_compile(handle, &fp, filter_exp, 0, net);
    if(status == -1){
#ifdef DEBUG
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
#endif
    }
    status = pcap_setfilter(handle, &fp);
    if(status == -1){
#ifdef DEBUG
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
#endif
    }
    
    // start sniffing
    lanceThread(boucle_pcap_loop, NULL, 0);

    // return result
    while(1){
        sleep(5);
        sniff_result(count_per_sec);
        count_per_sec = 0;
    }
}
