# Number Match
![Latest Tag](https://img.shields.io/github/v/tag/lishacodesgames/Number-Match?color=%237DBA84)
![Status Badge](https://img.shields.io/badge/Status-In_Development-yellow)

## Overview
The original Number match is a fun mobile game where you match up pairs of numbers that are the same or that add up to 10. Clearing rows and levels, and winning points and coins. There's also tournaments you can participate in, though the game is entirely singleplayer.

My version is desktop-friendly, has no ads, and is just as addicting.

### Tech Stack
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) 
![RayLib](https://img.shields.io/badge/RAYLIB-FFFFFF?style=for-the-badge&logo=raylib&logoColor=black)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

## How to Play
The main game relies on mouse clicks only. <br>
The only keybinding is: `Q` to go back to the home page from the game, and `Esc` to exit the Application

**Main Objective:** Match numbers that either sum to 10, or are the same.
- Matches happen on the same row, column, or diagonal; as long as there are no unmatched cells in between.
- Once all cells in a row have been matched, that row is cleared.
- Once all rows in the grid have been cleared, you move onto the next stage.
- If there are no more matches available, click the plus at the bottom of the grid.
- If you need help, click the hint icon at the bottom of the grid.
- For more settings, or a tutorial, click the gear icon at the top right of the grid.

There is no end to the game, there can be as many stages as you like. 

### Some Additional Features
- There are "tournament" games which feature an end goal of a certain amount of points.
- Various Achievements to work towards.

## How to use

### Clone & Build
```bash
git clone --recursive --shallow-submodules https://github.com/lishacodesgames/Number-Match.git
cd Number-Match
cmake --preset Release
cmake --build --preset Release
```

**NOTE: If you forgot the recursive flag in git clone, run:** <br>
`git submodule update --init --recursive --depth 1` after regular `git clone`, and then the cmake command.

**If you have the repository cloned, but want to update, run:** <br>
`git fetch && git pull` instead of clone, and then the cmake command.

### Run the Game
**Windows**
```bash
bin\Release\NumberMatch.exe
```

**Mac/Linux**
```bash
./bin/Release/NumberMatch
```

**Note:** <br> 
- Make sure you're running from the repo directory `Number-Match/`
- Make sure you have cmake and ninja in your path variables

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
* A C++ Compiler
* CMake 3.20+
* Ninja (not *required*, but it's what the presets use)
