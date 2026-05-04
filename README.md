# LinwisEngine

<img width="978" height="690" alt="изображение" src="https://github.com/user-attachments/assets/82152dc6-69f4-41f1-83ae-4766a00e727c" />

A C++17 software 3D renderer for Windows. Renders entirely on the CPU into its own framebuffer, then presents it to a Win32 window via GDI.

No GPU API, no DirectX, no OpenGL. Every stage of the pipeline — vertex transform, clipping, rasterization, depth testing — runs in plain C++ code you can read, step through with a debugger, and modify freely.

## Project layout

```
engine/
  include/lw/
    math/        — Vector3, Vector4, Matrix4, Quaternion, Plane
    scene/       — Object3D, Camera, Mesh, LineSegments, MeshGeometry, LineGeometry
    graphics/    — Renderer, GraphicsTypes, Clip, Rasterizer, RenderTarget
    helpers/     — AxesHelper
    platform/    — Win32Window
    core/        — LinwisEngine, DemoConfig
  src/           — implementations mirroring the include layout
  test/          — unit tests
```

Public headers are under `engine/include/lw/` and included as:

```cpp
#include <lw/math/Vector3.h>
#include <lw/scene/Mesh.h>
#include <lw/graphics/Renderer.h>
```

## Rendering pipeline

Each frame runs through six discrete stages. Every stage is a pure function — takes an array, returns an array:

```
1. Vertex processing
   Model → World → View → Clip space
   (ProcessVertex template, works for MeshVertex and LineVertex)

2. Primitive assembly
   Indexed vertices → ClipTriangle[] or ClipSegment[]

3. Clipping
   Sutherland–Hodgman against 6 frustum planes (clip space)
   → discards or trims geometry outside the view frustum
   → fan-triangulates clipped polygons back into triangles

4. Perspective divide + viewport transform
   Clip space → NDC → Screen space (ScreenTriangle[] or ScreenSegment[])

5. Rasterization
   Triangle: edge function, barycentric interpolation, per-pixel depth test
   Line: Bresenham algorithm

6. Wireframe pass (dev mode only)
   Draws triangle edges on top of the filled geometry
```

All pipeline types (`ClipVertex`, `ScreenVertex`, `ClipTriangle`, `ScreenTriangle`, `ClipSegment`, `ScreenSegment`) are declared in `GraphicsTypes.h`.

## Architecture

The codebase separates concerns into four layers:

| Layer | Responsibility |
|---|---|
| `math` | Linear algebra — vectors, matrices, quaternions |
| `scene` | What exists in the world — objects, transforms, geometry |
| `graphics` | How things are drawn — clipping, rasterization, buffers |
| `platform` | OS interface — window, input, framebuffer presentation |

`scene` has no dependency on `graphics`. A mesh knows nothing about how it is rasterized.

The renderer dispatches on `PrimitiveType` (Triangles or Lines) and calls the appropriate pipeline path. New geometry types can be added without touching existing paths.

## Dev mode

`DemoConfig` in `Win32Main.cpp` controls engine configuration:

```cpp
lw::DemoConfig config;
config.devMode = true;   // enables wireframe overlay

lw::LinwisEngine engine(config);
```

In dev mode the renderer draws white wireframe edges over filled geometry, making polygon boundaries and clipping artifacts visible.

## Building

**Requirements:** Windows, MSYS2 UCRT64, `g++`

**From VS Code:** `Ctrl+Shift+B` → `build-engine`, then `run-engine`

**Manual build:**

```powershell
C:/Program1/c++/msys64/ucrt64/bin/g++.exe -std=c++17 -Wall -Wextra -g `
  engine/src/math/*.cpp `
  engine/src/scene/*.cpp `
  engine/src/graphics/*.cpp `
  engine/src/helpers/*.cpp `
  engine/src/platform/win32/Win32Main.cpp `
  engine/src/platform/win32/Win32Window.cpp `
  -I engine/include -lgdi32 `
  -o engine/output/Win32Main.exe
```

Output: `engine/output/Win32Main.exe`

---

## CPU rasterization: tradeoffs

Software renderers occupy a well-defined niche. Understanding their tradeoffs helps clarify when they are and are not the right tool.

### Advantages

**Full pipeline visibility.** Every stage runs as ordinary C++ code. There is no driver abstraction layer, no SPIR-V or HLSL compilation, no opaque state machine. A debugger breakpoint inside `drawFilledTriangle` is as useful as one anywhere else. This makes the renderer useful as a reference implementation and as a learning tool.

**Deterministic execution.** GPU workloads are scheduled by a driver and executed in a way that depends on hardware, driver version, and workload shape. A CPU rasterizer produces the same output on every run on every machine, with no variance introduced by shader compiler optimizations or tile ordering.

**No GPU memory management.** There are no upload heaps, descriptor sets, staging buffers, or synchronization primitives between CPU and GPU timelines. Geometry lives in regular heap memory, is read by the same CPU that submitted it, and is freed normally.

**Algorithmic freedom.** Hardware rasterizers expose a fixed interface. A software rasterizer can implement any traversal order, any interpolation scheme, or any custom coverage rule without being limited to what the GPU API exposes. This is relevant for research into non-standard rendering algorithms.

**Zero external dependencies for rendering.** The engine links only against `gdi32` for window presentation. There is no DirectX SDK, no Vulkan loader, no OpenGL driver requirement. It compiles and runs on any Windows machine with a C++ compiler.

### Limitations

**Fill rate.** Rasterization cost scales linearly with the number of pixels a triangle covers. A single triangle filling an 800×600 screen requires ~480,000 iterations of the inner loop, each performing edge function evaluations, depth interpolation, and memory writes. GPU rasterizers execute these iterations in parallel across thousands of shader units. On a single CPU thread, a frame with large screen-filling triangles is noticeably more expensive than the same frame viewed from a distance.

**No hardware parallelism.** A modern GPU has thousands of shader invocations running concurrently. A single-threaded CPU rasterizer has one. Even with multithreading, a CPU offers tens of threads, not thousands. This gap is fundamental and grows as display resolution increases.

**Memory bandwidth.** GPU memory subsystems are designed for the access pattern of rasterization — concurrent reads and writes across a large framebuffer. CPU caches are optimized for sequential access and temporal reuse. Per-pixel depth reads and writes in a non-cache-friendly order cause frequent cache misses at high resolutions.

**Practical scene complexity ceiling.** Because of the above, a CPU rasterizer becomes frame-rate limited at scene complexities that a GPU handles with headroom. This makes it unsuitable for production rendering of complex scenes, but does not affect its utility as a rendering study tool or a reference for small scenes.

**No texture hardware.** GPU texture units perform bilinear and trilinear filtering in dedicated silicon at effectively zero pipeline cost. A software implementation of the same operations is expensive and competes for the same CPU cycles as rasterization.

### When it makes sense

A CPU rasterizer is appropriate when correctness, inspectability, and algorithmic control matter more than throughput — for studying rasterization, implementing novel algorithms, or building a renderer where the pipeline itself is the subject of interest rather than a means to an end.
