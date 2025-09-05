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

void	packet_reply_printing(t_traceroute *tracert)
{
	struct in_addr	src_addr;
	struct in_addr	prev_src_addr;
	char *ip_domain;

	if (tracert->icmp_reply->type == 0 || tracert->ttl >= 30)
	{
		// printf("done");
		g_is_running = 0;
	}
	else if (tracert->recv_f > 0)
	{
		src_addr.s_addr = tracert->ip_reply->saddr;
		prev_src_addr.s_addr = tracert->prev_station;
		if (tracert->hit == 0)
		{
			printf("%d ", tracert->ttl);
			// ip_domain = convert_ip_to_domain(inet_ntoa(src_addr));
			// if (ip_domain == NULL)
			ip_domain = inet_ntoa(src_addr);
			printf("%s (%s) ", ip_domain, inet_ntoa(src_addr));
			printf("%.3lf ms ", tracert->elapsed_time);
			
		}
		else
		{
			if (inet_ntoa(src_addr) == inet_ntoa(prev_src_addr))
			{
				printf("%.3lf ms ", tracert->elapsed_time);
			}
			else
			{
				printf("%s (%s) ", inet_ntoa(src_addr), inet_ntoa(src_addr));
				printf("%.3lf ms ", tracert->elapsed_time);
			}
		}

	}
	else if (tracert->recv_f < 0)
	{
		printf("* ");
	}
	// usleep(100);
}
