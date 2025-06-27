#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H


#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include<signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "libft/libft.h"

# define FLAG_OPTIONS_NUMBER 1
# define MAX_FLAG_SIZE 14
// # define TIMEOUT 4

// extern int is_running;

// #define SIGINT  2   /* Interrupt the process */ 
// #define SIGQUIT 3   /* Quit the process */ 

typedef struct s_traceroute
{
	char	**flags_options;
	char	*dest_ip;
	char	*ip_rep;
	char	*packet;
	int		seq;
	struct icmphdr* icmp;
	struct iphdr* ip_reply;
	struct iphdr* ip;
	struct sockaddr_in sockadd;
	size_t packet_size;
	char	*buffer;
	float	*timings;
	int		index;
	int		yes;
	char	*flag;
	int		transmitted_packets;
	int		recieved_packets;
}	t_traceroute;


void	setup(t_traceroute *tracert);
void	error_handle(int status, char *msg, t_traceroute *tracert);
void	input_parsing(char **av, t_traceroute *tracert);
int		flag_validation(char **av, t_traceroute *tracert);
int		ip_validation(char **av, t_traceroute *tracert);
void	print_usage();

#endif