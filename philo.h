#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_token
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int times_philo_must_eat;
}   token_t;

typedef struct s_philo
{
    int id;
    int meals_eaten;
    long long last_meal;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t meal_mutex;
    struct s_rules *rules;
}   philo_t;

typedef struct s_rules
{
    token_t token;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    int stop;
    pthread_mutex_t stop_mutex;
    // int stop;
    long long start_time;
    philo_t *philos;
}   rules_t;

long long timestamp(void);
void smart_sleep(int time_ms);
void print_status(philo_t *philo, char *msg);
int parse_input(rules_t *rules, int ac, char **av);
int parsing_check(char **av);
int philo_init(rules_t *rules);
void *routine(void *arg);
void *monitor(void *arg);

#endif
