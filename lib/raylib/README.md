# Raylib Submodule-able Fork
Based on raylib v5.5 <br>

<img align="left" style="width:260px" src="https://github.com/raysan5/raylib/blob/master/logo/raylib_logo_animation.gif" width="288px">
<h3><b>About Raylib</b></h3>

**raylib is a simple and easy-to-use library to enjoy videogames programming.**

raylib is highly inspired by Borland BGI graphics lib and by XNA framework and it's especially well suited for prototyping, tooling, graphical applications, embedded systems and education.

*NOTE for ADVENTURERS: raylib is a programming library to enjoy videogames programming; no fancy interface, no visual helpers, no debug button... just coding in the most pure spartan-programmers way.*
<br>

*Lisha here. I love this GIF. My neurodivergent brain finds it extremely satisfying and hypnotizing :p*
<br><br><br>

## This Minimal, Slimmed, CMake-only fork
⚠️This repository is meant to be used only as a sub-module dependency for games. <br>
⚠️It is not intended to be built standalone and does not include examples. <br>
<br>
**What I removed:**
* IDE templates
* Alternate build systems
* examples/
* projects/
* tools/
* ..etc (I don't remember)
<br>

*Original repo:* [Raylib](https://github.com/raysan5/raylib)
<br>

## Features of Raylib
  - **NO external dependencies**, all required libraries are [included into raylib](https://github.com/raysan5/raylib/tree/master/src/external)
  - Multiple platforms supported: **Windows, Linux, MacOS, RPI, Android, HTML5... and more!**
  - Written in plain C code (C99) using PascalCase/camelCase notation
  - Hardware accelerated with OpenGL: **1.1, 2.1, 3.3, 4.3, ES 2.0, ES 3.0**
  - **Unique OpenGL abstraction layer** (usable as standalone module): [rlgl](https://github.com/raysan5/raylib/blob/master/src/rlgl.h)
  - Multiple **Fonts** formats supported (TTF, OTF, FNT, BDF, sprite fonts)
  - Multiple texture formats supported, including **compressed formats** (DXT, ETC, ASTC)
  - **Full 3D support**, including 3D Shapes, Models, Billboards, Heightmaps and more! 
  - Flexible Materials system, supporting classic maps and **PBR maps**
  - **Animated 3D models** supported (skeletal bones animation) (IQM, M3D, glTF)
  - Shaders support, including model shaders and **postprocessing** shaders
  - **Powerful math module** for Vector, Matrix and Quaternion operations: [raymath](https://github.com/raysan5/raylib/blob/master/src/raymath.h)
  - Audio loading and playing with streaming support (WAV, QOA, OGG, MP3, FLAC, XM, MOD)
  - **VR stereo rendering** support with configurable HMD device parameters
  - Huge examples collection with [+140 code examples](https://github.com/raysan5/raylib/tree/master/examples)!
  - Bindings to [+70 programming languages](https://github.com/raysan5/raylib/blob/master/BINDINGS.md)!
  - **Free and open source**

basic example
--------------
This is a basic raylib example, it creates a window and draws the text `"Congrats! You created your first window!"` in the middle of the screen. Check this example [running live on web here](https://www.raylib.com/examples/core/loader.html?name=core_basic_window).
```c
#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
```
## Usage (Submodule)
Why use a submodule?
* if you don't want external dependencies
* If installing raylib is slow/not happening/you just don't want to
* It's convenient, especially if you add the raylib headers to a precompiled header file <br>

It's completely subjective :)
<br>
1. Add this repository in your game's as a submodule: <br>
  `git submodule add https://github.com/lishacodesgames/slim-raylib.git raylib`
2. Add it in your project's CMakeLists.txt (optional):
  ```cmake
  add_subdirectory(raylib)
  
  # eg. if your main.c or main.cpp is in src/ and your executable's name is "YourProjectName"
  add_executable(YourProjectName src/main.cpp) 
  target_link_libraries(YourProjectName PRIVATE raylib)
  ```
## Learning Resources
* [Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
* [Official Wiki](https://github.com/raysan5/raylib/wiki)
* [Architecture overview](https://github.com/raysan5/raylib/wiki/raylib-architecture)
* [Official website](https://www.raylib.com/)

license
-------

raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

raylib uses internally some libraries for window/graphics/inputs management and also to support different file formats loading, all those libraries are embedded with and are available in [src/external](https://github.com/raysan5/raylib/tree/master/src/external) directory. Check [raylib dependencies LICENSES](https://github.com/raysan5/raylib/wiki/raylib-dependencies) on [raylib Wiki](https://github.com/raysan5/raylib/wiki) for details.
