/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myrmarti <myrmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 16:57:33 by myrmarti          #+#    #+#             */
/*   Updated: 2022/03/03 14:42:07 by myrmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_sleep(t_philo *philo)
{
	philo->color = "\033[0;32m";
	print_thread(&philo->fork->mutex, "is sleeping", philo);
	slp(philo->time_to_sleep * 1e3);
}

void	ft_think(t_philo *philo)
{
	philo->color = "\033[0;37m";
	print_thread(&philo->fork->mutex, "is thinking", philo);
}

void	eat(t_philo *philo)
{
	int		fork_right;

	fork_right = order_fork(philo);
	/* if (philo->id % 2 == 0) */
	/* 	will_eat(philo, philo->id - 1, fork_right); */
	/* else */
	/* 	will_eat(philo, fork_right, philo->id -1); */
}

void	update_signal(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo->sig->even_max_eat[0] -= 1;
		if (!philo->sig->even_max_eat[0])
		{
			philo->sig->even_max_eat[0] = philo->sig->even_max_eat[1];
			philo->sig->sig_eat = 0;
		}
	}
	else
	{
		philo->sig->odd_max_eat[0] -= 1;
		printf("%d\n", philo->sig->odd_max_eat[0]);
		if (!philo->sig->odd_max_eat[0])
		{
			philo->sig->odd_max_eat[0] = philo->sig->odd_max_eat[1];
			philo->sig->sig_eat = 2;
		}
	}
}



void __eat(t_philo *philo)
{
	int	fork_right;

	fork_right = order_fork(philo);
	lock(philo, fork_right,  philo->id - 1);
	take_fork(philo);
	//eat
	long time_past;

	time_past = ft_time() - philo->t_beg_lp;
	printf("id: %zu, time: %ld\n", philo->id, time_past);
	printf("sig|%d|%d\n", philo->sig->sig_eat, philo->sig->odd_max_eat[0]);
	if (time_past < philo->time_bf_eat)
	{
		print_thread(&philo->fork->mutex, "is eating", philo);
		philo->t_beg_lp = ft_time();
		slp(philo->time_for_eat);
	}
	else
		exit(0);
	unlock(philo, fork_right, philo->id - 1);
	update_signal(philo);
	printf("sig|%d|%d\n", philo->sig->sig_eat, philo->sig->odd_max_eat[0]);
	//sleep
	print_thread(&philo->fork->mutex, "is sleeping", philo);
	slp(philo->time_to_sleep);
	//think
	/*print_thread(&philo->fork->mutex, "is thinking", philo);*/
	return ;
}
void	*event_loop(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	while (1)
	{

		if (
		(philo->id % 2 != 0 && philo->sig->sig_eat != 2)
		||(philo->id % 2 == 0 && philo->sig->sig_eat == 2))
			__eat(philo);
		/* if ((condition_event(eat, philo)) == 0 */
		/* 	|| philo->nbr_time_eat == 0) */
		/* 	break ; */
		/* if (condition_event(ft_sleep, philo) == 0) */
		/* 	break ; */
		/* if (condition_event(ft_think, philo) == 0) */
		/* 	break ; */
	}
	return (NULL);
}
