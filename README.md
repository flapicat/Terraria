# Terraria

A Terraria-inspired 2D sandbox written from scratch in C++23 and OpenGL 4.6. It's a hobby project and a learning exercise.

The repository has two parts:

- **Engine**: a small static library with the window and input layer, events, a layer stack, logging, and a batched 2D renderer (shaders, textures, texture atlases, an orthographic camera).
- **Terraria**: the game itself, built on the engine. It currently generates a tile world procedurally and lets you fly the camera around it.

## Features

- Procedural world generation with [FastNoiseLite](https://github.com/Auburn/FastNoiseLite): rolling hills, occasional tall mountains, a dirt layer of varying depth over stone, patches of stone in the dirt and dirt in the stone, caves that grow larger with depth, and open caverns deep underground
- A random seed every run, which is logged at startup
- Selectable world sizes: Small (4200×1200), Medium (6400×1800), Big (8400×2400)
- A batched quad renderer that draws up to 10,000 quads per draw call, with multiple textures per batch
- Tiles drawn from a texture atlas, with random variants per tile type

## Controls

| Input             | Action             |
|-------------------|--------------------|
| `W` `A` `S` `D`   | Move the camera    |
| `Left Ctrl` (hold)| Move faster        |
| Mouse wheel       | Zoom in or out     |
| `Esc`             | Quit               |

## Requirements

- Windows 10 or 11, 64-bit
- Visual Studio 2022 or 2026 with the **Desktop development with C++** workload
- A GPU that supports OpenGL 4.6

All other dependencies are in `vendor/`, including the premake executable.

## Building

1. Clone the repository:
   ```
   git clone https://github.com/flapicat/Terraria.git
   ```
2. Generate the Visual Studio solution by running the script for your Visual Studio version:
   - `scripts/generate-sln22.bat` for Visual Studio 2022
   - `scripts/generate-sln26.bat` for Visual Studio 2026
3. Open `Terraria.sln` and build. **Terraria** is already set as the startup project.

Build configurations:

| Configuration | Purpose                                   |
|---------------|-------------------------------------------|
| `Debug`       | Debug symbols, no optimizations           |
| `Release`     | Optimized, keeps logging                  |
| `Dist`        | Optimized build for distribution          |

Output goes to `bin/<config>-windows-x86_64/Terraria/`. Each build copies `Terraria/Assets/` next to the executable, so you can run `Terraria.exe` directly from that folder. The game loads files from `Assets/` relative to the working directory.

## Project structure

```
Engine/src/
  Core/        App, Window, Input, Layer/LayerStack, Log, Time, Random
  Event/       Window, keyboard and mouse events
  Renderer/    Renderer, Shader, Texture, SubTexture, Camera, buffers
Terraria/
  Assets/      Shaders and textures
  src/         Game layer and world generation (World/)
scripts/       Solution generation scripts
vendor/        Third-party libraries and premake
premake5.lua   Build configuration
```

## Third-party libraries

| Library | License |
|---|---|
| [GLFW](https://www.glfw.org/) | zlib |
| [glad](https://github.com/Dav1dde/glad) | MIT / Public domain |
| [GLM](https://github.com/g-truc/glm) | MIT |
| [Dear ImGui](https://github.com/ocornut/imgui) | MIT |
| [spdlog](https://github.com/gabime/spdlog) | MIT |
| [stb_image](https://github.com/nothings/stb) | MIT / Public domain |
| [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) | MIT |
| [premake](https://premake.github.io/) | BSD 3-Clause |

## License

The source code is released under the [MIT License](LICENSE).

The MIT License covers this project's code only. The files in `Terraria/Assets/Textures/` are **not** covered and belong to their respective owners. Third-party libraries in `vendor/` keep their own licenses.

## Disclaimer

This is an unofficial fan project. It is not affiliated with or endorsed by Re-Logic. *Terraria* is a trademark of Re-Logic.
