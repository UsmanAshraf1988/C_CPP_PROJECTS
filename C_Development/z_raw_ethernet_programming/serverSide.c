
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

#define DEST_MAC0	0x00
#define DEST_MAC1	0x00
#define DEST_MAC2	0xc9
#define DEST_MAC3	0xd2
#define DEST_MAC4	0x71
#define DEST_MAC5	0xfe

//#define DEFAULT_IF	"enp1s0"
#define DEFAULT_IF	"ens3f0"
#define BUF_SIZ		1024

#define ETHER_TYPE	0x0800

#define RECORD_ARRAY_SIZE 100

typedef enum {false=0, true=1} bool;

//long get_nanos();
uint64_t GetTimeStamp();

void perror(const char * msg)
{
	fprintf(stdout, "%s", msg);
}


int main(int argc, char *argv[])
{
	
	time_t proc_start_time, proc_end_time;
	
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	int tx_len = 0;
	char sendbuf[BUF_SIZ];
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];
	int sockopt;
	
	/* Get interface name */
	if (argc > 1)
		strcpy(ifName, argv[1]);
	else
		strcpy(ifName, DEFAULT_IF);
	
	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
	    perror("socket creation");
	}
	
	/* Get the index of the interface */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
	
	/* Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt) ) == -1) {
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	/* Bind to device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
		perror("SO_BINDTODEVICE");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	
	
	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
        
	eh->ether_dhost[0] = DEST_MAC0;
	eh->ether_dhost[1] = DEST_MAC1;
	eh->ether_dhost[2] = DEST_MAC2;
	eh->ether_dhost[3] = DEST_MAC3;
	eh->ether_dhost[4] = DEST_MAC4;
	eh->ether_dhost[5] = DEST_MAC5;
	/* Ethertype field */
	eh->ether_type = htons(ETHER_TYPE);
	tx_len += sizeof(struct ether_header);

	/* Packet data */
	sendbuf[tx_len++] = 0xde;
	sendbuf[tx_len++] = 0xad;
	sendbuf[tx_len++] = 0xbe;
	sendbuf[tx_len++] = 0xef;
	
	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = DEST_MAC0;
	socket_address.sll_addr[1] = DEST_MAC1;
	socket_address.sll_addr[2] = DEST_MAC2;
	socket_address.sll_addr[3] = DEST_MAC3;
	socket_address.sll_addr[4] = DEST_MAC4;
	socket_address.sll_addr[5] = DEST_MAC5;
	
	
	void* buffer1 = malloc(BUF_SIZ); /*Buffer for ethernet frame*/
	int lengthBytes = 0; /*length of the received frame*/ 
	
	int outter_i =0;
	int outter_j =0;
	int outter_k = 0;
	
        int minDiffTime=0;
	int measuretime=0;
	
	bool frstVal = false;
	  
	
	for(; outter_i<RECORD_ARRAY_SIZE; outter_i++){
	  
	  sleep(2); //to make this thread sleep for few seconds to make sure clearing of cache and so on, for new experiment.
	  frstVal = true;
	  minDiffTime = 0;
	  outter_j = 0;
	  
	  for(; outter_j<100; outter_j++){
	    
	    
	    measuretime = 0;
	    outter_k = 0;
	    
	    proc_start_time = GetTimeStamp();
	    
	      for(; outter_k<1000; outter_k++)/*while(1)*/{
		    /* Send packet */
		    ssize_t status = sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
		    
		    if (status < 0)
			perror("Send failed from send proc\n");
		    
		    lengthBytes = recvfrom(sockfd, buffer1, BUF_SIZ, 0, NULL, NULL);
		    if (lengthBytes < 0) { perror("packet recieve  failed from send proc\n"); }
		    //printf("listener: got packet %lu bytes\n", lengthBytes);
	      }//for outter_k
	      
	    proc_end_time = GetTimeStamp(); // SFP Execution time has been recorded.
	    
	    //printf("Executation time for \'%d\' packets is: %1d\n", outter_k, (proc_end_time-proc_start_time) );
	    measuretime = proc_end_time-proc_start_time;
	    if(frstVal)
	    {
	      minDiffTime = measuretime;
	      frstVal = false;
	    }
	    
	    if(minDiffTime > measuretime){
	      minDiffTime = measuretime;
	    }
	    
	  }//for outter_j
	  
	  printf("Eperiment %d: Minimum Executation time out of %d iterations for \"%d packets send and recieve\" is: %1d\n", outter_i+1, outter_j, outter_k, minDiffTime );
	  
	}// for outter_i
	
      
        delete buffer1;
	
	return 0;
}


uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}


