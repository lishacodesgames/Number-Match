// THESE ARE ALL THE LARGE HEADERS THAT TAKE UP COMPILE TIME
// THAT DON'T NEED TO AS THEY CAN BE READY PRECOMPILED.
// THIS INCREASES COMPILE SPEED A LOT
// ONLY ADD HEADERS THAT YOU'RE SURE YOU'RE NOT GOING TO CHANGE

// CMAKE DOES THE PRECOMPILATION FOR US

// NEVER include Precompiled headers in header files. 
// Only in cpp, at the VERY TOP of everything else.

// ---- STL ----
#include <vector>
#include <array>
#include <unordered_map>

// ---- STD INT ----
#include <cstdint>

// ---- LOGGING ----
#include <iostream>
#include <cstdio>

// ---- RAYLIB ----
#include <raylib.h>

// ---- OTHER ----
#include <algorithm>