CC         ?= gcc
CXX        ?= g++
CPPFLAGS   = -std=c++11 -Wall -pedantic -ggdb -march=native
LDFLAGS    = -std=c++11
SOURCES    = src/player.cpp src/board.cpp src/full_strategy.cpp src/minimax_strategy.cpp src/random_strategy.cpp
MAINS      = src/wrapper.cpp
EXECUTABLE = player

OBJECTS      = $(SOURCES:%.cpp=objs/%.o)
MAIN_OBJECTS = $(MAINS:%.cpp=objs/%.o)

all: release

debug: CPPFLAGS += -DDEBUG -O0 -g
debug: $(EXECUTABLE) tests

release: CPPFLAGS += -O3
release: $(EXECUTABLE) tests

$(EXECUTABLE): $(OBJECTS) $(MAIN_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

tests: testgame testminimax
testgame: testgame.cpp
	$(CXX) $(LDFLAGS) -o $@ $^
testminimax: $(OBJECTS) testminimax.o
	$(CXX) $(LDFLAGS) -o $@ $^

objs/%.o: $(dir $@) %.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

objs/:
	mkdir -p $@

objs/%/:
	mkdir -p $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) testgame testminimax

java:
	make -C java/
java-clean:
	make -C java/ clean

.PHONY: all debug release tests testgame testminimax clean java java-clean
