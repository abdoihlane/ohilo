/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers+.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 05:41:12 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/10 05:41:21 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pthread_join_all(int n, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
	if (n > 1)
		pthread_join(rules->monitor_thread, NULL);
}
