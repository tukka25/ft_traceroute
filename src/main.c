#include "ft_traceroute.h"

int main(int ac, char **av)
{
	t_traceroute tracert;

	if (ac >= 2 && ac <= 3)
	{
		setup(&tracert);
		input_parsing(av, &tracert);
		// packet_send(&tracert);
		return (0);
	}
	print_usage();
	return (1);
}