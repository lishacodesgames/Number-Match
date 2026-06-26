#pragma once
// ============================================================================
// 1. STANDARD TEMPLATE LIBRARY (STL)
// ============================================================================

// Core Types & Utilities
#include <typeinfo>
#include <stdarg.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <tuple>

// Functional & Logic
#include <functional>
#include <algorithm>
#include <optional>

// Data Containers
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

// Streams & I/O
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <format>
#include <cstdio>

// System, Time & Concurrency
#include <filesystem>
#include <stdexcept>
#include <chrono>
#include <random>
#include <thread>
#include <ctime>

// ============================================================================
// 2. THIRD-PARTY / EXTERNAL LIBRARIES
// ============================================================================

#include <raylib.h>
#include <raymath.h>

#include <nlohmann/json.hpp>

// ============================================================================
// 3. INTERNAL ENGINE CORE
// ============================================================================

#include "Core/Logging.h" 

#include "Core/Event.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"