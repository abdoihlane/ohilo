/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:09:34 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/06/27 19:10:00 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct t_token
{
	long			number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			times_philo_must_eat;
}					t_token;

typedef struct t_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	struct t_rules	*rules;
	int				has_left;
	int				has_right;
}					t_philo;

typedef struct t_rules
{
	t_token			token;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	int				stop;
	pthread_mutex_t	stop_mutex;
	int				stop_eating;
	long long		start_time;
	t_philo			*philos;
}					t_rules;

long				ft_atoi(const char *str);
unsigned int		timestamp(void);
void				smart_sleep(t_philo *p, int time_ms);
void				print_status(t_philo *philo, char *msg);
int					parse_input(t_rules *rules, int ac, char **av);
int					parsing_check(char **av);
int					philo_init(t_rules *rules, int n);
void				*routine(void *arg);
void				*monitor(void *arg);
void				destroy_mutexes(t_rules *rules);
void				eat(t_philo *p);
void				sleep_and_think(t_philo *p);
void				pthread_joan(int n, t_rules *rules);
#endif
