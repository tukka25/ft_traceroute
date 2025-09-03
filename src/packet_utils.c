/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha42 <abdamoha42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:26:56 by abdamoha42        #+#    #+#             */
/*   Updated: 2025/05/31 12:10:12 by abdamoha42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

unsigned short	calculate_checksum(unsigned short *icmp, int len)
{
	int			sum;
	u_short		*tmp_icmp;
	uint16_t	final;

	sum = 0;
	tmp_icmp = icmp;
	final = 0;
	while (len > 1)
	{
		sum += *tmp_icmp++;
		len -= 2;
	}
	if (len != 0)
		sum += *(uint8_t *)tmp_icmp;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	final = ~sum;
	return (final);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_is_running = 0;
}

void	add_timing(float g, t_traceroute *tracert)
{
	tracert->timings = realloc(tracert->timings,
			(tracert->index * (size_t) sizeof(float))
			+ (size_t) sizeof(float));
	tracert->timings[tracert->index] = g;
	tracert->index++;
}

void	packet_failure(t_traceroute *tracert, char *msg)
{
	free(tracert->buffer);
	free(tracert->packet);
	error_handle(EXIT_FAILURE, msg, tracert);
}

void	ip_icmp_initialization(struct iphdr *ip, struct icmphdr *icmp,
		t_traceroute *tracert, size_t packet_size)
{
	ip->version = 4;
	ip->tot_len = packet_size;
	ip->ttl = 1;
	ip->protocol = IPPROTO_ICMP;
	ip->daddr = inet_addr(tracert->ip_rep);
	ip->ihl = 5;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = getpid();
	icmp->un.echo.sequence = htons(1);
	icmp->checksum = calculate_checksum((unsigned short *)icmp,
			sizeof(struct icmphdr));
}
