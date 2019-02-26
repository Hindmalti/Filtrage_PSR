#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct bpf_program fp;
pcap_t *handle;
int count_per_sec = 0;

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
    count_per_sec++;
}

void *sniff(void *args){
    pcap_loop(handle, 0, got_packet, NULL);
    return NULL;
}

void interrupt(int sig){
    if(sig == SIGINT){
        // close session
        pcap_freecode(&fp);
        pcap_close(handle);
        printf("\nClosed...");
        exit(0);
    }
}

int main(void){
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 mask;
    bpf_u_int32 net;
    char filter_exp[] = "arp";
    
    struct sigaction action;
    action.sa_handler = interrupt;
    sigaction(SIGINT, &action, NULL);

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
    
    // start sniffing loop
    pthread_t thread;
    status = pthread_create(&thread, NULL, sniff, NULL);
    if(status == -1){
        fprintf(stderr, "Couldn't pthread\n");
        return 2;
    }
    
    // print
    while(1){
        sleep(1);
        printf("Number of packet per seconds :%d:\n", count_per_sec);
        count_per_sec = 0;
    }
    
    return 0;
}
