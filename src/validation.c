#include "ft_traceroute.h"

void input_parsing(char **av, t_traceroute *tracert)
{
	if (!av)
		error_handle(EXIT_FAILURE, "Invalid Parameters", tracert);
	if (flag_validation(av, tracert) == 1)
		error_handle(EXIT_FAILURE, "Invalid flag", tracert);
	if (tracert->flag)
	{
		if (strcmp(tracert->flag ,"-?") == 0)
		{
			print_usage();
			error_handle(EXIT_SUCCESS, "", tracert);
		}
	}
	if (ip_validation(av, tracert) == 1)
		error_handle(EXIT_FAILURE, "Invalid IP Schema", tracert);
	if (!tracert->flag)
		printf("OPTION --------------> No Flag\n");
	else
		printf("OPTION --------------> %s\n", tracert->flag);
	
}

/*
	This function validates the Flag and checks if there is an empty arg
*/
int flag_validation(char **av, t_traceroute *tracert)
{
	int counter;

	counter = 1;
	if (!av)
		error_handle(EXIT_FAILURE, "Invalid Parameters", tracert);
	while (av[counter])
	{
		if (!*av[counter])
			return 1;
		if (strchr(av[counter], '-') != NULL)
		{
			if (ft_ispart(tracert->flags_options, av[counter]) == 0)
			{
				tracert->flag = ft_strdup(av[counter]);
				return 0;
			}
			else
			{
				return 1;
			}
		}
		counter++;
	}
	return 0;
}


int ip_validation(char **av, t_traceroute *tracert)
{
	int counter;

	counter = 1;
	if (!av)
		error_handle(EXIT_FAILURE, "Invalid Parameters", tracert);
	while (av[counter])
	{
		if (!*av[counter])
			return 1;
		if (!tracert->flag)
		{
			tracert->dest_ip = ft_strdup(av[counter]);
			return 0;
		}
		else if (strcmp(tracert->flag, av[counter]))
		{
			tracert->dest_ip = ft_strdup(av[counter]);
			return 0;
		}
		counter++;
	}
	return 1;
}