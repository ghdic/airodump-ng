#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>

#define REQ_CNT 20


void convrt_mac( const char *data, char *cvrt_str, int sz );

int main( void )
{
     int sockfd, cnt, req_cnt = REQ_CNT;
     char mac_adr[128] = {0x00,};
     struct sockaddr_in *sock;
     struct ifconf ifcnf_s;
     struct ifreq *ifr_s;

     sockfd = socket( PF_INET , SOCK_DGRAM , 0 );
     if( sockfd < 0 ) {
          perror( "socket()" );
          return -1;
     }

     memset( (void *)&ifcnf_s , 0x0 , sizeof(ifcnf_s) );
     ifcnf_s.ifc_len = sizeof(struct ifreq) * req_cnt;
     ifcnf_s.ifc_buf = malloc(ifcnf_s.ifc_len);
     if( ioctl( sockfd, SIOCGIFCONF, (char *)&ifcnf_s ) < 0 ) {
          perror( "ioctl() - SIOCGIFCONF" );
          return -1;
     }

     if( ifcnf_s.ifc_len > (sizeof(struct ifreq) * req_cnt) ) {
          req_cnt = ifcnf_s.ifc_len;
          ifcnf_s.ifc_buf = realloc( ifcnf_s.ifc_buf, req_cnt );
     }

     ifr_s = ifcnf_s.ifc_req;
     for( cnt = 0 ; cnt < ifcnf_s.ifc_len ; cnt += sizeof(struct ifreq), ifr_s++ )
     {
          if( ioctl( sockfd, SIOCGIFFLAGS, ifr_s ) < 0 ) {
               perror( "ioctl() - SIOCGIFFLAGS" );
               return -1;
          }

          if( ifr_s->ifr_flags & IFF_LOOPBACK )
               continue;

          sock = (struct sockaddr_in *)&ifr_s->ifr_addr;
          printf( "\n<IP address> - %s\n" , inet_ntoa(sock->sin_addr) );

          if( ioctl( sockfd, SIOCGIFHWADDR, ifr_s ) < 0 ) {
               perror( "ioctl() - SIOCGIFHWADDR" );
               return -1;
          }
          convrt_mac( ether_ntoa((struct ether_addr *)(ifr_s->ifr_hwaddr.sa_data)), mac_adr, sizeof(mac_adr) -1 );
          printf( "<MAC address> - %s\n" , mac_adr );
     }
     return 0;
}

void convrt_mac( const char *data, char *cvrt_str, int sz )
{
     char buf[128] = {0x00,};
     char t_buf[8];
     char *stp = strtok( (char *)data , ":" );
     int temp=0;

     do
     {
          memset( t_buf, 0x0, sizeof(t_buf) );
          sscanf( stp, "%x", &temp );
          snprintf( t_buf, sizeof(t_buf)-1, "%02X", temp );
          strncat( buf, t_buf, sizeof(buf)-1 );
          strncat( buf, ":", sizeof(buf)-1 );
     } while( (stp = strtok( NULL , ":" )) != NULL );

     buf[strlen(buf) -1] = '\0';
     strncpy( cvrt_str, buf, sz );
}
