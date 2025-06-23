// #include "philo.h"


// void think_sleep(int ms) 
// {
//     usleep(ms * 1000);
// }

// void eat(int ms)
// {
//     usleep(ms * 1000);
// }
// void routine(void *arg)
// {
//     philo_t *p = arg;
//     token_t *r = p->rules;
//     int left = p->id;
//     int right = (p->id + 1) % r->number;
//     if (p->id % 2 == 0)
//     {
//         pthread_mutex_lock(right);
//         pthread_mutex_lock(left);
//     }
//     else
//     {
//         pthread_mutex_lock(left);
//         pthread_mutex_lock(right);
//     }

// // 
// }
// int parsing_check(char **av)
// {
//     int i =1;
//     while(av[i])
//     {
//         int z =0;
//         while(av[i][z])
//         {
//             if(av[i][z] > '9' || av[i][z] < 0)
//                 return 1;
//             z++;
//         }
//         i++;
//     }
//     return 0;
// }

// void token_numbers(philo_t *philo)
// {
//     philo->token.number_of_philosophers = philo->tab[0];
//     philo->token.time_to_die = philo->tab[1];
//     philo->token.time_to_eat = philo->tab[2];
//     philo->token.time_to_sleep = philo->tab[3];
//     // if(philo->tab[4])
//     //     philo->token.times_philo_must_eat = philo->tab[4];

// }

// int *store_numbers(char **av, int ac,philo_t *philo)
// {
//     int i = 1;
//     philo->tab = malloc(sizeof(int) * (ac));
//     while(av[i])
//         {
//             philo->tab[i-1] = atoi(av[i]);
//             i++;
//         }
//     philo->tab[i] = NULL;
//     token_numbers(philo);
//     return philo->tab;
// }

// int main(int ac, char **av)
// {
    
//     if(ac == 5 || ac == 6)
//     {
//         int z = parsing_check(av);
//         if(z == 0)
//             {
//                 philo_t *philo = malloc(sizeof(philo_t));
//                 store_numbers(av,ac,philo);
//                 printf("%d",philo->token.number_of_philosophers);
//             }
//         else 
//             {
//                 printf("must be only numbers");
//                 return 1;
//             }
//         }
//     else 
//         {
//             printf("must be only 4 or 5 number");
//             return 1;
//         }
//     return 0;
// }