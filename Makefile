# ##############################################################################
#
#              """          Makefile
#       -\-    _|__
#        |\___/  . \        Created on 23 Apr. 2024 at 18:22
#        \     /(((/        by hmelica
#         \___/)))/         hmelica@student.42.fr
#
# ##############################################################################

###################
##  PRINT COLOR  ##
###################

RESET		= \033[0m
BLACK		= \033[0;30m
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
GRAY		= \033[0;37m
DELETE		= \033[2K\r
GO_UP		= \033[1A\r

###################
##  PROJECT VAR  ##
###################

PICO_BOARD=pico_w
WEB_SRCS = index.html script.js style.css
PYTHON=./venv_hr_app/bin/python3

LIBFT_DIR		= ./libft
LIBFT			= ${SRCS_DIR}/libft/libft.a
LIBFT_TARGET	= $(if $(filter debug,$(MAKECMDGOALS)),debug,all)

########################
##  AUTO-EDIT ON VAR  ##
########################

DEBUG=${if ${filter ${MAKECMDGOALS}, debug}, -DCMAKE_BUILD_TYPE=Debug, }
WEB_HEADERS=${patsubst %,%.h,$(WEB_SRCS)}

all: build ${WEB_HEADERS}
	cd build && cmake .. -DPICO_BOARD=${PICO_BOARD} ${DEBUG} && make -j4

${LIBFT}: force
	@${MAKE} -C ${LIBFT_DIR} ${LIBFT_TARGET}

force:;

${WEB_HEADERS}&:${WEB_SRCS}
	${PYTHON} ./file_to_char.py ${WEB_SRCS}

push:
	cd build && openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 10000" -c "program picow_access_point_background.elf verify reset"

listen:
	minicom -b 115200 -o -D /dev/tty.usbmodem146102

gdb:
	gdb ./build/picow_access_point_background.elf

build:
	mkdir build

fclean:
	rm -rf build

re: fclean all

debug: all
	@printf "${MAGENTA}Debug mode${RESET}\n"
