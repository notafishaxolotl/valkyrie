# Set compiler variables BEFORE project()
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

cmake_minimum_required(VERSION 3.17)
project(Valkyrie CXX)

# --- BUILD CONFIGURATION ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# CRITICAL: Ensures compiler flags like -g are included for GDB and debugging
set(CMAKE_BUILD_TYPE Debug)

# --- DEPENDENCY DISCOVERY & CONFIGURATION ---

# 1. Vulkan and GLFW (These typically provide CMake config files and work with find_package)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# 2. GLM Header-Only Fix
# We use find_path instead of find_package because GLM does not provide a standard config file.
find_path(GLM_INCLUDE_DIR
    glm/glm.hpp
    PATHS /usr/include /usr/local/include
    ${NIX_PROFILES}/include # Prioritize the path provided by the Nix shell
    /nix/store/*/include    # Fallback to search Nix store paths
)

if (NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find GLM headers. Ensure 'glm' is in your shell.nix.")
endif()


# --- TARGET DEFINITION ---
add_executable(Valkyrie src/main.cpp) # Use Valkyrie as the executable name

# Add the found GLM include path to the compiler search paths
target_include_directories(Valkyrie PRIVATE ${GLM_INCLUDE_DIR})

# --- LINKING ---
# Link against the necessary libraries. 
# NOTE: GLM is NOT listed here because it is header-only and only needs the include path.
target_link_libraries(Valkyrie PRIVATE
    Vulkan::Vulkan # Vulkan core library
    glfw           # Windowing library
)

# --- SHADER COMPILATION INTEGRATION (Next Step to complete) ---
# NOTE: This block is currently missing, but required for SPIR-V generation.
# We will add it after this fix is verified.
