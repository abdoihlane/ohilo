/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:09:34 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/07/11 22:27:25 by ahabibi-         ###   ########.fr       */
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
	int				finished;
}					t_philo;

typedef struct t_rules
{
	t_token			token;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_t		monitor_thread;
	pthread_mutex_t	stop_mutex;
	int				stop_eating;
	int				stop;
	long long		start_time;
	t_philo			*philos;
	int				philosophers_done_eating;
}					t_rules;

			//parsing and 
int					parsing_check(char **av);
long				ft_atoi(const char *str);
int					parse_input(t_rules *rules, int ac, char **av);
void				precise_sleep(t_philo *p, int time_ms);
unsigned int		timestamp(void);
void				print_status(t_philo *philo, char *msg);
			// routine stuff
void				*routine(void *arg);
void				eat_continue(t_philo *p);
void				eat(t_philo *p);
void				sleep_and_think(t_philo *p);
int					one_philo(t_rules *rules);
			//monitor and death checks
void				*monitor(void *arg);
int					death_check(t_rules *rules, int time_since_meal,
						t_philo *p);
int					are_u_dead(t_rules *rules);
int					confirmed_death(t_rules *rules, t_philo *p);
			// init and clean up
void				init_mutexes(t_rules *rules);
int					philo_init(t_rules *rules, int n);
void				destroy_mutexes(t_rules *rules);
void				creat_threads(t_rules *rules);
void				pthread_join_all(int n, t_rules *rules);

#endif