DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c, ${DIR_OBJ}/%.o, $(notdir $(SRC)))

CC = gcc
CFLAGS = -g -Wall -I$(DIR_INC)

TARGET = np_simple
TARGET2 = np_single_proc

all:${TARGET} ${TARGET2}

${TARGET}:$(filter-out ${DIR_OBJ}/$(TARGET2).o, $(OBJ))
	$(CC) -o $@ $^

${TARGET2}:$(filter-out ${DIR_OBJ}/$(TARGET).o, $(OBJ))
	$(CC) -o $@ $^

$(DIR_OBJ)/%.o: ${DIR_SRC}/%.c
	mkdir -p $(DIR_OBJ)
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*.o
