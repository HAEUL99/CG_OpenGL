# opengl_example
Computer Graphics in 2023 Winter 
--------------

## Development Environment
- Language: C++
- Framework: Visual Studio Code

## Project Overview
- Local illumination and Shading
  - Support for several light sources.
  - Interactively turn lights on and off.
  - interactive change the (RGBA) values associated with the global ambient light.
  - interactive change the (RGBA) values associated with the specular material properties of the objects.
- Global illumination with Ray Tracing
  - Shadows.
  - Reflections.


## Game Routine
- Local illumination
  - ![vllo 2](https://user-images.githubusercontent.com/71214577/221396051-5eca52f6-4d07-419d-9360-0f6bae228c26.GIF)
- Global illumination
  - ![vllo](https://user-images.githubusercontent.com/71214577/221396001-bfd278b4-3353-4659-b0ba-9272084f67e2.GIF)

  
## How to compile
- download cmake 
- move to the directory(if you download it in desktop)
  - $ cd  ~/desktop/opengl_example-csc5870_assignment2
- build
  - $ cmake . -Bbuild
  - $ cmake --build build
- run
  - $ ./build/Debug/opengl_example

## Reference 
- GitHub: [opengl course](https://github.com/rinthel/opengl_course)
- GitHub: [TinyRaytracer](https://github.com/diskhkme/TinyRaytracer_SFML)
- Doc: [learnopengl](https://learnopengl.com/)

