/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afanidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:47:33 by afanidi           #+#    #+#             */
/*   Updated: 2023/11/17 00:14:51 by afanidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*line_after_new_line(char *res, int len)
{
	char	*p;
	int		i;

	i = 0;
	p = malloc(sizeof(char) * (ft_strlen(res) - len));
	if (!p)
		return (NULL);
	++len;
	while (res[len])
	{
		p[i] = res[len];
		i++;
		len++;
	}
	p[i] = '\0';
	free(res);
	if (ft_strlen(p) == 0)
	{
		free(p);
		p = NULL;
	}
	return (p);
}

char	*line_before(char **res, int len)
{
	char	*p;
	int		i;

	i = 0;
	p = malloc(sizeof(char) * (len + 2));
	if (!p || !*res)
		return (NULL);
	while (i < len && (*res)[i])
	{
		p[i] = (*res)[i];
		i++;
	}
	p[i++] = '\n';
	p[i] = '\0';
	*res = line_after_new_line(*res, len);
	return (p);
}

char	*get_last_line(char **reserve)
{
	char	*last_line;

	last_line = NULL;
	if (*reserve && (*reserve)[0] != '\0')
	{
		last_line = ft_strdup(*reserve);
		free(*reserve);
		*reserve = NULL;
	}
	return (last_line);
}

char	*help_fun(int fd, ssize_t *bytes, char **reserve)
{
	char	*buff;

	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (0);
	*bytes = read(fd, buff, BUFFER_SIZE);
	if (*bytes == -1)
	{
		free(buff);
		buff = NULL;
		if (*reserve)
		{
			free(*reserve);
			*reserve = NULL;
		}
		return (0);
	}
	return (buff);
}

char	*get_next_line(int fd)
{
	static char	*reserve;
	char		*buff;
	ssize_t		res;

	res = 1;
	while (res > 0)
	{
		buff = help_fun(fd, &res, &reserve);
		if (!buff)
			return (NULL);
		if (res == 0)
		{
			free(buff);
			if (reserve && check_new_line(reserve) > -1)
				return (line_before(&reserve, check_new_line(reserve)));
			break ;
		}
		buff[res] = '\0';
		reserve = ft_strjoin(reserve, buff);
		free(buff);
		if (check_new_line(reserve) > -1)
			return (line_before(&reserve, check_new_line(reserve)));
	}
	return (get_last_line(&reserve));
}
/*
int main()
{
	int fd = open("file.txt",O_RDWR);
	char *line;
	int i = 0;
	while ((line = get_next_line(fd))!= NULL)
	{
		i++;
		printf("line %d :%s",i,line);
        	free(line);
	}
	printf("->%p",line);
	close(fd);
	return (0);
}*/
