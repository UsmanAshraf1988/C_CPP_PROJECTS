
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#define DEST_MAC0	0x00
#define DEST_MAC1	0x00
#define DEST_MAC2	0xc9
#define DEST_MAC3	0xd2
#define DEST_MAC4	0x71
#define DEST_MAC5	0xfa

#define MY_MAC0		0x00
#define MY_MAC1		0x00
#define MY_MAC2		0xc9
#define MY_MAC3		0xd2
#define MY_MAC4		0x71
#define MY_MAC5		0xfe

#define ETHER_TYPE	0x0800

#define DEFAULT_IF	"ens3f1"

#define BUF_SIZ		1024

int main(int argc, char *argv[])
{
	int sockfd, ret, i;
	int sockopt;
	ssize_t numbytes;
	uint8_t buf[BUF_SIZ];
	char ifName[IFNAMSIZ];
	
	struct ether_header *eh_back;
	struct ifreq if_idx;
	struct ifreq if_mac;
	struct sockaddr_ll socket_address;
	char sendbuf[BUF_SIZ];
	int tx_len =0;
	
	
		/* Get interface name */
	if (argc > 1)
		strcpy(ifName, argv[1]);
	else
		strcpy(ifName, DEFAULT_IF);
	
	
	/* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
		perror("listener: socket");	
		return -1;
	}
	
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	
	
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
	
	
	/* Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
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
	
	
	eh_back = (struct ether_header *) sendbuf;
	/* Ethernet header */
	eh_back->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh_back->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh_back->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh_back->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh_back->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh_back->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
        
	eh_back->ether_dhost[0] = DEST_MAC0;
	eh_back->ether_dhost[1] = DEST_MAC1;
	eh_back->ether_dhost[2] = DEST_MAC2;
	eh_back->ether_dhost[3] = DEST_MAC3;
	eh_back->ether_dhost[4] = DEST_MAC4;
	eh_back->ether_dhost[5] = DEST_MAC5;
	/* Ethertype field */
	eh_back->ether_type = htons(ETHER_TYPE);
	
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
	
    while(1){
	
	// Recieve
	numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
	
	//Send
	if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
	    printf("Send failed from receiver proc\n");
	
    }
	close(sockfd);
	//return ret;
	return 0;
}
