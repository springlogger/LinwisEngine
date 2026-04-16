# LinwisEngine

A small C++17 console renderer that draws a rotating wireframe cube in the Windows terminal.

## Project layout

- `LinwisEngine.cpp` - demo application entry point.
- `headers/` - public engine and math interfaces.
- `engine/` - camera, renderer, and scene object implementations.
- `math/` - vector, matrix, plane, quaternion, and helper math code.
- `test/` - experimental/manual test files.

## Build

The VS Code task builds the demo with `g++` and writes the executable to `output/LinwisEngine.exe`.

Equivalent command:

```powershell
g++ -std=c++17 -Wall -Wextra -Iheaders LinwisEngine.cpp engine/*.cpp math/*.cpp -o output/LinwisEngine.exe
```

## Design notes

The code uses a small `lw` namespace, value-style math types, explicit transform matrices, and a renderer that works with geometry through the `Object3D` interface. The demo configuration is kept in one place so the scene can grow without turning `main` into engine code.
