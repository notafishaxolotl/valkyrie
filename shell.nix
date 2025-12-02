{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "vulkan-dev-shell";

  # 1. Tools used to run the build (compilers, build systems, debuggers)
  nativeBuildInputs = with pkgs; [
    # Compilers (Focusing solely on Clang for this project)
    clang           # Primary compiler
    # gcc           # Fallback compiler - Removed per preference
    cmake
    pkg-config      # Essential for CMake to find library info
    shaderc         # For compiling GLSL to SPIR-V
    gdb             # The primary debugger tool
  ];

  # 2. Libraries linked into the final executable (The actual dependencies)
  buildInputs = with pkgs; [
    vulkan-headers
    vulkan-loader
    vulkan-validation-layers
    vulkan-tools      
    glslang
    glfw              # Windowing library (Wayland compatible)
    glm               # Mathematics library
    
    # --- Wayland Specific Dependencies (Optimized for Wayland) ---
    wayland           # Core Wayland client protocol libraries
    xorg.libXcursor   # Still sometimes needed for mouse cursors in cross-platform tools like GLFW
  ];

  # NixOS quirk: Environment setup for the Vulkan Loader
  shellHook = ''
    export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${pkgs.vulkan-loader}/lib"
    
    echo "Vulkan development shell loaded. Target: Wayland/Linux with Clang."
  '';
}
