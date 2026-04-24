NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude

SRC_DIR = coders
SRC = \
	$(SRC_DIR)/codexion.c \
	$(SRC_DIR)/codexion_aux.c \
	$(SRC_DIR)/coder.c \
	$(SRC_DIR)/coder_aux.c \
	$(SRC_DIR)/dongle.c \
	$(SRC_DIR)/dongle_aux.c \
	$(SRC_DIR)/heap.c \
	$(SRC_DIR)/heap_aux.c \
	$(SRC_DIR)/monitor.c \
	$(SRC_DIR)/scheduler.c \
	$(SRC_DIR)/scheduler_aux.c \
	$(SRC_DIR)/utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

