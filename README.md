# Raylib Modular Template
Modular game template using raylib as a git submodule and CMake. <br>
This repository is intended to be used as a starting point for a new raylib project.

This library-based system is for those who want a distinct separation between the core app, external libraries, and gui code. <br>
This can be used as-is (like I do) or any subdirectories can be added or removed as needed

*I also have a simpler, non-lib-based, template: ![check it out](https://github.com/lishacodesgames/Raylib-Template)*

## How to Clone
`git clone --recursive` to get the raylib submodule local
<br>

**If you forgot, do:** <br>
`git submodule update --init --recursive` after regular `git clone`

## Project Structure
What your project architecture should ideally look like for the CMakeLists.txt I've included (Can change accordingly, otherwise)
```
‖
‖——.git*
‖——CMakeLists.txt
‖——CMakePresets.json
‖——App/
‖  ‖——CMakeLists.txt
‖  ‖——include/
‖  ‖  ‖——*.h or *.hpp → all *your* header files
‖  ‖——src/
‖  ‖  ‖——main.cpp
‖  ‖  ‖——*.c or *.cpp → all *your* source files
‖——lib/
‖  ‖——raylib/   → raylib submodule
‖  ‖——GUI/      → raylib GUI stuff that's not native to your app
‖  ‖——Layers/   → Layering system similar to the Hazel Game engine (wayyy simpler, but a good starting point for transitions)
```

Note: if you are using C, the repository might show C++ because I've excluded C in .gitattributes. <br>
Just remove this from `.gitattrbites`

```
**/*.h linguist-vendored
**/*.c linguist-vendored
```

Now your repository will show C

## Build
`mkdir build`
`cd build` <br>
`cmake --preset Debug` OR `cmake --preset Release` -- (configuration of cmake) <br>
`cmake --build --preset Debug` OR `cmake --build --preset Release` -- (compilation + build of your files, only what has changed since last build)
<br>

*Check out ![CMakePresets.json](CMakePresets.json) if you're confused* <br>

Executable will appear in `build/` under preset name
<br>

### Linux Dependencies
Raylib requires a lot of libraries that are built-in on Mac and Windows. <br>
No worries, Linux just requires 1 command to install them all:
```bash
sudo apt install \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxi-dev \
    libxinerama-dev \
    libgl1-mesa-dev \
    libasound2-dev \
    libpulse-dev \
    libxkbcommon-dev
```

## Add New Source Files
Put any new `.cpp` or `.c` files in `src/` <br>
Update App/CMakeLists.txt as needed, example:
```cmake
add_executable(YourProjectName
  src/main.cpp
  src/other_file.cpp
)
```

**Note:** I've made it so that you don't need to touch any CMakeLists.txt other than App/ as you will add your source files there. <br>
If any issues arise, lmk :)

## Updating raylib (the submodule)
```bash
cd lib/raylib
git pull
cd ../..
git add lib/raylib
git commit -m "Updated raylib submodule"
```

## Requirements
* CMake 3.20+
* C++ compiler: GCC / Clang
* Ninja (not *required*, but it's what the presets use)
