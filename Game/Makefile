CXX			:= g++
CXX_FLAGS 	:= -g --std=gnu++14 -pthread

BIN			:= bin
SRC			:= src
INCLUDES	:= -Iinclude

LIB			:= lib
LIBRARIES	:= -lglfw3dll -lentityx -lfreetype -lbass -lbox2d

EXECUTABLE	:= Game

TESTS		:= tests
TEST_BIN	:= bin_tests
TEST_SRC	:= $(filter-out $(SRC)/Main.cpp, $(wildcard $(SRC)/*.cpp))

TEST_LIBRARIES 	:= -lgtest

TEST_EXECUTABLE := GameTests

build: $(SRC)/*.cpp $(SRC)/*.c $(SRC)/Renderer/*.cpp $(SRC)/Scripts/*.cpp $(SRC)/WeaponScripts/*.cpp $(SRC)/Physics/*.cpp
	mkdir -p $(BIN)
	cp $(LIB)/*.dll $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

clean:
	rm -rf $(BIN)

run:
	$(BIN)/$(EXECUTABLE)

buildtests: $(TEST_SRC) $(TESTS)/*.cpp $(SRC)/*.c $(SRC)/Renderer/*.cpp $(SRC)/Scripts/*.cpp $(SRC)/WeaponScripts/*.cpp $(SRC)/Physics/*.cpp
	mkdir -p $(TEST_BIN)
	cp $(LIB)/*.dll $(TEST_BIN)  
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -L$(LIB) $^ -o $(TEST_BIN)/$(TEST_EXECUTABLE) $(LIBRARIES) $(TEST_LIBRARIES) 

cleantests:
	rm -rf $(TEST_BIN)

runtests:	
	$(TEST_BIN)/$(TEST_EXECUTABLE) > $(TEST_BIN)/test_results.txt
