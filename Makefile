CC=gcc
CFLAGS=-Iinclude
OBJ = src/main.o src/set.o

Jaccard: $(OBJ)
    $(CC) -o $@ $^ $(CFLAGS)

clean:
    rm -f src/*.o Jaccard
