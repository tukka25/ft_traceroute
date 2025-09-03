/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha42 <abdamoha42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:48:24 by abdamoha42        #+#    #+#             */
/*   Updated: 2025/05/31 12:27:36 by abdamoha42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	final_printing_exit(struct timeval *stop, struct timeval *start,
		t_traceroute *tracert, int sockfd)
{
	int		total_time;
	float	loss_p;

	total_time = (((stop->tv_sec * 1000) + (stop->tv_usec / 1000))
			- ((start->tv_sec * 1000) + (start->tv_usec / 1000)));
	loss_p = 100.0 - (((float)tracert->recieved_packets
				/ tracert->transmitted_packets) * 100.0);
	printf("\n--- %s ft_traceroute statistics ---\n", tracert->dest_ip);
	printf("%d packets transmitted, %d received, %0.4f%% packet loss, time "
		"%d ms\n", tracert->transmitted_packets, tracert->recieved_packets, loss_p,
		total_time);
	printf("rtt min/avg/max/mdev = %f/%f/%f/%f ms\n", get_minimum(tracert),
		get_average(tracert), get_maximum(tracert), get_mdev(tracert));
	free(tracert->timings);
	free(tracert->buffer);
	free(tracert->packet);
	close(sockfd);
	error_handle(EXIT_SUCCESS, "", tracert);
}

void	init_packet_memory(t_traceroute *tracert)
{
	tracert->packet_size = sizeof(struct iphdr) + sizeof(struct icmphdr) + 56;
	tracert->packet = ft_calloc(1, tracert->packet_size + 1);
	if (!tracert->packet)
		packet_failure(tracert, "Error: Failed to allocate memory for packet");
	tracert->buffer = ft_calloc(1, tracert->packet_size + 1);
	if (!tracert->buffer)
		packet_failure(tracert, "Error: Failed to allocate memory for buffer");
	tracert->ip = (struct iphdr *)tracert->packet;
	tracert->icmp = (struct icmphdr *)(tracert->packet + sizeof(struct iphdr));
	tracert->ip_rep = convert_domain_to_ip(tracert->dest_ip, tracert);
}

void	setting_options(t_traceroute *tracert)
{
	struct timeval	timeout;
	int				sockopt;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	tracert->yes = 1;
	sockopt = setsockopt(tracert->sockfd, IPPROTO_IP, IP_HDRINCL, &tracert->yes,
			sizeof(tracert->yes));
	if (sockopt < 0)
	{
		printf("Error setsockopt\n");
		packet_failure(tracert, "Error: Failed to set socket options");
	}
	if (setsockopt(tracert->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			sizeof timeout) < 0)
		packet_failure(tracert, "Error: Failed to set socket options");
}
