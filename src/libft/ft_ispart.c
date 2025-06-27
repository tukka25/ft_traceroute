#include "libft.h"

int		ft_ispart(char **darray, char *str)
{
	int i = 0;

	if (!darray || !str)
		return 1;
	while (darray[i])
	{
		if (strcmp(darray[i], str) == 0)
			return 0;
		i++;
	}
	return 1;
}