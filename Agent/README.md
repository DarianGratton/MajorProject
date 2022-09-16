# GameAgent - Reinforcement Learning AIs for Lightweight Games

***NOTE: The GameAgent Library currently only supports Windows and requires C++17.***

GameAgent is a simple to use C++ based library, that utilizes the PyTorch C++ Frontend to provide reinforcement learning AIs for lightweight games. 

## Installation

### Requirements

#### PyTorch

The GameAgent library requires PyTorch C++. 

Download for PyTorch C++ can be found here: https://pytorch.org/get-started/locally/

***NOTE: The current implementation for the library only supports the CPU computing platform.***

Once downloaded just add an absolute to libtorch in the CMake command arguments.

```
-DCMAKE_PREFIX_PATH="C:\\PathToLibTorch\\libtorch"
```

***NOTE: Given the current implementation it would be easier to just add this into the CMakeSettings.json file using VS2019.***

#### Gnuplot (Visualizer Only)

### CMake

#### VS2019

## Tutorial

### Agent

### Visualizer

### Utility Storage

## Usable Networks
### ACER

## References
