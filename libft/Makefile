### COMPILE ####################################################################

CC = cc
CFLAGS = -Werror -Wextra -Wall -I $(INC_DIR)

### LIB NAME ###################################################################

NAME = libft.a

### SRC ########################################################################

SRC  = ft_isalpha.c
SRC += ft_isdigit.c
SRC += ft_isalnum.c
SRC += ft_isascii.c
SRC += ft_isprint.c
SRC += ft_isspace.c
SRC += ft_strlen.c
SRC += ft_memset.c
SRC += ft_bzero.c
SRC += ft_memcpy.c
SRC += ft_memmove.c
SRC += ft_strlcpy.c
SRC += ft_strlcat.c
SRC += ft_toupper.c
SRC += ft_tolower.c
SRC += ft_strchr.c
SRC += ft_strrchr.c
SRC += ft_strncmp.c
SRC += ft_memchr.c
SRC += ft_memcmp.c
SRC += ft_strnstr.c
SRC += ft_atoi.c
SRC += ft_atol.c
SRC += ft_calloc.c
SRC += ft_strdup.c
SRC += ft_substr.c
SRC += ft_strjoin.c
SRC += ft_strjoinch.c
SRC += ft_strtrim.c
SRC += ft_split.c
SRC += ft_itoa.c
SRC += ft_strmapi.c
SRC += ft_striteri.c
SRC += ft_putchar_fd.c
SRC += ft_putstr_fd.c
SRC += ft_putendl_fd.c
SRC += ft_putnbr_fd.c
SRC += ft_lstnew.c
SRC += ft_lstadd_front.c
SRC += ft_lstsize.c
SRC += ft_lstlast.c
SRC += ft_lstadd_back.c
SRC += ft_lstdelone.c
SRC += ft_lstclear.c
SRC += ft_lstiter.c
SRC += ft_lstmap.c
SRC += ft_free_null.c
SRC += ft_free_str_array.c
SRC += ft_is_float.c
SRC += ft_str_isdigit.c
SRC += ft_atof.c
SRC += ft_strcmp.c

### PRINTF #######################################################################

SRC_PRINTF  = ft_printf.c
SRC_PRINTF += ft_printf_chars.c
SRC_PRINTF += ft_printf_nbrs.c

### GET_NEXT_LINE ###########################################################

SRC_GNL  = get_next_line.c
SRC_GNL += get_next_line_utils.c

### SRCS #######################################################################

SRCS = $(addprefix $(SRC_DIR), $(SRC))
SRCS_PRINTF = $(addprefix $(PRINTF_DIR), $(SRC_PRINTF))
SRCS_GNL = $(addprefix $(GNL_DIR), $(SRC_GNL))

### HEAD #######################################################################

INC_DIR = ./includes/
PRINTF_DIR = printf/
GNL_DIR = GNL/

### OBJS #######################################################################

OBJS := $(SRCS:.c=.o)
PRINTF_OBJS := $(SRCS_PRINTF:.c=.o)
GNL_OBJS := $(SRCS_GNL:.c=.o)

### COMMANDS ###################################################################

RM   = rm -f
AR   = ar rcs

################################################################################

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)
	$(AR) $(NAME) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

clean :
	$(RM) $(OBJS) $(PRINTF_OBJS) $(GNL_OBJS)

fclean : clean
	$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean re
