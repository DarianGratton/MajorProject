# GameAgent - Reinforcement Learning AIs for Lightweight Games

***NOTE: The GameAgent Library currently only supports Windows and requires C++17.***

GameAgent is a simple to use C++ based library, that utilizes the PyTorch C++ Frontend to provide reinforcement learning AIs for lightweight games. 

## Requirements

### PyTorch

The GameAgent library requires PyTorch C++. 

Download for PyTorch C++ can be found here: https://pytorch.org/get-started/locally/

***NOTE: The current implementation for the library only supports the CPU computing platform.***

Once downloaded just add an absolute path to libtorch in the CMake command arguments.

```
-DCMAKE_PREFIX_PATH="C:\\PathToLibTorch\\libtorch"
```

***NOTE: Given the current implementation it would be easier to add this into the CMakeSettings.json file using VS.***

### Gnuplot (Visualizer Only)

If using the Visualizer System, the GameAgent library requires Gnuplot 5.2.8.

Download for Gnuplot can be found here: http://www.gnuplot.info/download.html

## Installation

### VS2019 (CMake)

## Classes

### State

Defines what constitutes a state in the environment and contains functions for manipulating said state. 

```
#include <GameAgent/GameAgent.h>

GameAgent::State state;
state.AddDelta("X", 9.0f);
state.AddDelta("Y"); 
```

### Environment

Defines a game environment that the Agent acts in and stores all the information that the Agent needs from acting in it.

```
#include <GameAgent/GameAgent.h>

GameAgent::State initialState;
state.AddDelta("X", 9.0f);
state.AddDelta("Y");

GameAgent::Environment environment(initialState);
```

### Network Parameters

Defines the parameters needed to initialize a new NetworkAgent.

Note that each NetworkAgent requires different parameters, example shown is for the ACERAgent NetworkAgent.

```
#include <GameAgent/GameAgent.h>

GameAgent::Networks::ACERParameters params;
params.learningRate = 1e-3f;
params.nActions = 1;
params.inputDims = 4;
params.memSize = 1000000;
...
```

### Agent

The frontend of the GameAgent library. 

Provides the interface needed to run the agent on an environment.

Note: any changes made on the environment variable will affect the environment in the Agent object.

***NOTE: Currently Agent construction requires environment to be a shared_ptr. In the future this will be abstracted away.***

```
#include <GameAgent/GameAgent.h>

GameAgent::Networks::ACERParameters params;
params.learningRate = 1e-3f;
params.nActions = 1;
params.inputDims = 4;
params.memSize = 1000000;
...

GameAgent::State initialState;
state.AddDelta("X", 9.0f);
state.AddDelta("Y");

auto environment = std::make_shared<GameAgent::Environment>(initialState);
GameAgent::Agent agent(env, params);
```

### Visualizer

Defines various functions for creating visual graphs and charts.

Note: the example uses hardcoded vectors, however in a game environment it can be used to graph environment varibles such as reward history.

Requires Gnuplot 5.2.8.

```
#include <GameAgent/GameAgent.h>
#include <vector>

GameAgent::Visualizer visualizer;
std::vector<float> x = { 1, 2, 3, 4, 5 };
std::vector<float> y = { 2, 4, 6, 8, 10 };
visualizer.PlotLine(x, y, "LinePlot.png", "Label"); 
```

### Utility Storage

A storage system for storing the environment's different initial states and their average utilities.

Useful for comparing how the agent performs with different initial states. 

Note: Gets initial state and utility from environment stored in the agent.

```
#include <GameAgent/GameAgent.h>

// Define NetworkAgent and Environment
...

GameAgent agent(env, params);

// Run Agent on Environment
...

agent.SaveUtility();
```

## NetworkAgents

### ACERAgent

A NetworkAgent which utilizes the deep reinforcement learning algorithm Actor Critic with Experience Replay (ACER).

ACER is an off-policy extension of the Asynchronus Advantage Actor Critic (A3C) which greatly increases the sample efficiency and decreases the data correlation.

Note: Currently only supports discrete action spaces.

Paper: https://arxiv.org/abs/1611.01224v2

***NOTE: Current implementation omits the running of multiple agents training in paraell due to time restraints. Current implementation also doesn't support environments that run multiple actions each frame.***
