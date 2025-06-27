/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:09:30 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/06/27 20:37:13 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!philo->rules->stop)
		printf("%lld %d %s\n", timestamp() - philo->rules->start_time, philo->id
			+ 1, msg);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

int	death_check(t_rules *rules, int time_since_meal, t_philo *p)
{
	if (time_since_meal > rules->token.time_to_die)
	{
		rules->stop = 1;
		printf("%lld %d died\n", (timestamp() - rules->start_time), p->id + 1);
		return (1);
	}
	else if (rules->token.times_philo_must_eat != -1
		&& p->meals_eaten >= rules->token.times_philo_must_eat)
	{
		pthread_mutex_lock(&rules->stop_mutex);
		rules->stop_eating = 1;
		pthread_mutex_unlock(&rules->stop_mutex);
		printf("%lld %d has eaten enough meals\n", (timestamp()
				- rules->start_time), p->id + 1);
		return (1);
	}
	else
		return (0);
}

void	*monitor(void *arg)
{
	t_rules			*rules;
	int				i;
	t_philo			*p;
	unsigned int	time_since_meal;
	int				z;

	rules = (t_rules *)arg;
	while (!rules->stop)
	{
		i = 0;
		while (i < rules->token.number_of_philosophers)
		{
			p = &rules->philos[i];
			pthread_mutex_lock(&p->meal_mutex);
			time_since_meal = timestamp() - p->last_meal;
			pthread_mutex_unlock(&p->meal_mutex);
			z = death_check(rules, time_since_meal, p);
			if (z == 1)
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*p;

	int (stopped), (stop_eating);
	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&p->rules->stop_mutex);
		stopped = p->rules->stop;
		stop_eating = p->rules->stop_eating;
		pthread_mutex_unlock(&p->rules->stop_mutex);
		if (stopped || p->rules->token.number_of_philosophers == 1
			|| (stop_eating == 1
				&& p->meals_eaten >= p->rules->token.times_philo_must_eat))
			break ;
		eat(p);
		smart_sleep(p, p->rules->token.time_to_eat);
		sleep_and_think(p);
	}
	if (p->has_right)
		pthread_mutex_unlock(p->right_fork);
	if (p->has_left)
		pthread_mutex_unlock(p->left_fork);
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
	parse_input(&rules, ac, av);
	if (parse_input(&rules, ac, av) == 1)
		return (1);
	n = rules.token.number_of_philosophers;
	philo_init(&rules, n);
	rules.stop = 0;
	rules.start_time = timestamp();
	if (n == 1 && one_philo(&rules) == 0)
		return (0);
	monitor(&rules);
	pthread_joan(n, &rules);
	destroy_mutexes(&rules);
	free(rules.forks);
	free(rules.philos);
	return (0);
}
