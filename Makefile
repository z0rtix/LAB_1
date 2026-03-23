CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O2 -D_GNU_SOURCE
LDFLAGS = -lm

SRC = main.c \
      TypeInfo/TypeInfo.c \
      DynamicArray/DynamicArrayCore.c \
      DynamicArray/DynamicArrayAlgorithms.c \
      utils/utils.c \
      UI_menu/menu.c \
      Tests/tests_of_int.c \
      Tests/tests_of_str.c 

OBJ = $(SRC:.c=.o)

main: $(OBJ)
	$(CC) $(OBJ) -o main $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) main main_asan

.PHONY: clean

asan: CFLAGS += -fsanitize=address -fsanitize=undefined
asan: LDFLAGS += -fsanitize=address
asan: clean main