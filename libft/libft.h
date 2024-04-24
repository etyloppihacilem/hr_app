/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:11:30 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/03 15:48:42 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_insert
{
	char	type;
	int		flags;
	int		min_width;
	int		prec;
	int		null_prec;
}	t_insert;

/* ft_is */
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_islower(int c);
int			ft_isplusminus(int c);
int			ft_isprint(int c);
int			ft_isquote(int c);
int			ft_isspace(int c);
int			ft_isstr_int(char *str);
int			ft_isupper(int c);

/* ft_to */
int			ft_tolower(int c);
int			ft_toupper(int c);
int			ft_atoi(const char *str);
int			ft_atoi_base(char *str, char *base);
long		ft_atol(const char *str);
char		*ft_itoa(int n);

/* ft_mem */
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_bzero(void *s, size_t n);
char		*ft_strrealloc(char *src, int new_len);

/* ft_trim */
char		*ft_trimleft(const char *str);
char		*ft_trimright(const char *str);

/* ft_str */
char		**ft_split(const char *s, char c);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s);
char		*ft_strjoin(const char *s1, const char *s2);
char		*ft_strjoin(const char *s1, const char *s2);
char		*ft_strjoin2(char *s1, char *s2, int free_s1, int free_s2);
char		*ft_strmapi(const char *s, char (*f)(unsigned int, char));
char		*ft_strnjoin(int size, char **strs, char *sep);
char		*ft_strnstr(const char *s, const char *to_find, size_t size);
char		*ft_strrchr(const char *s, int c);
char		*ft_strtrim(const char *s1, const char *set);
char		*ft_substr(const char *s, unsigned int start, size_t len);
int			ft_strcasecmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlen(const char *s);
size_t		ft_wordlen(const char *str);
void		ft_arrclear(char **array);
void		ft_offset(char *s, size_t off);
void		ft_striteri(char *s, void (*f)(unsigned int, char *));

/* ft_put */
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

/* ft_lst */
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *element);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *element);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* ft_printf and stuff */

int			ft_dprintf(int fd, const char *format, ...);
int			ft_printf(const char *format, ...);

int			check_flag(int nb, char c);
int			get_min_width(const char **format,
				int *flags, va_list act, va_list ori);
int			is_csp(char c);
int			is_digit(char c);
int			is_flag(char c);
int			set_flag(char c);
void		compile_flags(t_insert *ins);
int			precision(const char **format, va_list act, va_list ori);
int			read_following_int(const char **format);

int			display(char *s, t_insert ins, int force_len, int fd);
int			hex_len(unsigned long i);
int			int_len(unsigned int i);
int			dec_rec(unsigned int i, int fd);

int			main_c(va_list act, t_insert ins, int fd);
int			main_d(va_list act, t_insert ins, int fd);
int			main_p(va_list act, t_insert ins, int fd);
int			main_s(va_list act, t_insert ins, int fd);
int			main_u(va_list act, t_insert ins, int fd);
int			main_x(va_list act, t_insert ins, char *tab, int fd);

/* get_next_line */

typedef struct s_lstash
{
	char			buffer[BUFFER_SIZE + 1];
	size_t			buffersize;
	size_t			cumulsize;
	struct s_lstash	*next;
}	t_lstash;

// get_next_line_bonus.c
char		*get_next_line(int fd);
int			fill_lstash(int fd, t_lstash **p_lstash);
char		*build_nl(t_lstash **p_lstash);
void		copy_lstash(char *newline, t_lstash **p_lstash, size_t crlen);
t_lstash	*clean_lstash(t_lstash **p_lstash, size_t crlen);

// get_next_line_utils_bonus.c
size_t		nl_len(const char *str);
void		clear_lstash(t_lstash **stash);
void		add_lstash(t_lstash **lstash, t_lstash *newstash, size_t readsize);

#endif
