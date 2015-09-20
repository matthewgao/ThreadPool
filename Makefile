CC=g++ -g 
LPTHREAD= -lpthread
TARGET=main
OBJS=ThreadPool.o \
	 main.o

all : $(TARGET)
	echo "ALL Done"
#$^ 会一次返回所有结果
#$< 一次返回一个结果
$(TARGET) : $(OBJS)
	$(CC) $(LPTHREAD) -o $@  $^

%.o : %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f *.o main
	echo "Remove Done"
