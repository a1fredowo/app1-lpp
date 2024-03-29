CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = app1
SRC = app.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)