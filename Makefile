CC=g++
CFLAG= -g -Wall -std=c++11
LPTHREAD= -lpthread
BOOST_LIB=-I/usr/local/Cellar/boost/1.58.0/include 

#How the make tool find out the cpp and h file.
#I didn't specify the route??
SRC_DIR=src
BIN_DIR=bin

TARGET=$(BIN_DIR)/main
TEST_TARGET=$(BIN_DIR)/testjob

TEST_OBJS=$(SRC_DIR)/queue.o \
		  $(SRC_DIR)/job.o \
		  $(SRC_DIR)/test.o


OBJS=$(SRC_DIR)/ThreadPool.o \
	 $(SRC_DIR)/main.o \
	 $(SRC_DIR)/queue.o \
	 $(SRC_DIR)/job.o 

all : $(TARGET) $(TEST_TARGET)
	echo "ALL Done"
#$^ 会一次返回所有结果
#$< 一次返回一个结果
$(TARGET) : $(OBJS)
	$(CC) $(CFLAG)  -o $@  $^ $(LPTHREAD)

$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) $(CFLAG) $(BOOST_LIB) -o $@ $^

%.o : %.cpp
	$(CC) $(CFLAG) -c $< -o $@ $(BOOST_LIB)

%.dep : %.cpp
	$(CC) -M $< > $@ $(BOOST_LIB)

include $(OBJS:.o=.dep)
include $(TEST_OBJS:.o=.dep)

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) $(TEST_TARGET) $(SRC_DIR)/*.dep
	echo "Remove Done"
