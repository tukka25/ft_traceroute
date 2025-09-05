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

void	clean_and_exit(t_traceroute *tracert)
{

	free(tracert->timings);
	free(tracert->buffer);
	free(tracert->packet);
	close(tracert->sockfd);
	error_handle(EXIT_SUCCESS, "", tracert);
}

void	init_packet_memory(t_traceroute *tracert)
{
	tracert->packet_size = sizeof(struct iphdr) + sizeof(struct icmphdr) + 32;
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

char	*convert_ip_to_domain(char *ip)
{
	struct sockaddr_in sa;
    char *hostname;
    int res;

    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) <= 0) {
        perror("inet_pton failed");
        return NULL;
    }
    sa.sin_family = AF_INET;
    sa.sin_port = htons(0);
	hostname = malloc(NI_MAXHOST);
	if (!hostname)
		return NULL;
    res = getnameinfo((struct sockaddr *)&sa, sizeof(sa),
                      hostname, NI_MAXHOST,
                      NULL, 0, NI_NAMEREQD);

    if (res == 0) {
        return hostname;
    } else {
        fprintf(stderr, "getnameinfo failed: %s\n", gai_strerror(res));
        return NULL;
    }
}