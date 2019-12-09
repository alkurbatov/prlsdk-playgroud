OBJS = test_create_ct.o
CXX = g++

CXXFLAGS = -I/usr/include/vzctl -lvzctl2 -lpthread

TARGET = vz-test

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS)

%.o:%.cpp
	$(CXX) -g3 -c -o $@ $(CXXFLAGS) $<

clean:
	rm -f $(TARGET) *.o core.*

.PHONY : all clean

