/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmicolon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 18:18:39 by rmicolon          #+#    #+#             */
/*   Updated: 2016/07/09 02:55:48 by rmicolon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	cw_perror(char *str, t_cwar *cwar)
{
	t_player *tmp;
	
	if (cwar)
	{
		if (cwar->opt)
			free(cwar->opt);
		if (cwar->players)
		{
			while (cwar->players && cwar->players->next)
			{
				tmp = cwar->players;
				while (tmp->next->next)
					tmp = tmp->next;
				if (tmp->next->pg)
					free(tmp->next->pg);
				free(tmp->next);
				tmp->next = NULL;
			}
			free(cwar->players);
		}
		free(cwar);
	}
	perror(str);
	exit (1);
}

void	cw_error(char *str, t_cwar *cwar)
{
	t_player *tmp;
	
	if (cwar)
	{
		if (cwar->opt)
			free(cwar->opt);
		if (cwar->players)
		{
			while (cwar->players && cwar->players->next)
			{
				tmp = cwar->players;
				while (tmp->next->next)
					tmp = tmp->next;
				if (tmp->next->pg)
					free(tmp->next->pg);
				free(tmp->next);
				tmp->next = NULL;
			}
			free(cwar->players);
		}
		free(cwar);
	}
	ft_printf("%s", str);
	exit (1);
}

t_cwar	*cw_init(void)
{
	t_cwar	*cwar;

	if (!(cwar = (t_cwar *)malloc(sizeof(t_cwar))))
		cw_perror("Malloc failed.", cwar);
	cwar->players_nbr = 0;
	cwar->opt = NULL;
	cwar->players = NULL;
	cwar->arena = NULL;
	return (cwar);
}

t_opt	*cw_opt_init(t_cwar *cwar)
{
	t_opt	*opt;

	if (!(opt = (t_opt *)malloc(sizeof(t_opt))))
		cw_perror("Malloc failed.", cwar);
	opt->ncurses = 0;
	return (opt);
}

void	cw_usage(t_cwar *cwar)
{
	t_player *tmp;

	if (cwar)
	{
		if (cwar->opt)
			free(cwar->opt);
		if (cwar->players)
		{
			while (cwar->players && cwar->players->next)
			{
				tmp = cwar->players;
				while (tmp->next->next)
					tmp = tmp->next;
				if (tmp->next->pg)
					free(tmp->next->pg);
				free(tmp->next);
				tmp->next = NULL;
			}
			free(cwar->players);
		}
		free(cwar);
	}
	ft_printf("Corewar usage\n");
	exit (1);
}

unsigned int	cw_betole(unsigned int a)
{
	unsigned int	b;
	unsigned char	*src;
	unsigned char	*dst;

	src = (unsigned char *)&a;
	dst = (unsigned char *)&b;
	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
	return (b);
}

void	cw_add_player(t_player *new, t_cwar *cwar)
{
	t_player *tmp;

	if (cwar->players)
	{
		if (cwar->players->next)
		{
			tmp = cwar->players;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			cwar->players->next = new;
	}
	else
		cwar->players = new;
	cwar->players_nbr++;
}

char	cw_new_player(header_t header, int fd, t_cwar *cwar)
{
	int			ret;
	t_player	*new;

	if (!(new = (t_player *)malloc(sizeof(t_player))))
		cw_perror("Malloc failed.", cwar);
	new->header = header;
	if (!(new->pg = (unsigned char *)malloc(header.prog_size + 1)))
		cw_perror("Malloc failed.", cwar);
	if ((ret = read(fd, new->pg, header.prog_size)) < 0)
		cw_perror(header.prog_name, cwar);
	if (ret != (int)header.prog_size)
		return (0);
	new->next = NULL;
	if (cwar->players_nbr < MAX_PLAYERS)
		cw_add_player(new, cwar);
	else
		cw_error("Too much players (This error should be checked)", cwar);
	return (1);
}

void	cw_check_arg(char *arg, t_cwar *cwar)
{
	int				fd;
	int				ret;
	header_t		header;

	if ((fd = open(arg, O_RDONLY)) < 0)
		cw_perror(arg, cwar);
	if ((ret = read(fd, &header, sizeof(header_t))) < 0)
		cw_perror(arg, cwar);
	if (ret != sizeof(header_t))
		cw_error("File (insert filename) is too small to be a champion", cwar);	
	header.magic = cw_betole(header.magic);
	header.prog_size = cw_betole(header.prog_size);
	if (header.prog_size > CHAMP_MAX_SIZE)
		cw_error("(insert filename) is too big (this error msg has to be checked !!)", cwar);
	if (header.magic != COREWAR_EXEC_MAGIC)
		cw_error("(insert filename) has wrong header (this error msg has to be checked !!)", cwar);
	if (!cw_new_player(header, fd, cwar))
		cw_error("(insert filename) is too small", cwar);
	close(fd);
}

void	cw_process_args(int argc, char **argv, t_cwar *cwar)
{
	int i;

	i = 1;
	cwar->opt = cw_opt_init(cwar);
	while (i < argc)
	{
		if (!ft_strcmp(argv[i], "-n"))
			cwar->opt->ncurses = 1;
		else
			cw_check_arg(argv[i], cwar);
		++i;
	}
}

void	cw_setup_arena(t_cwar *cwar)
{
	t_player		*tmp;
	int				i;
	unsigned int	j;
	int				begin;

	if (!(cwar->arena = (unsigned char *)malloc(MEM_SIZE + 1)))
		cw_perror("Malloc failed.", cwar);
	ft_bzero(cwar->arena, MEM_SIZE + 1);
	if (cwar->players)
	{
		i = 0;
		tmp = cwar->players;
		while (tmp)
		{
			begin = (MEM_SIZE * i) / cwar->players_nbr;
			j = 0;
			while (j < tmp->header.prog_size)
			{
				cwar->arena[begin + j] = tmp->pg[j];
				j++;
			}
			++i;
			tmp = tmp->next;
		}
	}
}

void	cw_introduce(t_cwar *cwar)
{
	t_player	*tmp;
	int			i;

	i = 1;
	ft_printf("Introducing contestants...\n");
	if (cwar->players)
	{
		tmp = cwar->players;
		while (tmp)
		{
			ft_printf("* Player %d, weighting %d bytes, \"%s\" (\"%s\") !\n", i, tmp->header.prog_size, tmp->header.prog_name, tmp->header.comment);
			++i;
			tmp = tmp->next;
		}
	}
}

char	*cw_color(int n)
{
	if (n % 6 == 0)
		return (ANSI_COLOR_GREEN);
	else if (n % 6 == 1)
		return (ANSI_COLOR_BLUE);
	else if (n % 6 == 2)
		return (ANSI_COLOR_RED);
	else if (n % 6 == 3)
		return (ANSI_COLOR_YELLOW);
	else if (n % 6 == 4)
		return (ANSI_COLOR_MAGENTA);
	else
		return (ANSI_COLOR_CYAN);
}

void	cw_ncurses(t_cwar *cwar)
{
//	int	i;
	curse_disp(cwar);
//	i = 0;
	//e->x = 5;
	//e->arena = cwar->arena;

	/*if (cwar->arena)
		while (i < MEM_SIZE)
		{
			ft_printf("%.2x", cwar->arena[i]);
			if (!i || (i + 1) % 64)
				ft_putchar(' ');
			else
				ft_putchar('\n');
			++i;
		}*/
}

int		main(int argc, char **argv)
{
	t_cwar	*cwar;

	cwar = cw_init();
	if (argc == 1)
		cw_usage(cwar);
	cw_process_args(argc, argv, cwar);
	cw_setup_arena(cwar);
	if (!cwar->opt->ncurses)
		cw_introduce(cwar);
	else
		cw_ncurses(cwar);
	return (0);
}