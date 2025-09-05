#include "ft_traceroute.h"

int			g_is_running = 1;

int	create_socket(t_traceroute *tracert)
{
	int	sockfd;

	tracert->sockadd.sin_family = AF_INET;
	tracert->sockadd.sin_port = 0;
	tracert->sockadd.sin_addr.s_addr = inet_addr(tracert->ip_rep);
	sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
		packet_failure(tracert, "Error: Failed to create raw socket");
	return (sockfd);
}

void	next_traceroute_setup(t_traceroute *tracert)
{
	struct in_addr	src_addr;

	src_addr.s_addr = tracert->ip->daddr;
	// usleep(1000000);
	tracert->ip->ttl = ++tracert->ttl;
	tracert->seq++;
	tracert->icmp->un.echo.sequence = htons(tracert->seq);
	tracert->icmp->checksum = 0;
	tracert->icmp->checksum = calculate_checksum((unsigned short *)tracert->icmp,
			sizeof(struct icmphdr));
	// printf("next daddr = %s\n", inet_ntoa(src_addr));
	// printf("ip_rep = %s\n,ip_reply = %s", tracert->ip_rep, inet_ntoa(src_addr));
	// if (tracert->ip_rep == inet_ntoa(src_addr))
	// {
	// 	printf("Reached To %s\n", tracert->ip_rep);
	// 	g_is_running = 0;
	// }
}

void	init_packet_send(t_traceroute *tracert, t_packet *packet)
{
	init_packet_memory(tracert);
	ip_icmp_initialization(tracert->ip, tracert->icmp, tracert, tracert->packet_size);
	tracert->sockfd = create_socket(tracert);
	flag_options_printing(tracert);
	setting_options(tracert);
	signal(SIGINT, handle_sigint);
	gettimeofday(&packet->start_total, NULL);
}

// static void print_packet_analysis(t_traceroute *ping)
// {
//     printf("=== PACKET ANALYSIS ===\n");
    
//     // Cast packet buffer to structures for analysis
//     struct iphdr *ip = (struct iphdr *)ping->packet;
//     struct icmphdr *icmp = (struct icmphdr *)(ping->packet + sizeof(struct iphdr));
    
//     // IP Header Analysis (from raw packet)
//     printf("IP Header (from packet buffer):\n");
//     printf("  Version: %d\n", ip->version);
//     printf("  Header Length: %d (bytes: %d)\n", ip->ihl, ip->ihl * 4);
//     printf("  Total Length: %d\n", ntohs(ip->tot_len));
//     printf("  TTL: %d\n", ip->ttl);
//     printf("  Protocol: %d (ICMP=%d)\n", ip->protocol, IPPROTO_ICMP);
//     printf("  Destination: %s\n", inet_ntoa(*(struct in_addr*)&ip->daddr));
    
//     // ICMP Header Analysis (from raw packet)
//     printf("ICMP Header (from packet buffer):\n");
//     printf("  Type: %d (ECHO_REQUEST=%d)\n", icmp->type, ICMP_ECHO);
//     printf("  Code: %d\n", icmp->code);
//     printf("  ID: %d\n", icmp->un.echo.id);
//     printf("  Sequence: %d\n", ntohs(icmp->un.echo.sequence));
//     printf("  Checksum: 0x%04x\n", icmp->checksum);
    
//     // Raw packet hex dump
//     printf("Raw packet bytes (%zu total):\n", ping->packet_size);
//     for (size_t i = 0; i < ping->packet_size; i++) {
//         printf("%02x ", (unsigned char)ping->packet[i]);
//         if ((i + 1) % 16 == 0)
//             printf("\n");
//     }
//     if (ping->packet_size % 16 != 0)
//         printf("\n");
    
//     // Data section
//     printf("Data section: %zu bytes (should be 56)\n", 
//            ping->packet_size - sizeof(struct iphdr) - sizeof(struct icmphdr));
    
//     printf("=======================\n");
// }

// static void print_packet_hex(t_traceroute *ping)
// {
//     printf("=== PACKET DEBUG ===\n");
//     printf("Packet size: %zu bytes\n", ping->packet_size);
//     printf("Raw packet data:\n");
    
//     for (size_t i = 0; i < ping->packet_size; i++) {
//         printf("%02x ", (unsigned char)ping->packet[i]);
//         if ((i + 1) % 16 == 0)
//             printf("\n");
//     }
//     if (ping->packet_size % 16 != 0)
//         printf("\n");
//     printf("===================\n");
// }

void	handle_send(t_traceroute *tracert, t_packet *packet)
{

	tracert->transmitted_packets += 1;
	tracert->addr_len = sizeof(tracert->sockadd);
	if (tracert->hit != 0)
		tracert->prev_station = tracert->ip_reply->saddr;
	tracert->recv_f = recvfrom(tracert->sockfd, tracert->buffer, tracert->packet_size, 0,
			(struct sockaddr *)&tracert->sockadd,
			(unsigned int *restrict) & tracert->addr_len);
	// printf("recv_f = %d , buffer = %s, strlen =%zu\n", tracert->recv_f, tracert->buffer, strlen(tracert->buffer));
	// perror("recvfrom");
	// printf("\n");
	// printf("ip = %s\n", tracert->ip_rep);
	tracert->ip_reply = (struct iphdr *)tracert->buffer;
	tracert->icmp_reply = (struct icmphdr *)(tracert->buffer + sizeof(struct iphdr));

	// printf("IP header length: %d bytes\n", tracert->ip_reply->ihl * 4);
	// printf("TTL: %d\n", tracert->ip_reply->ttl);
	// struct in_addr addr;
	// addr.s_addr = tracert->ip_reply->daddr;
	// char *ip_string = inet_ntoa(addr);
	// printf("IP DADDR: %s\n", ip_string);
	// addr.s_addr = tracert->ip_reply->saddr;
	// ip_string = inet_ntoa(addr);
	// printf("IP SADDR: %s\n", ip_string);
	// printf("Protocol: %d\n", tracert->ip_reply->protocol);

	// printf("ICMP type = %d\n", tracert->icmp_reply->type);
	// printf("ICMP code = %d\n", tracert->icmp_reply->code);
	// printf("ICMP checksum = %x\n", ntohs(tracert->icmp_reply->checksum));

	gettimeofday(&packet->stop, NULL);
	tracert->elapsed_time = (((packet->stop.tv_sec * 1000)
					+ (packet->stop.tv_usec / 1000)) - ((packet->start.tv_sec
						* 1000) + (packet->start.tv_usec / 1000)));
	tracert->recieved_packets += 1;
	packet_reply_printing(tracert);
	// if (tracert->recv_f > 0)
	// {
	// 	tracert->elapsed_time = (((packet->stop.tv_sec * 1000)
	// 				+ (packet->stop.tv_usec / 1000)) - ((packet->start.tv_sec
	// 					* 1000) + (packet->start.tv_usec / 1000)));
	// 	tracert->recieved_packets += 1;
	// 	// add_timing(tracert->elapsed_time, tracert);
	// 	// packet_reply_printing(tracert->icmp_reply->type, tracert->recv_f,
	// 	// 	tracert->elapsed_time, tracert);
	// }
}

// static void print_timings(t_traceroute *ping)
// {
//     if (!ping->timings || ping->index <= 0)
//     {
//         printf("No timings available\n");
//         return;
//     }
    
//     printf("Timings array (%d entries):\n", ping->index);
//     for (int i = 0; i < ping->index; i++)
//     {
//         printf("  [%d]: %.3f ms\n", i, ping->timings[i]);
//     }
// }

void	packet_send(t_traceroute *tracert)
{
	t_packet	packet;

	tracert->hit = 0;
	init_packet_send(tracert, &packet);
	printf("ft_traceroute to %s (%s), 30 hops max, 60 bytes packets\n", tracert->dest_ip , tracert->ip_rep);
	while (g_is_running)
	{
		// printf("ttl = %d\n", tracert->ip->ttl);
		while (tracert->hit < 3)
		{
			gettimeofday(&packet.start, NULL);
			tracert->sendt = sendto(tracert->sockfd, tracert->packet, tracert->packet_size, 0,
				(struct sockaddr *)&tracert->sockadd, sizeof(struct sockaddr));
			if (tracert->sendt > 0)
				handle_send(tracert, &packet);
			tracert->hit++;
		}
		printf("\n");
		// printf("--- before ---");
		// print_timings(tracert);
		// printf("--- after ---");
		// print_timings(tracert);
		// ft_bzero(tracert->timings, sizeof(tracert->timings));
		tracert->index = 0;
		tracert->hit = 0;
		next_traceroute_setup(tracert);
	}
	gettimeofday(&packet.stop_total, NULL);
	clean_and_exit(tracert);
}
