# LinwisEngine

 <img width="978" height="690" alt="изображение" src="https://github.com/user-attachments/assets/82152dc6-69f4-41f1-83ae-4766a00e727c" />

A small C++17 console renderer that draws a rotating wireframe cube in the Windows terminal.

## Project layout

- `LinwisEngine.cpp` - demo application entry point.
- `platform` - platform specified code
- `headers/` - public engine and math interfaces.
- `engine/` - camera, renderer, and scene object implementations.
- `math/` - vector, matrix, plane, quaternion, and helper math code.
- `test/` - experimental/manual test files.

## Build

The VS Code task builds the demo with `g++` and writes the executable to `output/LinwisEngine.exe`.

Equivalent command:

```powershell
g++ -std=c++17 -Wall -Wextra -I headers -I platform/win32/headers LinwisEngine.cpp engine/*.cpp math/*.cpp platform/win32/*.cpp -lgdi32  -o output/LinwisEngine.exe
```

## Design notes

The code uses a small `lw` namespace, value-style math types, explicit transform matrices, and a renderer that works with geometry through the `Object3D` interface. The demo configuration is kept in one place so the scene can grow without turning `main` into engine code.
