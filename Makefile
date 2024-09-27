######################## SETTINGS ########################

NAME = minishell

CC = cc

FLAGS = -Wall -Wextra -Werror

LINKFLAGS = -lreadline

LIBS = libft \

INCLUDES =	includes \
			${foreach lib, ${LIBS}, ${lib} ${lib}/includes}

VALGRIND = valgrind --suppressions=readline.supp --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all

######################## SOURCES ########################

PARSING =	split.c \
			parser.c \
			vars.c \
			heredocs.c \
			files.c \
			cmds.c \
			args.c \
			utils.c

BUILTIN =	builtin.c \
			echo.c \
			cd.c \
			pwd.c \
			env.c \
			unset.c \
			export.c \
			exit.c

EXEC =	exec.c \
		utils.c

UTILS =	init.c \
		errors.c \
		checks.c \
		signals.c \
		env.c

SRCS_NAMES =	main.c \
				${addprefix parsing/, ${PARSING}} \
				${addprefix exec/, ${EXEC}} \
				${addprefix builtin/, ${BUILTIN}} \
				${addprefix utils/, ${UTILS}} \

SRCS_DIR = srcs/

SRCS = ${addprefix ${SRCS_DIR}, ${SRCS_NAMES}}

OBJS_DIR = objs/

OBJS = ${addprefix ${OBJS_DIR}, ${SRCS_NAMES:.c=.o}}

HEADERS =	includes/*.h

######################## BASIC RULES ########################

.PHONY: all re clean fclean norm test leak

all : ${NAME}

re : fclean
	${MAKE} all

clean :
	-${foreach lib, ${LIBS}, ${MAKE} clean -C ${lib}}
	rm -rf ${OBJS_DIR}
	rm -f readline.supp

fclean : clean
	-${foreach lib, ${LIBS}, ${MAKE} fclean -C ${lib}}
	rm -f ${NAME}

norm :
	${foreach lib, ${LIBS}, ${MAKE} norm -C ${lib}}
	norminette -R CheckForbiddenSourceHeader ${SRCS}
	norminette -R CheckDefine ${INCLUDES}

######################## COMPILATION ########################

${NAME} : SUB_MODULE ${OBJS_DIR} ${OBJS}
	${foreach lib, ${LIBS}, ${MAKE} -C ${lib}}
	${CC} ${FLAGS} ${OBJS} ${foreach lib, ${LIBS},${lib}/${lib}.a} -o $@ ${LINKFLAGS}

debug : ${OBJS_DIR} ${OBJS}
	${foreach lib, ${LIBS}, ${MAKE} -C ${lib}}
	${CC} ${FLAGS} -g3 -fsanitize=address ${OBJS} ${foreach lib, ${LIBS},${lib}/${lib}.a} -o ${NAME} ${LINKFLAGS}

${OBJS_DIR} :
	mkdir $@
	mkdir $@parsing
	mkdir $@exec
	mkdir $@builtin
	mkdir $@utils

${OBJS_DIR}%.o : ${SRCS_DIR}%.c ${HEADERS}
	${CC} ${FLAGS} ${CPPFLAGS} ${foreach include, ${INCLUDES},-I ${include}} -c $< -o $@

SUB_MODULE :
	git submodule update --init

######################## TEST ########################

test : all
	clear
	./${NAME}

leak : all
	echo "{\nignore_libreadline_leaks\n Memcheck:Leak\n ...\n obj:*/libreadline.so.*\n }" > readline.supp
	${valgrind} ./${NAME}
	rm readline.supp