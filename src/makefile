CC=gcc

TARGET = i8080_tests.exe
SOURCES = i8080_tests.c i8080.c instructions.c
OBJS = i8080_tests.o i8080.o instructions.o

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET)

clean:
	rm $(OBJS) $(TARGET)