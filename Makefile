CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O2 -D_GNU_SOURCE
LDFLAGS = -lm

SRC = main.c \
      tests.c \
      TypeInfo/TypeInfo.c \
      DynamicArray/DynamicArrayCore.c \
      DynamicArray/DynamicArrayAlgorithms.c \
      utils/utils.c \
      UI_menu/menu.c

OBJ = $(SRC:.c=.o)
TARGET = lab1

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) vgcore.* core.*

asan: CFLAGS += -fsanitize=address -fsanitize=undefined
asan: LDFLAGS += -fsanitize=address -fsanitize=undefined
asan: clean $(TARGET)
	@echo "Сборка с AddressSanitizer завершена. Запусти ./$(TARGET)"

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: clean asan valgrind test