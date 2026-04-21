# LinwisEngine

<img width="978" height="690" alt="изображение" src="https://github.com/user-attachments/assets/82152dc6-69f4-41f1-83ae-4766a00e727c" />

A small C++17 software 3D renderer for Windows that draws into its own Win32 window using a CPU framebuffer and GDI blitting.

## Project layout

- `engine/include/lw/` - public engine headers grouped by modules such as math, core, helpers, and platform.
- `engine/src/core/` - core engine objects such as renderer, camera, mesh, scene objects, geometry, and materials.
- `engine/src/math/` - vector, matrix, quaternion, and math utility implementations.
- `engine/src/helpers/` - helper scene objects such as debug and visualization helpers.
- `engine/src/platform/win32/` - Win32 entry point, window creation, message loop, input handling, and framebuffer presentation.
- `engine/output/` - build output directory for the Windows executable.
- `test/` - experimental or manual test files.

## Current rendering model

The engine renders into its own Win32 window.

It currently uses:

- a CPU-side framebuffer
- a software renderer that writes pixels into that framebuffer
- a Win32 window created through `windows.h`
- `StretchDIBits` from `gdi32` to present the framebuffer into the window

So the pipeline is roughly:

1. scene data is updated on the CPU
2. the software renderer fills the framebuffer
3. the Win32 layer copies the framebuffer into the window

## Current architecture

The codebase is organized into several modules:

- math primitives such as vectors, matrices, and quaternions
- core scene and rendering objects such as `Object3D`, `Mesh`, `Camera`, and `Renderer`
- helper objects intended for engine users, such as axes or debug-style helpers
- a platform layer isolated under `platform/win32`

The engine logic and the Win32 layer are intentionally kept separate so platform-specific code does not leak into the renderer, math, or scene code.

Public headers are placed under:

```text
engine/include/lw/
```

and are intended to be included like this:

```cpp
#include <lw/math/Vector3.h>
#include <lw/core/Camera.h>
#include <lw/core/Mesh.h>
```

## Scene model

The engine is being structured around scene objects rather than flat render-only data.

The current idea is:

- `Object3D` is the common base for transformable scene objects
- `Mesh` extends `Object3D` with geometry and material data
- helpers can also be exposed as scene objects that users can place into the scene
- scene storage is being prepared for polymorphic usage

## Building the project

The project is currently built with a VS Code task and `g++` from MSYS2 UCRT64.

### Requirements

- Windows
- MSYS2 with the UCRT64 toolchain
- `g++`
- VS Code with tasks support

Current compiler path used by the project:

```text
C:/Program1/c++/msys64/ucrt64/bin/g++.exe
```

If your MSYS2 installation is located elsewhere, update the path in `tasks.json`.

### Build from VS Code

Use:

- `Terminal -> Run Task -> build-engine`
- or `Ctrl+Shift+B`

This compiles the engine sources and produces:

```text
engine/output/Win32Main.exe
```

### Run from VS Code

Use:

- `Terminal -> Run Task -> run-engine`

This runs the built executable.

### Current build layout

The build task currently compiles source files from:

- `engine/src/core/`
- `engine/src/math/`
- `engine/src/helpers/`
- `engine/src/platform/win32/`

and links against:

- `gdi32`

## Build command

Equivalent command to the current `tasks.json`:

```powershell
C:/Program1/c++/msys64/ucrt64/bin/g++.exe ^
  -std=c++17 ^
  -Wall ^
  -Wextra ^
  -g ^
  engine/src/core/*.cpp ^
  engine/src/math/*.cpp ^
  engine/src/helpers/*.cpp ^
  engine/src/platform/win32/Win32Main.cpp ^
  engine/src/platform/win32/Win32Window.cpp ^
  -I engine/include ^
  -lgdi32 ^
  -o engine/output/Win32Main.exe
```

Or in one line:

```powershell
C:/Program1/c++/msys64/ucrt64/bin/g++.exe -std=c++17 -Wall -Wextra -g engine/src/core/*.cpp engine/src/math/*.cpp engine/src/helpers/*.cpp engine/src/platform/win32/Win32Main.cpp engine/src/platform/win32/Win32Window.cpp -I engine/include -lgdi32 -o engine/output/Win32Main.exe
```

## Notes

The project is still evolving, and the structure is being gradually moved toward a cleaner engine layout with:

- module-based source directories
- public headers under `engine/include/lw`
- clearer separation between engine internals and user-facing API
- scene objects that can be reused both by the engine and by user code
