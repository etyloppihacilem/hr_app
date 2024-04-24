# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmelica <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/07 10:50:51 by hmelica           #+#    #+#              #
#    Updated: 2023/10/28 14:53:49 by hmelica          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Sources
FTMA_DIR	=	ft_math/
FTIS_DIR	=	ft_is/
FTTO_DIR	=	ft_to/
FTMEM_DIR	=	ft_mem/
FTTRIM_DIR	=	ft_trim/
FTSTR_DIR	=	ft_str/
FTPUT_DIR	=	ft_put/
FTLST_DIR	=	ft_lst/
FTGNL_DIR	=	ft_gnl/
FTPRF_DIR	=	ft_printf/

FTMA		=	ft_abs.c

FTIS		=	ft_isalnum.c \
				ft_isalpha.c \
				ft_isascii.c \
				ft_isdigit.c \
				ft_islower.c \
				ft_isplusminus.c \
				ft_isprint.c \
				ft_isquote.c \
				ft_isspace.c \
				ft_isstr_int.c \
				ft_isupper.c

FTTO		=	ft_atoi.c \
				ft_atoi_base.c \
				ft_atol.c \
				ft_itoa.c \
				ft_tolower.c \
				ft_toupper.c

FTMEM		=	ft_bzero.c \
				ft_calloc.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_memcpy.c \
				ft_memmove.c \
				ft_memset.c \
				ft_strrealloc.c \

FTTRIM		=	ft_trimleft.c \
				ft_trimright.c

FTSTR		=	ft_arrclear.c \
				ft_offset.c \
				ft_split.c \
				ft_strcasecmp.c \
				ft_strchr.c \
				ft_strdup.c \
				ft_striteri.c \
				ft_strjoin.c \
				ft_strjoin2.c \
				ft_strnjoin.c \
				ft_strlcat.c \
				ft_strlcpy.c \
				ft_strlen.c \
				ft_strmapi.c \
				ft_strncmp.c \
				ft_strnstr.c \
				ft_strrchr.c \
				ft_strtrim.c \
				ft_substr.c \
				ft_wordlen.c

FTPUT		=	ft_putchar_fd.c \
				ft_putendl_fd.c \
				ft_putnbr_fd.c \
				ft_putstr_fd.c

FTLST		=	ft_lstadd_back.c \
				ft_lstadd_front.c \
				ft_lstclear.c \
				ft_lstdelone.c \
				ft_lstiter.c \
				ft_lstlast.c \
				ft_lstmap.c \
				ft_lstnew.c \
				ft_lstsize.c

FTGNL		=	get_next_line.c \
				get_next_line_utils.c

FTPRF		=	char.c \
				digit.c \
				flags.c \
				ft_dprintf.c \
				ft_printf.c \
				hexa.c \
				is_something.c \
				min_width.c \
				pointer.c \
				string.c \
				unsigned.c

SRCS_DIR = src/
SRCS_FILES	+= $(addprefix $(FTMA_DIR),$(FTMA))
SRCS_FILES	+= $(addprefix $(FTIS_DIR),$(FTIS))
SRCS_FILES	+= $(addprefix $(FTTO_DIR),$(FTTO))
SRCS_FILES	+= $(addprefix $(FTMEM_DIR),$(FTMEM))
SRCS_FILES	+= $(addprefix $(FTTRIM_DIR),$(FTTRIM))
SRCS_FILES	+= $(addprefix $(FTSTR_DIR),$(FTSTR))
SRCS_FILES	+= $(addprefix $(FTPUT_DIR),$(FTPUT))
SRCS_FILES	+= $(addprefix $(FTLST_DIR),$(FTLST))
SRCS_FILES	+= $(addprefix $(FTGNL_DIR),$(FTGNL))
SRCS_FILES	+= $(addprefix $(FTPRF_DIR),$(FTPRF))
SRCS = $(addprefix src/, $(SRCS_FILES))

OBJS_DIR 	= obj/
DIR_TODO	+= $(FTMA_DIR)
DIR_TODO	+= $(FTIS_DIR)
DIR_TODO	+= $(FTTO_DIR)
DIR_TODO	+= $(FTMEM_DIR)
DIR_TODO	+= $(FTTRIM_DIR)
DIR_TODO	+= $(FTSTR_DIR)
DIR_TODO	+= $(FTPUT_DIR)
DIR_TODO	+= $(FTLST_DIR)
DIR_TODO	+= $(FTGNL_DIR)
DIR_TODO	+= $(FTPRF_DIR)
DIR_TODO	:= $(addprefix $(OBJS_DIR), $(DIR_TODO))
OBJS		= ${patsubst ${SRCS_DIR}%.c,${OBJS_DIR}%.o,$(SRCS)}

NAME		= libft.a
HEADERS		= libft.h
INCLUDES	= -I .

LIBC		= ar
LIBFLAGS	= rcs
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f
MKDIR		= mkdir

OK_PROMPT		= \033[1;32mdone\033[0m
CFLAGS			:= ${CFLAGS} $(if $(filter debug,$(MAKECMDGOALS)),-g,)
DEBUG_PROMPT	= \033[1;35mdebug mode\033[0m
OK_PROMPT		= \033[1;32mdone \033[0m$(if $(filter debug,$(MAKECMDGOALS)),$(DEBUG_PROMPT),)
DELETE			= \033[2K\r

all: ${NAME}
	@echo > /dev/null

${NAME}: ${OBJS_DIR} ${OBJS}
	@printf "$(DELETE)\033[1;33m...Building\033[0m %-33s" "libft"
	@${LIBC} ${LIBFLAGS} ${NAME} ${OBJS} ${HEADERS}
	@echo "${OK_PROMPT}"

${OBJS_DIR}:
	@printf "\033[1;34m...Creating\033[0m %-33s" "${OBJS_DIR} directory"
	@${MKDIR} ${OBJS_DIR} ${DIR_TODO}
	@printf "\033[1;32mdone\033[0m"

${OBJS_DIR}%.o: ${SRCS_DIR}%.c
	@printf "$(DELETE)\033[1;34mCompiling\033[0m %-35s" $<
	@${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
	@printf "${OK_PROMPT}"

clean:
	@${RM} ${OBJS} ${BONUS_OBJS}
	@ rm -df ${DIR_TODO} ${OBJS_DIR}

fclean: clean
	@${RM} ${NAME}

re: fclean all

debug: all
	@exit;
