/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:09:30 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/10 05:35:44 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->stop_mutex);
	if (!philo->rules->stop)
	{
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%lld %d %s\n", (timestamp() - philo->rules->start_time),
			philo->id + 1, msg);
		pthread_mutex_unlock(&philo->rules->print_mutex);
	}
	pthread_mutex_unlock(&philo->rules->stop_mutex);
}

int	death_check(t_rules *rules, int time_since_meal, t_philo *p)
{
	int	eaten;

	pthread_mutex_lock(&p->meal_mutex);
	eaten = p->meals_eaten;
	pthread_mutex_unlock(&p->meal_mutex);
	if (time_since_meal > rules->token.time_to_die)
	{
		confirmed_death(rules, p);
		return (1);
	}
	else if (rules->token.times_philo_must_eat != -1
		&& eaten >= rules->token.times_philo_must_eat)
	{
		p->finished = 1;
		rules->philosophers_done_eating++;
		if (rules->philosophers_done_eating
			== rules->token.number_of_philosophers)
		{
			pthread_mutex_lock(&rules->stop_mutex);
			rules->stop_eating = 1;
			pthread_mutex_unlock(&rules->stop_mutex);
		}
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_rules			*rules;
	t_philo			*p;
	unsigned int	time_since_meal;
	int				i;

	rules = (t_rules *)arg;
	while (1)
	{
		if (are_u_dead(rules) == 1)
			return (NULL);
		i = 0;
		while (i < rules->token.number_of_philosophers)
		{
			p = &rules->philos[i];
			pthread_mutex_lock(&p->meal_mutex);
			time_since_meal = timestamp() - p->last_meal;
			pthread_mutex_unlock(&p->meal_mutex);
			if (death_check(rules, time_since_meal, p) == 1)
				return (NULL);
			i++;
		}
		usleep(50);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*p;
	int		stopped;
	int		stop_eating;

	p = (t_philo *)arg;
	while (1)
	{
		if (p->id % 2 != 0)
			usleep(100);
		pthread_mutex_lock(&p->rules->stop_mutex);
		stopped = p->rules->stop;
		stop_eating = p->rules->stop_eating;
		pthread_mutex_unlock(&p->rules->stop_mutex);
		if (stopped || p->rules->token.number_of_philosophers == 1
			|| (stop_eating == 1
				&& p->meals_eaten >= p->rules->token.times_philo_must_eat))
			break ;
		eat(p);
		sleep_and_think(p);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_rules	rules;
	int		i;
	int		n;

	i = 0;
	if (ac != 5 && ac != 6)
		return (printf("Usage: ./philo nbr die eat sleep [must_eat]\n"), 1);
	if (parsing_check(av))
		return (printf("Error: Arguments must be numeric.\n"), 1);
	if (parse_input(&rules, ac, av) == 1)
		return (1);
	n = rules.token.number_of_philosophers;
	rules.start_time = timestamp();
	rules.stop = 0;
	rules.stop_eating = 0;
	rules.philosophers_done_eating = 0;
	philo_init(&rules, n);
	if (n == 1 && one_philo(&rules) == 0)
		return (0);
	pthread_create(&rules.monitor_thread, NULL, monitor, &rules);
	pthread_join_all(n, &rules);
	destroy_mutexes(&rules);
	free(rules.forks);
	free(rules.philos);
	return (0);
}
