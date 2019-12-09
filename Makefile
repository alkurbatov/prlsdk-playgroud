OBJS = main.o consts.o server.o vm.o callback.o job.o session.o prototype.o test.o stat.o collection.o
CXX = g++

CXXFLAGS = -I/usr/include/prlsdk/ -rdynamic -lprl_sdk -ldl -lpthread

TARGET = play

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS)

%.o:%.cpp
	$(CXX) -g2 -c -o $@ $(CXXFLAGS) $<

clean:
	rm -f $(TARGET) *.o *.png core.*

.PHONY : all clean

