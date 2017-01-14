
FASTUTILS_VERSION = "0.0.1"
BUILD_DATE = "$(shell date)"

CXX      ?= g++
CXXFLAGS  = -O3
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


# CC=g++

# all: OPTIMIZE_FLAGS build
# debug: DEBUG_FLAGS build
# profile: PROFILE_FLAGS build
# build: clean fastUtils

# LDFLAGS=#-static
# LIBS=-lz -lm -pthread -lpthread
# CFLAGS=-DFASTUTILS_VERSION=\"$(FASTUTILS_VERSION)\" -DBUILD_DATE=\"$(BUILD_DATE)\" #-fno-pic 

# objects=fastutils.o shuffle.o toLower.o revComp.o split.o getStat.o format.o common.o

# # compile: $(objects)

# fastUtils: $(objects)
# 	$(CC) $(objects) -o $@ ${LDFLAGS} ${LIBS}

# %.o: %.cpp
# 	$(CC) $(CFLAGS) -c $<

# clean:
# 	@rm -f $(objects) fastUtils
	
# clean-executable:
# 	@rm -f fastUtils

# DEBUG_FLAGS:
# 	$(eval CFLAGS = $(CFLAGS) -ggdb)
# 	$(eval LIBS = $(LIBS) -ggdb)

# OPTIMIZE_FLAGS:
# 	$(eval CFLAGS = $(CFLAGS) -O2)

# PROFILE_FLAGS:
# 		$(eval CFLAGS = $(CFLAGS) -pg -g)
# 	$(eval LIBS = $(LIBS) -pg -g)
