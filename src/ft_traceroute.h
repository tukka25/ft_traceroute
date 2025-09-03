#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H


# include "./libft/libft.h"
# include <arpa/inet.h>
# include <errno.h>
# include <math.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/in_systm.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# define FLAG_OPTIONS_NUMBER 1
# define MAX_FLAG_SIZE 14
# define TIMEOUT 4

extern int				g_is_running;

# define SIGINT 2 /* Interrupt the process */
# define SIGQUIT 3 /* Quit the process */

typedef struct s_packet
{
	struct timeval	stop;
	struct timeval	start;
	struct timeval	stop_total;
	struct timeval	start_total;
}	t_packet;

typedef struct s_traceroute
{
	char				**flags_options;
	int					recv_f;
	float				elapsed_time;
	int					sendt;
	int					sockfd;
	int					addr_len;
	struct icmphdr		*icmp_reply;
	char				*dest_ip;
	char				*ip_rep;
	char				*packet;
	int					seq;
	struct icmphdr*		icmp;
	struct iphdr*		ip_reply;
	struct iphdr*		ip;
	struct sockaddr_in	sockadd;
	size_t				packet_size;
	char				*buffer;
	float				*timings;
	int					index;
	int					yes;
	char				*flag;
	int					transmitted_packets;
	int					recieved_packets;
}	t_traceroute;


void	setup(t_traceroute *tracert);
void	error_handle(int status, char *msg, t_traceroute *tracert);
void	input_parsing(char **av, t_traceroute *tracert);
int		flag_validation(char **av, t_traceroute *tracert);
int		ip_validation(char **av, t_traceroute *tracert);
void	print_usage();
void	packet_send(t_traceroute *tracert);
void	next_traceroute_setup(t_traceroute *tracert);
int		create_socket(t_traceroute *tracert);
void	next_traceroute_setup(t_traceroute *tracert);
void	init_packet_send(t_traceroute *tracert, t_packet *packet);
void	handle_send(t_traceroute *tracert, t_packet *packet);
unsigned short	calculate_checksum(unsigned short *icmp, int len);
void	handle_sigint(int sig);
void	add_timing(float g, t_traceroute *tracert);
void	packet_failure(t_traceroute *tracert, char *msg);
void	ip_icmp_initialization(struct iphdr *ip, struct icmphdr *icmp,
		t_traceroute *tracert, size_t packet_size);
void	setting_options(t_traceroute *tracert);
void	init_packet_memory(t_traceroute *tracert);
void	final_printing_exit(struct timeval *stop, struct timeval *start,
		t_traceroute *tracert, int sockfd);
float	get_maximum(t_traceroute *tracert);
float	get_mdev(t_traceroute *tracert);
char	*convert_domain_to_ip(char *domain, t_traceroute *tracert);
void	flag_options_printing(t_traceroute *tracert);
void	packet_reply_printing(int type, int recv_f, float elapsed_time,
		t_traceroute *tracert);
float	get_average(t_traceroute *tracert);
float	get_minimum(t_traceroute *tracert);




#endif