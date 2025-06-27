#include "libft.h"
#include <stddef.h>

size_t	ft_darray_len(char **src)
{
	size_t i;

	i = 0;
	if (!src)
		return 0;
	while (src[i])
		i++;
	return i;
}