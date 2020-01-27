FASTUTILS_VERSION = $(shell cat VERSION)

CXX      ?= g++
CXXFLAGS  = -O2 -std=c++11 -DFASTUTILS_VERSION=\"$(FASTUTILS_VERSION)\"
LDFLAGS   = -lz

# all: clean-exe fastutils clean
all: fastutils

SOURCES   = $(wildcard src/*.cpp)
OBJECTS   = $(SOURCES:.cpp=.o)

fastutils: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ ${LDFLAGS}

clean:
	@rm -f $(OBJECTS)
	
clean-exe:
	@rm -f fastutils
	
clean-all: clean clean-exe
