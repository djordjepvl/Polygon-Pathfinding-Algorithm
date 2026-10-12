# Polygon Pathfinding (Funnel Algorithm Visualizer)

A 2D app that visualizes the **funnel algorithm** for shortest paths inside a simple polygon. Give it any non-self-intersecting polygon and two points inside it, and it finds the shortest path between them that stays within the polygon.

It was made for a geometric algorithms course. For the visuals, it uses the same OpenGL wrapper boilerplate that i made for my other project (Simple OpenGL Engine).

The repo includes `Funnel Algorithm.pptx`, which explains how the algorithm works (in Serbian).

## How it works

The algorithm is split into a few smaller stages (the source is organized the same way: `triangulate`, `pathfind`, `funnel`). You can jump from stage to stage, or watch each stage run step by step.

## Controls

**Choosing a polygon**

| Input | Action |
|---|---|
| `R` | Random polygon preset (out of 20) with random start/end points |
| `←` / `→` | Previous / next polygon preset |
| Right click (on polygon) | Move the start point |
| Left click (on polygon) | Move the end point |

**Drawing your own polygon**

| Input | Action |
|---|---|
| `D` | Enter draw mode |
| Right click | Place a vertex |
| `Enter` | Finish the polygon |

**Running the algorithm**

| Input | Action |
|---|---|
| `Enter` | Skip to the next stage |
| `F` (press or hold) | Step through the current stage and watch it work |

## Building

The project needs only a C++17 compiler, CMake and OpenGL drivers. GLAD, GLM and the GLFW source are in `dependencies/` and are built with the project.

### Windows

1. Install CMake (3.21+) and a MinGW-w64 GCC:
   ```
   winget install Kitware.CMake
   winget install BrechtSanders.WinLibs.POSIX.UCRT
   ```
   Open a **new** terminal afterwards and check that `g++ --version` and `cmake --version` work.
2. From the project root:
   ```
   cmake --preset windows -DCMAKE_BUILD_TYPE=Release
   cmake --build build/windows
   ```
3. Run it from its own folder, so the polygon presets are found:
   ```
   cd build\windows
   LearningOpenGL.exe
   ```

The MinGW build links the runtime statically, so the exe needs no extra DLLs. To share it, zip the exe together with the `polygons` folder next to it.

### Linux

1. Install the dependencies.

   **Arch / CachyOS:**
   ```
   sudo pacman -S --needed base-devel cmake gdb git mesa libx11 libxrandr libxinerama libxcursor libxi xorgproto wayland wayland-protocols libxkbcommon
   ```
   **Debian / Ubuntu:**
   ```
   sudo apt install build-essential cmake gdb git libgl1-mesa-dev xorg-dev libwayland-dev libxkbcommon-dev wayland-protocols
   ```
2. Build:
   ```
   cmake --preset linux -DCMAKE_BUILD_TYPE=Release
   cmake --build build/linux
   ```
3. Run it from the build folder:
   ```
   cd build/linux
   ./LearningOpenGL
   ```

If GLFW's configure step complains about Wayland or X11 packages, add `-DGLFW_BUILD_WAYLAND=OFF` or `-DGLFW_BUILD_X11=OFF` to the first `cmake` command.

### Notes

- `CMakePresets.json` defines a `windows` preset (MinGW Makefiles) and a `linux` preset (Unix Makefiles). Each builds into its own folder (`build/windows`, `build/linux`), so you can share one checkout between both systems without the caches clashing. Presets need CMake 3.21+.
- The `polygons` folder is copied next to the executable after each build and loaded with relative paths, so always run the exe from the folder it's in.
- You need GPU drivers with OpenGL support (Mesa for AMD/Intel, the proprietary driver for NVIDIA).
- **VS Code:** install *CMake Tools* plus a debugger extension (*C/C++* for gdb, or *CodeLLDB*), then run **CMake: Select Configure Preset** and choose the one for your OS.
- Linux is case-sensitive, so includes must match filenames exactly.

## Libraries

[GLFW](https://www.glfw.org/), [GLAD](https://glad.dav1d.de/), [GLM](https://github.com/g-truc/glm)
