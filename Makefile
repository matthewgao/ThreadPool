#How the make tool find out the cpp and h file.
#I didn't specify the route??
SRC_DIR=src
BIN_DIR=bin
LIB_DIR=lib


CC=g++
CFLAG=-g -Wall -std=c++11 -DBOOST_LOG_DYN_LINK
LPTHREAD=-lpthread
BOOST_LIB=-lboost_log-mt


INCLUDE=-I/usr/local/include
LIB=-L/usr/local/lib -L lib


TARGET=$(BIN_DIR)/main
TEST_TARGET=$(BIN_DIR)/testjob
SO_TARGET=$(LIB_DIR)/libthreadpool.so


OBJS=$(SRC_DIR)/main.o 
SO_OBJS=$(SRC_DIR)/ThreadPool.o \
		$(SRC_DIR)/queue.o \
		$(SRC_DIR)/job.o
TEST_OBJS=$(SRC_DIR)/queue.o \
		  $(SRC_DIR)/job.o \
		  $(SRC_DIR)/test.o



all : $(SO_TARGET) $(TARGET) $(TEST_TARGET)
	echo "ALL Done"
#$^ 会一次返回所有结果
#$< 一次返回一个结果
$(TARGET) : $(OBJS)
	$(CC) $(CFLAG)  -o $@  $^ $(LIB) $(LPTHREAD) $(BOOST_LIB) -lthreadpool

$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) $(CFLAG) $(INCLUDE) -o $@ $^ $(LIB) $(BOOST_LIB) -lthreadpool

$(SO_TARGET) : $(SO_OBJS)
	$(CC) $(CFLAG) $(INCLUDE) -shared -fpic -o $@ $^ $(LIB) $(LPTHREAD) $(BOOST_LIB)


%.o : %.cpp
	$(CC) $(CFLAG)  -c $< -o $@  $(INCLUDE)

%.dep : %.cpp
	$(CC) -M $< > $@ $(INCLUDE)

include $(OBJS:.o=.dep)
include $(TEST_OBJS:.o=.dep)
include $(SO_OBJS:.o=.dep)

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) $(TEST_TARGET) $(SRC_DIR)/*.dep $(LIB_DIR)/*.so
	echo "Remove Done"
