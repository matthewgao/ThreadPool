CC=g++ -g 
LPTHREAD= -lpthread
BOOST_LIB=-I/usr/local/Cellar/boost/1.58.0/include
TARGET=main
TEST_TARGET=testjob
TEST_OBJS=queue.o \
		  job.o \
		  test.o


OBJS=ThreadPool.o \
	 main.o

all : $(TARGET) $(TEST_TARGET)
	echo "ALL Done"
#$^ 会一次返回所有结果
#$< 一次返回一个结果
$(TARGET) : $(OBJS)
	$(CC) $(LPTHREAD) -o $@  $^

$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) $(BOOST_LIB) -o $@ $^

%.o : %.cpp
	$(CC) -c $< -o $@ $(BOOST_LIB)

clean:
	rm -f *.o main
	echo "Remove Done"
