/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:22:00 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/06/27 20:28:34 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_rules *rules)
{
	int			i;
	pthread_t	monitor_thread;

	monitor_thread = 0;
	i = 0;
	while (i < rules->token.number_of_philosophers)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	i = 0;
	while (i < rules->token.number_of_philosophers)
	{
		pthread_mutex_destroy(&rules->philos[i].meal_mutex);
		pthread_mutex_destroy(rules->philos[i].left_fork);
		pthread_mutex_destroy(rules->philos[i].right_fork);
		i++;
	}
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->stop_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	pthread_join(monitor_thread, NULL);
}

int	one_philo(t_rules *rules)
{
	print_status(&rules->philos[0], "has taken a fork");
	smart_sleep(&rules->philos[0], rules->token.time_to_die);
	pthread_mutex_lock(&rules->print_mutex);
	printf("%lld 1 died\n", timestamp() - rules->start_time);
	pthread_mutex_unlock(&rules->print_mutex);
	destroy_mutexes(rules);
	free(rules->forks);
	free(rules->philos);
    return 0;
}
