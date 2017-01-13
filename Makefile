CXX      ?= g++
CXXFLAGS  = -O3
LDFLAGS   = -lz

all: clean-exe build clean

SOURCES   = $(wildcard *.cpp)
BINARIES  = $(SOURCES:.cpp=)

build: $(BINARIES)

clean:
	@rm -f *.o
	
clean-exe:
	@rm -f $(BINARIES)
