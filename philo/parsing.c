/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:09:22 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/14 22:43:49 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
		return (printf("Error: Please use positive value\n"), INT_MAX);
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + str[i++] - 48;
		if (num > INT_MAX)
			return (printf("Error: value is too big\n"), INT_MAX);
	}
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i])
		return (printf("Error: not valid integer\n"), INT_MAX);
	return (num);
}

int	parsing_check(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	parse_input(t_rules *rules, int ac, char **av)
{
	rules->token.number_of_philosophers = ft_atoi(av[1]);
	rules->token.time_to_die = ft_atoi(av[2]);
	rules->token.time_to_eat = ft_atoi(av[3]);
	rules->token.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		rules->token.times_philo_must_eat = ft_atoi(av[5]);
	else
		rules->token.times_philo_must_eat = -1;
	if (rules->token.times_philo_must_eat == 0)
		return (1);
	if (rules->token.time_to_die == INT_MAX
		|| rules->token.number_of_philosophers == INT_MAX
		|| rules->token.time_to_eat == INT_MAX
		|| rules->token.time_to_sleep == INT_MAX
		|| rules->token.number_of_philosophers == 0
		|| rules->token.time_to_eat <= 0
		|| rules->token.time_to_sleep <= 0)
		return (1);
	return (0);
}

int	philo_init(t_rules *rules, int n)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * n);
	rules->philos = malloc(sizeof(t_philo) * n);
	i = 0;
	while (i < n)
		pthread_mutex_init(&rules->forks[i++], NULL);
	init_mutexes(rules);
	i = 0;
	while (i < rules->token.number_of_philosophers)
	{
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % n];
		rules->philos[i].id = i;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].finished = 0;
		pthread_mutex_init(&rules->philos[i].meal_mutex, NULL);
		rules->philos[i].rules = rules;
		i++;
	}
	rules->start_time = timestamp();
	creat_threads(rules);
	return (0);
}

void	creat_threads(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->token.number_of_philosophers)
	{
		rules->philos[i].last_meal = rules->start_time;
		pthread_create(&rules->philos[i].thread, NULL, routine,
			&rules->philos[i]);
		i++;
	}
}
