#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// GLM includes for math, though not strictly used in this step
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

class HelloTriangleApplication {
public:
    // 1. The main entry point for the application logic
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    // **Member Variables (The State)**
    GLFWwindow* window = nullptr;
    VkInstance instance = VK_NULL_HANDLE; // Handle for the Vulkan instance

    // 2. Initialization of the window
    void initWindow() {
        // Initialize GLFW
        if (!glfwInit()) {
            throw std::runtime_error("failed to initialize GLFW!");
        }

        // Tell GLFW not to create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // We will keep the window fixed size for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
        
        // Create the window
        window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
        
        if (window == nullptr) {
            glfwTerminate(); // Clean up GLFW if window creation fails
            throw std::runtime_error("failed to create GLFW window!");
        }
    }

    // Helper function to create the VkInstance
    void createInstance() {
        // 1. Define application metadata (optional but good practice)
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // 2. Define instance creation parameters
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // 3. Get required GLFW extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        // GLFW tells Vulkan which extensions it needs to draw on the Wayland/X11/Win32 window
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::cout << "Required GLFW Extensions (" << glfwExtensionCount << "):" << std::endl;
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            std::cout << "\t" << glfwExtensions[i] << std::endl;
        }

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        
        // We will enable validation layers later, but for now we set them to 0
        createInfo.enabledLayerCount = 0;

        // 4. Create the instance and check for failure
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vulkan instance!");
        }
        std::cout << "Vulkan instance successfully created." << std::endl;
    }

    // 3. Initialization of Vulkan (calls the instance creation)
    void initVulkan() {
        createInstance();
    }

    // 4. The main event loop
    void mainLoop() {
        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    // 5. Cleanup (reverse order of creation)
    void cleanup() {
        // Destroy the Vulkan instance first (if it was created successfully)
        if (instance != VK_NULL_HANDLE) {
            vkDestroyInstance(instance, nullptr);
        }

        // Destroy the GLFW window and terminate GLFW
        if (window != nullptr) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
        std::cout << "Cleanup complete." << std::endl;
    }
};

// **The Program Entry Point**
int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        // Print the error message
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
