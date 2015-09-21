CC=g++ -g -Wall
LPTHREAD= -lpthread
BOOST_LIB=-I/usr/local/Cellar/boost/1.58.0/include 
TARGET=main
TEST_TARGET=testjob
TEST_OBJS=queue.o \
		  job.o \
		  test.o


OBJS=ThreadPool.o \
	 main.o \
	 queue.o \
	 job.o

all : $(TARGET) $(TEST_TARGET)
	echo "ALL Done"
#$^ 会一次返回所有结果
#$< 一次返回一个结果
$(TARGET) : $(OBJS)
	$(CC)  -o $@  $^ $(LPTHREAD)

$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) $(BOOST_LIB) -o $@ $^

%.o : %.cpp
	$(CC) -c $< -o $@ $(BOOST_LIB)

%.dep : %.cpp
	$(CC) -M $< > $@

include $(OBJS:.o=.dep)
include $(TEST_OBJS:.o=.dep)

clean:
	rm -f *.o main *.dep
	echo "Remove Done"
