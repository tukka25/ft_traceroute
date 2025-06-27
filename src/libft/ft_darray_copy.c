#include "libft.h"

char **ft_darray_copy(char **src)
{
	int i;
	char **ret;

	i = 0;
	if (!src)
		return NULL;
	ret = malloc((ft_darray_len(src) + 1) * sizeof(char *));
	if (!ret)
		return NULL;
	while (src[i])
	{
		ret[i] = ft_strdup(src[i]);
		i++;
	}
	ret[i] = NULL;
	return ret;
}