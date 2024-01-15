# W_Engine

**Windows only Rendering Engine**

## Includes

- [**GLAD**](https://glad.dav1d.de/) - OpenGL Loader-Generator, manages OpenGL function pointers.
- [**GLFW**](https://www.glfw.org/docs/3.3/quick.html) - Provides a simple API for creating windows, contexts, and managing input.
- [**assimp**](https://github.com/assimp/assimp) - Open Asset Import Library, used for importing various well-known 3D model formats.
- [**stb_image**](https://github.com/nothings/stb/tree/master) - Single header file used for importing and loading textures.
- [**glm**](https://github.com/g-truc/glm) - OpenGL Mathematics, a header-only math library designed for OpenGL.

## Usage

### Importing

The engine can import textures, shaders, and models from the `resources` folder.

**Model Importation:**
- Only supports `.obj` files.
- Supports only one diffuse, normal, and specular texture per mesh.

Paths should be specified relative to the `resources` folder and should use double backslashes `\\` to represent folder structure.

### Camera Movement

**Controls:**
- `W` : Move forward.
- `S` : Move backward.
- `A` : Move left.
- `D` : Move right.

Click and drag to rotate the camera.

## Future Work

- Textures are not currently loaded correctly.
- Materials are not displayed correctly.
