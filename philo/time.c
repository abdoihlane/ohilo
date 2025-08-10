/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 05:36:19 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/10 05:38:48 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(t_philo *p, int time_ms)
{
	long long	start;
	long long	elapsed;

	start = timestamp();
	while (1)
	{
		elapsed = timestamp() - start;
		if (elapsed >= time_ms)
			break ;
		pthread_mutex_lock(&p->rules->stop_mutex);
		if (p->rules->stop == 1)
		{
			pthread_mutex_unlock(&p->rules->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&p->rules->stop_mutex);
		if (time_ms - elapsed > 5)
			usleep(500);
		else
			usleep(50);
	}
}

void	eat_continue(t_philo *p)
{
	long long	now;

	now = timestamp();
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal = now;
	p->meals_eaten++;
	pthread_mutex_unlock(&p->meal_mutex);
	print_status(p, "is eating");
}

void	eat(t_philo *p)
{
	int				fork1_id;
	int				fork2_id;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;

	fork1_id = p->id;
	fork2_id = (p->id + 1) % p->rules->token.number_of_philosophers;
	if (fork1_id < fork2_id)
	{
		fork1 = p->left_fork;
		fork2 = p->right_fork;
	}
	else
	{
		fork1 = p->right_fork;
		fork2 = p->left_fork;
	}
	pthread_mutex_lock(fork1);
	print_status(p, "has taken a fork");
	pthread_mutex_lock(fork2);
	print_status(p, "has taken a fork");
	eat_continue(p);
	precise_sleep(p, p->rules->token.time_to_eat);
	pthread_mutex_unlock(fork2);
	pthread_mutex_unlock(fork1);
}

void	sleep_and_think(t_philo *p)
{
	print_status(p, "is sleeping");
	precise_sleep(p, p->rules->token.time_to_sleep);
	print_status(p, "is thinking");
	if (p->rules->token.number_of_philosophers % 2 == 1)
	{
		usleep(100 * (p->id + 1));
	}
}
