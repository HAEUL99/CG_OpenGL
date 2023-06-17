# Computer Graphics
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


## Results
- Local illumination
  - ![vllo](https://user-images.githubusercontent.com/71214577/230675306-8e1f451a-a28d-4144-a909-6630369d433f.GIF)
 
- Global illumination
  - <img src="https://user-images.githubusercontent.com/71214577/230677910-c5623dc2-3d51-488f-b26f-59a4831d10b2.png" width="640" height="350">
  
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
- Doc: [learnopengl](https://learnopengl.com/)
- GitHub: [TinyRaytracer](https://github.com/ssloy/tinyraytracer)
- GitHub: [Raytracer](https://github.com/FacelessLaw/RayTracing/tree/db0d6480ca35527e08f6f7c95fe94c7fe5682836)

