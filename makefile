CC=gcc
CFLAGS=
LDFLAGS=

TARGET=test
OBJS=test.o snake.o

$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm $(TARGET) $(OBJS)
