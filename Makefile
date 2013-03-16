CXX := g++
CXXFLAGS := -O2 -std=c++11

INCLUDES := -I. -I/usr/include/SDL
LIBS := -lSDL -lGL -lGLU -lSDL_ttf -lSDL_image -lSDL_mixer

SOURCES := $(wildcard Sources/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all
all: streams

.PHONY: clean
clean:
	rm -f Sources/*.o streams

streams: $(OBJS)
	$(CXX) $(LIBS) $(OBJS) -o streams

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

