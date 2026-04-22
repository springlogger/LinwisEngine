# LinwisEngine

<img width="978" height="690" alt="изображение" src="https://github.com/user-attachments/assets/82152dc6-69f4-41f1-83ae-4766a00e727c" />

A small C++17 software 3D renderer for Windows that draws into its own Win32 window using a CPU framebuffer and GDI blitting.

## Project layout

- `engine/include/lw/` - public engine headers grouped by modules such as math, scene, graphics, helpers, and platform.
- `engine/src/math/` - vector, matrix, quaternion, and math utility implementations.
- `engine/src/scene/` - scene-level objects such as `Object3D`, `RenderableObject`, `Mesh`, `LineSegments`, `Camera`, and geometry classes.
- `engine/src/graphics/` - renderer, framebuffer, render target, depth buffer, and software rasterization code.
- `engine/src/helpers/` - helper scene objects such as `AxesHelper` and other debug-style objects.
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
2. renderable scene objects are processed by the software renderer
3. the renderer fills the framebuffer
4. the Win32 layer copies the framebuffer into the window

## Current architecture

The codebase is organized into several modules:

- `math` for vectors, matrices, quaternions, and math helpers
- `scene` for scene objects and renderable entities such as meshes and line-based objects
- `graphics` for the renderer, buffers, and rasterization pipeline
- `helpers` for user-facing debug and visualization objects such as axes helpers
- `platform/win32` for window creation, input, and presentation

The scene layer and the graphics layer are intentionally separated:

- `scene` describes what exists in the world
- `graphics` describes how those objects are rendered

This keeps scene objects, transforms, and geometry ownership separate from the low-level rendering pipeline.

Public headers are placed under:

```text
engine/include/lw/
```

and are intended to be included like this:

```cpp
#include <lw/math/Vector3.h>
#include <lw/scene/Object3D.h>
#include <lw/scene/Mesh.h>
#include <lw/graphics/Renderer.h>
#include <lw/helpers/AxesHelper.h>
```

## Scene model

The engine is being structured around scene objects rather than flat render-only data.

The current design direction is:

- `Object3D` is the common base for transformable scene objects
- `RenderableObject` extends `Object3D` for objects that can produce renderable data
- `Mesh` represents triangle-based renderable geometry
- `LineSegments` represents line-based renderable geometry
- helpers such as `AxesHelper` are built on top of line-based scene objects
- the renderer works with renderable object types rather than helper-specific classes

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

- `engine/src/math/`
- `engine/src/scene/`
- `engine/src/graphics/`
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
  engine/src/math/*.cpp ^
  engine/src/scene/*.cpp ^
  engine/src/graphics/*.cpp ^
  engine/src/helpers/*.cpp ^
  engine/src/platform/win32/Win32Main.cpp ^
  engine/src/platform/win32/Win32Window.cpp ^
  -I engine/include ^
  -lgdi32 ^
  -o engine/output/Win32Main.exe
```

Or in one line:

```powershell
C:/Program1/c++/msys64/ucrt64/bin/g++.exe -std=c++17 -Wall -Wextra -g engine/src/math/*.cpp engine/src/scene/*.cpp engine/src/graphics/*.cpp engine/src/helpers/*.cpp engine/src/platform/win32/Win32Main.cpp engine/src/platform/win32/Win32Window.cpp -I engine/include -lgdi32 -o engine/output/Win32Main.exe
```

## Notes

The project is still evolving, and the structure is being gradually moved toward a cleaner engine layout with:

- module-based source directories
- public headers under `engine/include/lw`
- clearer separation between engine internals and user-facing API
- separate scene and graphics layers
- scene objects that can be reused both by the engine and by user code
