#include "philo.h"

void print_status(philo_t *philo, char *msg)
{
    pthread_mutex_lock(&philo->rules->print_mutex);
    if (!philo->rules->stop)
        printf("%lld %d %s\n", timestamp() - philo->rules->start_time, philo->id + 1, msg);
    pthread_mutex_unlock(&philo->rules->print_mutex);
}

void *monitor(void *arg)
{
    rules_t *rules = (rules_t *)arg;
    while (!rules->stop)
    {
        int i = 0;
        while (i < rules->token.number_of_philosophers)
        {
            philo_t *p = &rules->philos[i];

            pthread_mutex_lock(&p->meal_mutex);
            long long time_since_meal = timestamp() - p->last_meal;
            pthread_mutex_unlock(&p->meal_mutex);

            if (time_since_meal > rules->token.time_to_die)
            {
                rules->stop = 1;
                printf("%lld %d died\n", (timestamp() - rules->start_time), p->id + 1);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
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


void *routine(void *arg)
{
    philo_t *p = (philo_t *)arg;
    int has_left = 0;
    int has_right = 0;

    if (p->id % 2 == 0)
        usleep(1000);

    while (1)
    {
        pthread_mutex_lock(&p->rules->stop_mutex);
        int stopped = p->rules->stop;
        pthread_mutex_unlock(&p->rules->stop_mutex);
        if (stopped)
            break;

        pthread_mutex_lock(p->left_fork);
        has_left = 1;
        print_status(p, "has taken a fork");

        pthread_mutex_lock(p->right_fork);
        has_right = 1;
        print_status(p, "has taken a fork");

        pthread_mutex_lock(&p->meal_mutex);
        p->last_meal = timestamp();
        pthread_mutex_unlock(&p->meal_mutex);
        print_status(p, "is eating");

        smart_sleep(p->rules->token.time_to_eat);
        p->meals_eaten++;

        pthread_mutex_unlock(p->right_fork);
        has_right = 0;
        pthread_mutex_unlock(p->left_fork);
        has_left = 0;

        print_status(p, "is sleeping");
        smart_sleep(p->rules->token.time_to_sleep);

        print_status(p, "is thinking");
    }

    if (has_right)
        pthread_mutex_unlock(p->right_fork);
    if (has_left)
        pthread_mutex_unlock(p->left_fork);

    return NULL;
}


// A deadlock in C occurs when two or more threads or processes
// are blocked indefinitely
//, each waiting for a resource that the other holds.




int main(int ac, char **av)
{
    if (ac != 5 && ac != 6)
        return (printf("Usage: ./philo nbr die eat sleep [must_eat]\n"), 1);
    if (parsing_check(av)) // chrckssssssssssss
        return (printf("Error: Arguments must be numeric.\n"), 1);

    int i = 0;
    rules_t rules;
    pthread_t monitor_thread = 0;

    parse_input(&rules, ac, av);  // fill  
    if (parse_input(&rules, ac, av) == 1)
        return 1;
    int n = rules.token.number_of_philosophers;

    rules.forks = malloc(sizeof(pthread_mutex_t) * n);
    rules.philos = malloc(sizeof(philo_t) * n);
    pthread_mutex_init(&rules.stop_mutex, NULL);

    pthread_mutex_init(&rules.print_mutex, NULL);
    rules.stop = 0;
    rules.start_time = timestamp();

    while(i < n)
    {
        pthread_mutex_init(&rules.forks[i], NULL);
        i++;
    }

    philo_init(&rules);
    monitor(&rules);
    
    // pthread_create(&monitor_thread, NULL, monitor, &rules);

    i = 0;
    while(i < n)
    {
        pthread_join(rules.philos[i].thread, NULL);
        i++;
    }
    i = 0;
    while(i < n)
    {
        pthread_mutex_destroy(&rules.forks[i]);
        i++;
    }
    pthread_mutex_destroy(&rules.stop_mutex);

    pthread_mutex_destroy(&rules.death_mutex);
    for (i = 0; i < n; i++)
    {
        pthread_mutex_destroy(&rules.philos[i].meal_mutex);
        pthread_mutex_destroy(rules.philos[i].left_fork);
        pthread_mutex_destroy(rules.philos[i].right_fork);
    }

    pthread_mutex_destroy(&rules.print_mutex);
    pthread_join(monitor_thread, NULL);
    free(rules.forks);
    free(rules.philos);

    return 0;
}
