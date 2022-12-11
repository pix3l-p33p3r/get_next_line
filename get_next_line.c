/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-yak <elel-yak@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:17:17 by elel-yak          #+#    #+#             */
/*   Updated: 2022/12/11 16:57:42 by elel-yak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	shift_left(char *buffer, int nb)
{
	int	i;

	i = 0;
	if (nb == 0)
		return ;
	while (i + nb < BUFFER_SIZE)
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
		buffer[0] = 0;
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
	i = 0;
	while (old_line && old_line[i++])
		new_line[i] = old_line[i];
	new_line[i + count] = 0;
	printf("%d\n", ft_strlen(old_line));
	while (count--)
	{
		printf("%d .--- %d\n",i , count);
		printf("%c \n",buffer[count] );
		new_line[i + count] = buffer[count];	
		printf("%c****** \n",new_line[i + count] );
	
	}
	change_buffer(buffer);
	free (old_line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char *save_buff;
	char		*buff;
	char		*line;
	int 		nb_read;

	line = NULL;
	buff = malloc(BUFFER_SIZE * sizeof(char) + 1);
	if (buff)
	{
		line = line_join(line, buff);
		return (NULL);
	}
	while (fd)
	{
		if (buff[0] == '\n')
		{
			shift_left(buff, 1);
			break ;
		}
		nb_read = read(fd, buff, BUFFER_SIZE);
		if (nb_read == -1)
			return (ft_free(line));
		if (nb_read == 0)
			return (line);
		line = line_join(line, buff);
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