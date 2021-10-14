CXX			 := g++
CXX_FLAGS 	 := -g --std=c++17 -pthread

BIN			 := bin
SRC			 := src
INCLUDES	 := -Iincludes -Iincludes/pytorch -Iincludes/pytorch/torch/csrc/api/include

LIB			 := -Llibs -Llibs/pytorch
LIBRARIES	 := -lc10 -ltorch -lcaffe2

PREPROCESSOR := -D_GLIBCXX_USE_CXX11_ABI=0

EXECUTABLE	 := Agent

build: $(SRC)/*.cpp
	mkdir -p $(BIN)
	set USE_CUDA=0
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

clean:
	rm -rf $(BIN)

run:
	$(BIN)/$(EXECUTABLE)
