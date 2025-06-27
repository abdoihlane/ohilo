/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:09:40 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/06/27 20:22:40 by ahabibi-         ###   ########.fr       */
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
