/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:22:00 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/10 05:41:46 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->token.number_of_philosophers)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		pthread_mutex_destroy(&rules->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->stop_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
}

int	one_philo(t_rules *rules)
{
	t_philo	*p;

	p = rules->philos;
	pthread_mutex_lock(p->left_fork);
	print_status(&rules->philos[0], "has taken a fork");
	precise_sleep(&rules->philos[0], rules->token.time_to_die);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_lock(&rules->print_mutex);
	printf("%lld 1 died\n", timestamp() - rules->start_time);
	pthread_mutex_unlock(&rules->print_mutex);
	pthread_join_all(1, rules);
	pthread_mutex_destroy(&rules->forks[0]);
	pthread_mutex_destroy(&rules->philos[0].meal_mutex);
	pthread_mutex_destroy(&rules->stop_mutex);
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules->philos);
	free(rules->forks);
	return (0);
}

void	init_mutexes(t_rules *rules)
{
	pthread_mutex_init(&rules->stop_mutex, NULL);
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->death_mutex, NULL);
}

int	confirmed_death(t_rules *rules, t_philo *p)
{
	pthread_mutex_lock(&rules->stop_mutex);
	if (!rules->stop)
	{
		rules->stop = 1;
		pthread_mutex_unlock(&rules->stop_mutex);
		pthread_mutex_lock(&rules->print_mutex);
		printf("%lld %d died\n", (timestamp() - rules->start_time), p->id + 1);
		pthread_mutex_unlock(&rules->print_mutex);
	}
	else
		pthread_mutex_unlock(&rules->stop_mutex);
	return (1);
}

int	are_u_dead(t_rules *rules)
{
	pthread_mutex_lock(&rules->stop_mutex);
	if (rules->stop || rules->stop_eating)
	{
		pthread_mutex_unlock(&rules->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&rules->stop_mutex);
	return (0);
}
