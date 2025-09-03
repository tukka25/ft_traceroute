#include "ft_traceroute.h"

/*
This function sets up the environment to run the program.
*/
void setup(t_traceroute *tracert)
{
	char *s[FLAG_OPTIONS_NUMBER + 1] = {"--help", NULL};

	ft_bzero(tracert, sizeof(t_traceroute));
	tracert->flags_options = ft_darray_copy(s);
	if (!tracert->flags_options)
		error_handle(EXIT_FAILURE, "Error: Failed to copy flags options", tracert);
}

/*
The function prints %msg in %fd and exits incase of an error occurred.
*/
void error_handle(int status, char *msg, t_traceroute *tracert)
{
	if (!msg)
		exit(EXIT_FAILURE);
	
	if (status == EXIT_FAILURE)
		fprintf(stderr, "%s\n", msg);
	
	// Clean up resources
	ft_darray_free(tracert->flags_options);
	free(tracert->flag);
	free(tracert->dest_ip);
	
	exit(status);
}

/*
This function prints the usage of the program
*/
void print_usage()
{
	printf("Usage: ft_traceroute [OPTION...] HOST\n");
	printf("Print the route packets trace to network host.\n\n");
	printf("Options:\n");
	printf("  --help                 display this help and exit\n\n");
	printf("Examples:\n");
	printf("  ft_traceroute google.com\n");
	printf("  ft_traceroute --help\n\n");
}

float	get_minimum(t_traceroute *tracert)
{
	float	min;
	int		i;

	i = 1;
	if (!tracert->timings || tracert->index == 0)
		return (0);
	min = tracert->timings[0];
	while (i < tracert->index)
	{
		if (tracert->timings[i] < min)
			min = tracert->timings[i];
		i++;
	}
	return (min);
}

float	get_average(t_traceroute *tracert)
{
	float	avg;
	int		i;

	i = 0;
	avg = 0;
	while (i < tracert->index)
	{
		avg += tracert->timings[i];
		i++;
	}
	if (avg == 0)
		return (0);
	return (avg / tracert->index);
}