# OpenGL Fluid Simulation

This repository contains the code for an Eulerian Fluid Simulation based on the paper from [GPU gems chapter 30](https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-30-real-time-simulation-and-rendering-3d-fluids). </br>
The basic structure of the project is based on an early version of the [Hazel Engine](https://github.com/TheCherno/Hazel). 

For more details regarding the project implementation see the [documentation](https://github.com/Gianluca-Iacchini/Fluid-Simulation/blob/main/3D%20Fluid%20Simulation%20and%20Rendering.pdf)

## Compilation and Setup
The project can only be compiled on Windows platforms using Visual Studio 2022 and [premake5](https://premake.github.io/). Once downloaded, the project can be compiled by simply executing the "premake" .bat file

## Usage
Launch the project to view a simple smoke simulation. Other fluids simulations are possible by changing the parameters in the **Fluid** class.
The mouse and WASD keys can be used to move the camera around the scene, as well as the E and Q keys to move the camera up and down respectively.

## Examples
Here are some examples of the simulation
<p align="center">
  <img width="313" alt="256" src="https://user-images.githubusercontent.com/34865358/191582395-319e11b0-90d8-4222-b42e-164455f154d3.png"> <img width="313" alt="image" src="https://user-images.githubusercontent.com/34865358/191582348-3c1f9851-770d-42da-ae1e-1b04220460eb.png">
  <img src="https://user-images.githubusercontent.com/34865358/191581942-5c5a519d-9cae-4419-a551-1e785819bb02.gif" alt="animated" />
</p>
A video showing clipping and compositing can be found [here](https://youtu.be/c3fzueAaLJA)
