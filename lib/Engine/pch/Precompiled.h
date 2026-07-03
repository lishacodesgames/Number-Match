#pragma once
// ===================================
// 1. STANDARD TEMPLATE LIBRARY (STL)
// ===================================

// Core Types & Utilities
#include <typeinfo>
#include <stdarg.h>
#include <cstdint>
#include <utility>
#include <memory>
#include <tuple>

// Functional & Logic
#include <functional>
#include <algorithm>
#include <optional>

// Data Containers
#include <unordered_map>
#include <string_view>
#include <string>
#include <vector>
#include <array>

// Streams & I/O
#include <fstream>
#include <sstream>
#include <format>
#include <cstdio>

// raylib uses printf which must not mix with std::cout
// #include <iostream> 
// #include <iomanip>

// System, Time & Concurrency
#include <source_location>
#include <filesystem>
#include <stdexcept>
#include <chrono>
#include <random>
#include <thread>
#include <ctime>

// ====================================
// 2. THIRD-PARTY / EXTERNAL LIBRARIES
// ====================================

#include <raylib.h>
#include <raymath.h>

#include <nlohmann/json.hpp>

// ========================
// 3. INTERNAL ENGINE CORE
// ========================

#include "Core/Logging.h" 

#include "Core/Event.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"