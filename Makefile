CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

SRC = src/main.c \
      lib/interface/interface.c \
      lib/fcfs/fcfs.c \
      lib/priority/priority.c \
      lib/rr/rr.c \
      lib/sjf/sjf.c

OBJ = $(SRC:%.c=obj/%.o)

INCLUDES = -Ilib/interface/include \
           -Ilib/fcfs/include \
           -Ilib/priority/include \
           -Ilib/rr/include \
           -Ilib/sjf/include

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

rebuild: clean all