CC = g++
TARGET = Terminal
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) 

clean:
	rm -f $(TARGET)

