# OpenGL Fluid Simulation

This repository contains the code for an Eulerian Fluid Simulation based on the paper from [GPU gems chapter 30](https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-30-real-time-simulation-and-rendering-3d-fluids)

The basic structure of the project is based on an early version of the [Hazel Engine](https://github.com/TheCherno/Hazel)

## Compilation and Setup
The project can only be compiled on Windows platforms using Visual Studio 2022 and [premake5](https://premake.github.io/). Once downloaded, the project can be compiled by simply executing the "premake" .bat file

------------------------

## Usage
Launch the project to view a simple smoke simulation. Other fluids simulations are possible by changing the parameters in the "Fluid" class.
The mouse and WASD keys can be used to move the camera around the scene, as well as the E and Q keys to move the camera up and down respectively.
