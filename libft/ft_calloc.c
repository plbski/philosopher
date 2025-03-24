/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbuet <pbuet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:38:52 by gvalente          #+#    #+#             */
/*   Updated: 2025/03/18 15:16:15 by pbuet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*bites;

	if (count == 0 || size == 0)
		return (malloc(0));
	bites = malloc(count * size);
	if (bites == NULL)
		return (NULL);
	ft_bzero(bites, count * size);
	return (bites);
}
