all: monitor
# BUILD_DIR = ./build
SRC_DIR = ./src

EXE	= monitor

SRCS = ./src/format.cpp ./src/linux_parser.cpp ./src/process.cpp ./src/processor.cpp ./src/system.cpp ./src/main.cpp
DEPS = ./include/format.h ./include/linux_parser.h ./include/process.h ./include/processor.h ./include/system.h 
OBJS = $(SRCS:%.cpp=%.o)

CXX = g++
CXXFLAGS = -g -O0 -std=c++17 -Wall -Wextra -Wshadow


$(filter %.o,$(OBJS)): %.o: %.cpp

%.o : %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS) 

.PHONY:	$(EXE)

.PHONY: clean

clean:
	rm -f $(SRC_DIR)/*.o 
