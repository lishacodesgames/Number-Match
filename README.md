# Number Match
![Latest Tag](https://img.shields.io/github/v/tag/lishacodesgames/Number-Match?color=%237DBA84)
![Status Badge](https://img.shields.io/badge/Status-In_Development-yellow)

### Overview
The origin Number match is a fun mobile game where you match up pairs of numbers that are the same or that add up to 10. Clearing rows and levels, and winning points and money. There's also tournaments you can participate in, though the game is entirely singleplayer.

My version is desktop-friendly, has no ads, and is just as addicting.

### Tech Stack
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) 
![RayLib](https://img.shields.io/badge/RAYLIB-FFFFFF?style=for-the-badge&logo=raylib&logoColor=black)

## How to Clone
`git clone --recursive` to get the raylib submodule local
<br>

**If you forgot, do:** <br>
`git submodule update --init --recursive` after regular `git clone`

## Build
```bash
cmake --preset Release
cmake --build --preset Release
```
Executable will appear in `build/` under preset name

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

## Requirements
* CMake 3.20+
* C++ compiler: GCC 13.3 / Clang
* Ninja (not *required*, but it's what the presets use)
