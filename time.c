/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:09:40 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/06/27 19:10:07 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	smart_sleep(t_philo *p, int time_ms)
{
	long long	start;

	start = timestamp();
	while ((timestamp() - start) < time_ms && p->rules->stop != 1)
		usleep(500);
}

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

void	eat(t_philo *p)
{
	pthread_mutex_lock(p->left_fork);
	p->has_left = 1;
	print_status(p, "has taken a fork");
	pthread_mutex_lock(p->right_fork);
	p->has_right = 1;
	print_status(p, "has taken a fork");
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal = timestamp();
	pthread_mutex_unlock(&p->meal_mutex);
	print_status(p, "is eating");
	p->meals_eaten++;
	pthread_mutex_unlock(p->right_fork);
	p->has_right = 0;
	pthread_mutex_unlock(p->left_fork);
	p->has_left = 0;
}

void	sleep_and_think(t_philo *p)
{
	print_status(p, "is sleeping");
	smart_sleep(p, p->rules->token.time_to_sleep);
	print_status(p, "is thinking");
}
