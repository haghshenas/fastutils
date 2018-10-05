FASTUTILS_VERSION = "0.0.2"

CXX      ?= g++
CXXFLAGS  = -O2 -DFASTUTILS_VERSION=\"$(FASTUTILS_VERSION)\"
LDFLAGS   = -lz

all: clean-exe fastutils clean

SOURCES   = $(wildcard *.cpp)
OBJECTS   = $(SOURCES:.cpp=.o)

fastutils: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ ${LDFLAGS}

clean:
	@rm -f $(OBJECTS)
	
clean-exe:
	@rm -f fastutils
