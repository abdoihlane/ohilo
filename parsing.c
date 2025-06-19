#include "philo.h"

int parsing_check(char **av)
{
    int i = 1;
    while(av[i])
    {
        int j = 0;
        while(av[i][j])
        {
            if (av[i][j] < '0' || av[i][j] > '9')
                return 1;
            j++;
        }
        i++;
    }
    return 0;
}

int parse_input(rules_t *rules, int ac, char **av)
{
    rules->token.number_of_philosophers = atoi(av[1]);
    rules->token.time_to_die = atoi(av[2]);
    rules->token.time_to_eat = atoi(av[3]);
    rules->token.time_to_sleep = atoi(av[4]);
    rules->token.times_philo_must_eat = (ac == 6) ? atoi(av[5]) : -1;
    return 0;
}

int philo_init(rules_t *rules)
{
    int i = 0;
    while( i < rules->token.number_of_philosophers)
    {
        rules->philos[i].id = i;
        rules->philos[i].meals_eaten = 0;
        rules->philos[i].last_meal = timestamp();
        rules->philos[i].left_fork = &rules->forks[i];
        rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->token.number_of_philosophers];
        rules->philos[i].rules = rules;
        pthread_mutex_init(&rules->philos[i].meal_mutex, NULL);
        pthread_create(&rules->philos[i].thread, NULL, routine, &rules->philos[i]);
        i++;
    }
    return 0;
}
