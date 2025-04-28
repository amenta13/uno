CC = gcc
CFLAGS = -Wall -g
SRC_DIR = src
OBJ = $(SRC_DIR)/main.o $(SRC_DIR)/setup.o $(SRC_DIR)/play.o
EXEC = uno

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compile main.c to main.o
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/setup.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

# Compile setup.c to setup.o
$(SRC_DIR)/setup.o: $(SRC_DIR)/setup.c $(SRC_DIR)/setup.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/setup.c -o $(SRC_DIR)/setup.o

# Compile play.c to play.o
$(SRC_DIR)/play.o: $(SRC_DIR)/play.c $(SRC_DIR)/play.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/play.c -o $(SRC_DIR)/play.o

# Clean up generated files
clean:
	rm -f $(OBJ) $(EXEC)

# Run uno game
run: all
	./$(EXEC)