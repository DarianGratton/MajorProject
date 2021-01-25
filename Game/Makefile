CXX			:= g++
CXX_FLAGS 	:= -g --std=gnu++14 -pthread

BIN			:= bin
SRC			:= src
INCLUDES	:= -Iincludes

LIB				:= libs
LIBRARIES		:= 
TEST_LIBRARIES 	:= -lgtest 

EXECUTABLE	:= main

build: $(SRC)/*.cpp
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -L$(LIB) $^ -o $(BIN)/Game $(LIBRARIES)

clean:
	rm -rf $(BIN)
