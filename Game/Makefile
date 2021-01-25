CXX			:= g++
CXX_FLAGS 	:= -g --std=gnu++14 -Wl,--allow-multiple-definition -pthread

BIN			:= bin
SRC			:= src
TESTS		:= src/Tests
INCLUDES	:= -Iincludes
TEST_SRC	:= $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp))

LIB				:= libs
LIBRARIES		:= 
TEST_LIBRARIES 	:= -lgtest 

EXECUTABLE	:= main

build: $(SRC)/*.cpp
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -L$(LIB) $^ -o $(BIN)/Game $(LIBRARIES)

clean:
	rm -rf $(BIN)
