CXX			:= g++
CXX_FLAGS 	:= -g --std=gnu++14 -pthread

BIN			:= bin
SRC			:= src
INCLUDES	:= -Iincludes

LIB				:= libs
LIBRARIES		:= 
TEST_LIBRARIES 	:= -lgtest 

EXECUTABLE	:= Game

build: $(SRC)/*.cpp
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

clean:
	rm -rf $(BIN)

run:
	$(BIN)/$(EXECUTABLE)
