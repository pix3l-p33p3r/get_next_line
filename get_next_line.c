/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-yak <elel-yak@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:17:17 by elel-yak          #+#    #+#             */
/*   Updated: 2022/12/12 15:39:32 by elel-yak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	shift_left(char *buffer, int nb)
{
	int	i;

	i = 0;
	if (nb == 0)
		return ;
	while ((i + nb) < BUFFER_SIZE)
	{
		buffer[i] = buffer[i + nb];
		i++;
	}
	buffer[i] = 0;
}

void	change_buffer(char *buffer)
{
	int	count;

	count = 0;
	while (count < BUFFER_SIZE && buffer[count] && buffer[count] != '\n')
		count++;
	if (count == BUFFER_SIZE || buffer[count] == 0)
	{
		while (count--)
			buffer[count] = 0;
	}
	else if (buffer[count] == '\n')
		shift_left(buffer, count);
}

char	*line_join(char *old_line, char *buffer)
{
	int		count;
	int		i;
	char	*new_line;

	count = 0;
	while (count < BUFFER_SIZE && buffer[count] && buffer[count] != '\n')
		count++;
	if (count < BUFFER_SIZE && buffer[count] == '\n')
		count++;
	new_line = malloc((ft_strlen(old_line) + count + 1) * sizeof(char));
	if (!new_line)
		return (ft_free(old_line));
	i = -1;
	while (old_line && old_line[++i])
		new_line[i] = old_line[i];
	if (!old_line)
		i++;
	new_line[i + count] = 0;
	while (count--)
		new_line[i + count] = buffer[count];
	change_buffer(buffer);
	ft_free (old_line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	char		*line;
	int			nb_read;

	line = NULL;
	if (fd >= 0 && buffer[0])
		line = line_join(line, buffer);
	while (fd >= 0)
	{
		if (buffer[0] == '\n')
		{
			shift_left(buffer, 1);
			break ;
		}
		nb_read = read(fd, buffer, BUFFER_SIZE);
		if (nb_read == -1)
			return (ft_free(line));
		if (nb_read == 0)
			return (line);
		line = line_join(line, buffer);
		if (!line)
			return (NULL);
	}
	return (line);
}

int	main()
{
	char *line;
	int fd = open("./file", O_RDONLY);
	while ((line = get_next_line(fd)))
		printf("%s", line);
	return (0);
}
