CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

SRC = src/main.c \
      lib/interface/interface.c \
      lib/processes/processes.c \
      lib/processor/cpu.c \
      lib/scheduler/fcfs.c \
      lib/scheduler/priority.c \
      lib/scheduler/rr.c \
      lib/scheduler/sjf.c

OBJ = $(SRC:%.c=obj/%.o)

INCLUDES = -Ilib/interface/include \
           -Ilib/processes/include \
           -Ilib/processor/include \
           -Ilib/scheduler/include

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

rebuild: clean all