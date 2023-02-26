# opengl_example
Computer Graphics in 2023 Winter 
--------------

## Development Environment
- Language: C++
- Framework: Visual Studio Code

## Project Overview
- Read a obj mesh file
- Translate the model/ camera in x, y and z directions
- Rotate the model/ camera around x, y, and z axes
- rotate the model/ camera according to the moving direction and distance of the mouse
- Zoom in and zoom out view of the model
- Change the field of view of the camera - vertical
- Change the values of the near and far clipping plane.
- Rotate the model around non-major axes as intuitively as possible
  - I made code to rotate the model around world x, y axes. (second function was local x, y, and z axes)

## Game Routine
- Model
- Camera
  
## How to compile
- download cmake 
- move to the directory(if you download it in desktop)
  - $ cd  ~/desktop/opengl_example-csc4870_assignment1
- build
  - $ cmake . -Bbuild
  - $ cmake --build build
- run
  - $ ./build/Debug/opengl_example

## Reference 
- GitHub: [opengl course](https://github.com/rinthel/opengl_course)
- Doc: [learnopengl](https://learnopengl.com/)
