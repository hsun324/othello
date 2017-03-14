CC         ?= gcc
CXX        ?= g++
CPPFLAGS   = -std=c++11 -Wall -pedantic -ggdb -march=native
LDFLAGS    = -std=c++11
SOURCES    = src/board.cpp src/full_strategy.cpp src/minimax_strategy.cpp src/random_strategy.cpp src/thirdparty/spookyhash.cpp
MAINS      = src/wrapper.cpp
EXECUTABLE = ShallowAzure

OBJECTS      = $(SOURCES:%.cpp=objs/%.o)
MAIN_OBJECTS = $(MAINS:%.cpp=objs/%.o)

.SECONDEXPANSION:

all: release

debug: CPPFLAGS += -DDEBUG -O0 -g
debug: $(EXECUTABLE) tests

release: CPPFLAGS += -O3
release: $(EXECUTABLE) tests

$(EXECUTABLE): $(OBJECTS) $(MAIN_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

tests: testgame
testgame: testgame.cpp
	$(CXX) $(LDFLAGS) -o $@ $^

objs/%.o: $$(dir $$@) %.cpp
	$(CXX) -c $(CPPFLAGS) $(word 2,$^) -o $@

objs/:
	mkdir -p $@

objs/%/: objs/
	mkdir -p $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) testgame

java:
	make -C java/
java-clean:
	make -C java/ clean

.PHONY: all debug release tests testgame clean java java-clean
