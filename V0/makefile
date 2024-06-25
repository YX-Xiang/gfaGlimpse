SRCS := ${wildcard *.cpp}
OBJS := $(SRCS:.cpp=.o)

PROG := gfaGlimpse.exe
CXX := g++
CFLAGS := -Wall -Ofast -std=c++1z -fopenmp

$(PROG): $(OBJS)
	$(CXX) ${CFLAGS} -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
