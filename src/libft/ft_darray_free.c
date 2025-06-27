#include "libft.h"

void ft_darray_free(char **darray)
{
	int i;

	if (!darray)
		return;
	i = 0;
	while (darray[i])
	{
		free(darray[i]);
		i++;
	}
	free(darray);
}
