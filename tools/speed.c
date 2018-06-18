/*
 * 列出网卡以及数据
 */
#include <stdio.h>
#include <linux/if_link.h> //struct rtnl_link_stats
#include <sys/types.h>
#include <ifaddrs.h> // getifaddrs / freeifaddrs
#include <netdb.h> // NI_MAXHOST & NI_NUMERICHOST

int main(int argc,char **argv)
{
  struct ifaddrs *links,*ifa;
  char host[NI_MAXHOST];
  if(getifaddrs(&links)){
    perror("getifaddrs()");
    return -1;
  }
  ifa=links;
  while(ifa){
    if (ifa->ifa_addr){
      int family = ifa->ifa_addr->sa_family;
      int err;
      printf("%-8s %s(%d)\n",
	     ifa->ifa_name,
	     (family == AF_PACKET) ? "AF_PACKET" :
	     (family == AF_INET) ? "AF_INET" :
	     (family == AF_INET6) ? "AF_INET6" : "???",
	     family);
      switch(family){
      case AF_INET:
      case AF_INET6:
	err = getnameinfo(ifa->ifa_addr,
			      (family == AF_INET) ? sizeof(struct sockaddr_in): sizeof(struct sockaddr_in6),
			      host,NI_MAXHOST,
			      NULL,0,NI_NUMERICHOST);
	if (err){
	  printf("getnameinfo() failed: %s\n",gai_strerror(err));
	  return -1;
	}
	printf("\taddress: [%s]\n",host);
	break;
      case AF_PACKET:
	if(ifa->ifa_data){
	  struct rtnl_link_stats *stats = ifa->ifa_data;
	printf("\ttx_packets\t= %10u; rx_packets\t= %10u\n"
	       "\ttx_bytes\t= %10u; rx_bytes\t\t= %10u\n",
	       stats->tx_packets,stats->rx_packets,
	       stats->tx_bytes,stats->rx_bytes);
	}
      }
    }
    ifa=ifa->ifa_next;
  }
  freeifaddrs(links);
  return 0;
}
