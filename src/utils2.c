/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdamoha42 <abdamoha42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:46:12 by abdamoha42        #+#    #+#             */
/*   Updated: 2025/05/31 12:26:53 by abdamoha42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

float	get_maximum(t_traceroute *tracert)
{
	float	max;
	int		i;

	i = 1;
	if (!tracert->timings || tracert->index == 0)
		return (0);
	max = tracert->timings[0];
	while (i < tracert->index)
	{
		if (tracert->timings[i] > max)
			max = tracert->timings[i];
		i++;
	}
	return (max);
}

float	get_mdev(t_traceroute *tracert)
{
	float	mdev;
	int		i;

	i = 0;
	mdev = 0;
	while (i < tracert->index)
	{
		mdev += fabs(tracert->timings[i] - get_average(tracert));
		i++;
	}
	if (mdev == 0)
		return (0);
	return (mdev / tracert->index);
}

char	*convert_domain_to_ip(char *domain, t_traceroute *tracert)
{
	struct hostent	*h;
	char			error_msg[256];

	h = gethostbyname(domain);
	if (!h)
	{
		snprintf(error_msg,
			sizeof(error_msg),
			"ft_traceroute: \
			%s: Temporary failure in name resolution",
			domain);
		packet_failure(tracert, error_msg);
	}
	return (inet_ntoa(*(struct in_addr *)h->h_addr));
}

void	flag_options_printing(t_traceroute *tracert)
{
	if (tracert->flag)
	{
		if (strcmp(tracert->flag, "-v") == 0)
		{
			fprintf(stdout, "\ntracert %s (%s): 56 data bytes, id 0x%x = %d\n",
				tracert->dest_ip, tracert->ip_rep, tracert->icmp->un.echo.id,
				tracert->icmp->un.echo.id);
		}
	}
}

void	packet_reply_printing(int type, int recv_f, float elapsed_time,
		t_traceroute *tracert)
{
	struct in_addr	src_addr;

	src_addr.s_addr = tracert->ip_reply->saddr;
	if (type == 3)
		printf("Destination Unreachable\n");
	else if (type == 5)
		printf("Redirect\n");
	else if (type == 11)
		printf("%ld bytes from %s: Time to live exceeded\n", recv_f
			- sizeof(struct iphdr), inet_ntoa(src_addr));
	else if (type == 12)
		printf("Parameter Problem\n");
	else
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n", recv_f
			- sizeof(struct iphdr), inet_ntoa(src_addr), tracert->seq,
			tracert->ip_reply->ttl, elapsed_time);
	add_timing(elapsed_time, tracert);
}
