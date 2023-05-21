
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#define GLAD_VULKAN_IMPLEMENTATION
#include <vulkan.h>

// Include graphics because we use sf::Image for loading images
#include <SFML/Graphics.hpp>

#include <SFML/Window.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <vector>

#include <cmath>
#include <cstring>


////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////
namespace
{
using Vec3   = float[3];
using Matrix = float[4][4];

// Multiply 2 matrices
void matrixMultiply(Matrix& result, const Matrix& left, const Matrix& right)
{
    Matrix temp;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            temp[i][j] = left[0][j] * right[i][0] + left[1][j] * right[i][1] + left[2][j] * right[i][2] +
                         left[3][j] * right[i][3];
    }

    std::memcpy(result, temp, sizeof(Matrix));
}

// Rotate a matrix around the x-axis
void matrixRotateX(Matrix& result, sf::Angle angle)
{
    const float rad = angle.asRadians();

    // clang-format off
    const Matrix matrix = {
        {1.f,   0.f,           0.f,           0.f},
        {0.f,   std::cos(rad), std::sin(rad), 0.f},
        {0.f,  -std::sin(rad), std::cos(rad), 0.f},
        {0.f,   0.f,           0.f,           1.f}
    };
    // clang-format on

    matrixMultiply(result, result, matrix);
}

// Rotate a matrix around the y-axis
void matrixRotateY(Matrix& result, sf::Angle angle)
{
    const float rad = angle.asRadians();

    // clang-format off
    const Matrix matrix = {
        { std::cos(rad), 0.f, std::sin(rad), 0.f},
        { 0.f,           1.f, 0.f,           0.f},
        {-std::sin(rad), 0.f, std::cos(rad), 0.f},
        { 0.f,           0.f, 0.f,           1.f}
    };
    // clang-format on

    matrixMultiply(result, result, matrix);
}

// Rotate a matrix around the z-axis
void matrixRotateZ(Matrix& result, sf::Angle angle)
{
    const float rad = angle.asRadians();

    // clang-format off
    const Matrix matrix = {
        { std::cos(rad), std::sin(rad), 0.f, 0.f},
        {-std::sin(rad), std::cos(rad), 0.f, 0.f},
        { 0.f,           0.f,           1.f, 0.f},
        { 0.f,           0.f,           0.f, 1.f}
    };
    // clang-format on

    matrixMultiply(result, result, matrix);
}

// Construct a lookat view matrix
void matrixLookAt(Matrix& result, const Vec3& eye, const Vec3& center, const Vec3& up)
{
    // Forward-looking vector
    // clang-format off
    Vec3 forward = {
        center[0] - eye[0],
        center[1] - eye[1],
        center[2] - eye[2]
    };
    // clang-format on

    // Normalize
    float factor = 1.0f / std::sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);

    for (float& f : forward)
        f *= factor;

    // Side vector (Forward cross product Up)
    // clang-format off
    Vec3 side = {
        forward[1] * up[2] - forward[2] * up[1],
        forward[2] * up[0] - forward[0] * up[2],
        forward[0] * up[1] - forward[1] * up[0]
    };
    // clang-format on

    // Normalize
    factor = 1.0f / std::sqrt(side[0] * side[0] + side[1] * side[1] + side[2] * side[2]);

    for (float& f : side)
        f *= factor;

    result[0][0] = side[0];
    result[0][1] = side[1] * forward[2] - side[2] * forward[1];
    result[0][2] = -forward[0];
    result[0][3] = 0.f;

    result[1][0] = side[1];
    result[1][1] = side[2] * forward[0] - side[0] * forward[2];
    result[1][2] = -forward[1];
    result[1][3] = 0.f;

    result[2][0] = side[2];
    result[2][1] = side[0] * forward[1] - side[1] * forward[0];
    result[2][2] = -forward[2];
    result[2][3] = 0.f;

    result[3][0] = (-eye[0]) * result[0][0] + (-eye[1]) * result[1][0] + (-eye[2]) * result[2][0];
    result[3][1] = (-eye[0]) * result[0][1] + (-eye[1]) * result[1][1] + (-eye[2]) * result[2][1];
    result[3][2] = (-eye[0]) * result[0][2] + (-eye[1]) * result[1][2] + (-eye[2]) * result[2][2];
    result[3][3] = (-eye[0]) * result[0][3] + (-eye[1]) * result[1][3] + (-eye[2]) * result[2][3] + 1.0f;
}

// Construct a perspective projection matrix
void matrixPerspective(Matrix& result, sf::Angle fov, float aspect, float nearPlane, float farPlane)
{
    const float a = 1.f / std::tan(fov.asRadians() / 2.f);

    result[0][0] = a / aspect;
    result[0][1] = 0.f;
    result[0][2] = 0.f;
    result[0][3] = 0.f;

    result[1][0] = 0.f;
    result[1][1] = -a;
    result[1][2] = 0.f;
    result[1][3] = 0.f;

    result[2][0] = 0.f;
    result[2][1] = 0.f;
    result[2][2] = -((farPlane + nearPlane) / (farPlane - nearPlane));
    result[2][3] = -1.f;

    result[3][0] = 0.f;
    result[3][1] = 0.f;
    result[3][2] = -((2.f * farPlane * nearPlane) / (farPlane - nearPlane));
    result[3][3] = 0.f;
}

// Helper function we pass to GLAD to load Vulkan functions via SFML
GLADapiproc getVulkanFunction(const char* name)
{
    return sf::Vulkan::getFunction(name);
}

// Debug we pass to Vulkan to call when it detects warnings or errors
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugReportFlagsEXT,
    VkDebugReportObjectTypeEXT,
    std::uint64_t,
    std::size_t,
    std::int32_t,
    const char*,
    const char* pMessage,
    void*)
{
    std::cerr << pMessage << std::endl;

    return VK_FALSE;
}
} // namespace


////////////////////////////////////////////////////////////
// VulkanExample class
////////////////////////////////////////////////////////////
class VulkanExample
{
public:
    // Constructor
    VulkanExample()
    {
        // Vulkan setup procedure
        if (vulkanAvailable)
            setupInstance();
        if (vulkanAvailable)
            setupDebugReportCallback();
        if (vulkanAvailable)
            setupSurface();
        if (vulkanAvailable)
            setupPhysicalDevice();
        if (vulkanAvailable)
            setupLogicalDevice();
        if (vulkanAvailable)
            setupSwapchain();
        if (vulkanAvailable)
            setupSwapchainImages();
        if (vulkanAvailable)
            setupShaders();
        if (vulkanAvailable)
            setupRenderpass();
        if (vulkanAvailable)
            setupDescriptorSetLayout();
        if (vulkanAvailable)
            setupPipelineLayout();
        if (vulkanAvailable)
            setupPipeline();
        if (vulkanAvailable)
            setupCommandPool();
        if (vulkanAvailable)
            setupVertexBuffer();
        if (vulkanAvailable)
            setupIndexBuffer();
        if (vulkanAvailable)
            setupUniformBuffers();
        if (vulkanAvailable)
            setupDepthImage();
        if (vulkanAvailable)
            setupDepthImageView();
        if (vulkanAvailable)
            setupTextureImage();
        if (vulkanAvailable)
            setupTextureImageView();
        if (vulkanAvailable)
            setupTextureSampler();
        if (vulkanAvailable)
            setupFramebuffers();
        if (vulkanAvailable)
            setupDescriptorPool();
        if (vulkanAvailable)
            setupDescriptorSets();
        if (vulkanAvailable)
            setupCommandBuffers();
        if (vulkanAvailable)
            setupDraw();
        if (vulkanAvailable)
            setupSemaphores();
        if (vulkanAvailable)
            setupFences();

        // If something went wrong, notify the user by setting the window title
        if (!vulkanAvailable)
            window.setTitle("SFML window with Vulkan (Vulkan not available)");
    }


    // Destructor
    ~VulkanExample()
    {
        // Wait until there are no pending frames
        if (device)
            vkDeviceWaitIdle(device);

        // Teardown swapchain
        cleanupSwapchain();

        // Vulkan teardown procedure
        for (VkFence fence : fences)
            vkDestroyFence(device, fence, nullptr);

        for (VkSemaphore renderFinishedSemaphore : renderFinishedSemaphores)
            vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);

        for (VkSemaphore imageAvailableSemaphore : imageAvailableSemaphores)
            vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

        if (descriptorPool)
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        for (VkDeviceMemory i : uniformBuffersMemory)
            vkFreeMemory(device, i, nullptr);

        for (VkBuffer uniformBuffer : uniformBuffers)
            vkDestroyBuffer(device, uniformBuffer, nullptr);

        if (textureSampler)
            vkDestroySampler(device, textureSampler, nullptr);

        if (textureImageView)
            vkDestroyImageView(device, textureImageView, nullptr);

        if (textureImageMemory)
            vkFreeMemory(device, textureImageMemory, nullptr);

        if (textureImage)
            vkDestroyImage(device, textureImage, nullptr);

        if (indexBufferMemory)
            vkFreeMemory(device, indexBufferMemory, nullptr);

        if (indexBuffer)
            vkDestroyBuffer(device, indexBuffer, nullptr);

        if (vertexBufferMemory)
            vkFreeMemory(device, vertexBufferMemory, nullptr);

        if (vertexBuffer)
            vkDestroyBuffer(device, vertexBuffer, nullptr);

        if (commandPool)
            vkDestroyCommandPool(device, commandPool, nullptr);

        if (descriptorSetLayout)
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        if (fragmentShaderModule)
            vkDestroyShaderModule(device, fragmentShaderModule, nullptr);

        if (vertexShaderModule)
            vkDestroyShaderModule(device, vertexShaderModule, nullptr);

        if (device)
            vkDestroyDevice(device, nullptr);

        if (surface)
            vkDestroySurfaceKHR(instance, surface, nullptr);

        if (debugReportCallback)
            vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);

        if (instance)
            vkDestroyInstance(instance, nullptr);
    }

    // Cleanup swapchain
    void cleanupSwapchain()
    {
        // Swapchain teardown procedure
        for (VkFence fence : fences)
            vkWaitForFences(device, 1, &fence, VK_TRUE, std::numeric_limits<std::uint64_t>::max());

        if (!commandBuffers.empty())
            vkFreeCommandBuffers(device, commandPool, static_cast<std::uint32_t>(commandBuffers.size()), commandBuffers.data());

        commandBuffers.clear();

        for (VkFramebuffer swapchainFramebuffer : swapchainFramebuffers)
            vkDestroyFramebuffer(device, swapchainFramebuffer, nullptr);

        swapchainFramebuffers.clear();

        if (graphicsPipeline)
            vkDestroyPipeline(device, graphicsPipeline, nullptr);

        if (renderPass)
            vkDestroyRenderPass(device, renderPass, nullptr);

        if (pipelineLayout)
            vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

        if (depthImageView)
            vkDestroyImageView(device, depthImageView, nullptr);

        if (depthImageMemory)
            vkFreeMemory(device, depthImageMemory, nullptr);

        if (depthImage)
            vkDestroyImage(device, depthImage, nullptr);

        for (VkImageView swapchainImageView : swapchainImageViews)
            vkDestroyImageView(device, swapchainImageView, nullptr);

        swapchainImageViews.clear();

        if (swapchain)
            vkDestroySwapchainKHR(device, swapchain, nullptr);
    }

    // Cleanup and recreate swapchain
    void recreateSwapchain()
    {
        // Wait until there are no pending frames
        vkDeviceWaitIdle(device);

        // Cleanup swapchain
        cleanupSwapchain();

        // Swapchain setup procedure
        if (vulkanAvailable)
            setupSwapchain();
        if (vulkanAvailable)
            setupSwapchainImages();
        if (vulkanAvailable)
            setupPipelineLayout();
        if (vulkanAvailable)
            setupRenderpass();
        if (vulkanAvailable)
            setupPipeline();
        if (vulkanAvailable)
            setupDepthImage();
        if (vulkanAvailable)
            setupDepthImageView();
        if (vulkanAvailable)
            setupFramebuffers();
        if (vulkanAvailable)
            setupCommandBuffers();
        if (vulkanAvailable)
            setupDraw();
    }

    // Setup Vulkan instance
    void setupInstance()
    {
        // Load bootstrap entry points
        gladLoadVulkan({}, getVulkanFunction);

        if (!vkCreateInstance)
        {
            vulkanAvailable = false;
            return;
        }

        // Retrieve the available instance layers
        std::uint32_t objectCount = 0;

        std::vector<VkLayerProperties> layers;

        if (vkEnumerateInstanceLayerProperties(&objectCount, nullptr) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        layers.resize(objectCount);

        if (vkEnumerateInstanceLayerProperties(&objectCount, layers.data()) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Activate the layers we are interested in
        std::vector<const char*> validationLayers;

        for (VkLayerProperties& layer : layers)
        {
            // VK_LAYER_LUNARG_standard_validation, meta-layer for the following layers:
            // -- VK_LAYER_GOOGLE_threading
            // -- VK_LAYER_LUNARG_parameter_validation
            // -- VK_LAYER_LUNARG_device_limits
            // -- VK_LAYER_LUNARG_object_tracker
            // -- VK_LAYER_LUNARG_image
            // -- VK_LAYER_LUNARG_core_validation
            // -- VK_LAYER_LUNARG_swapchain
            // -- VK_LAYER_GOOGLE_unique_objects
            // These layers perform error checking and warn about bad or sub-optimal Vulkan API usage
            // VK_LAYER_LUNARG_monitor appends an FPS counter to the window title
            if (std::string_view(layer.layerName) == "VK_LAYER_LUNARG_standard_validation")
            {
                validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
            }
            else if (std::string_view(layer.layerName) == "VK_LAYER_LUNARG_monitor")
            {
                validationLayers.push_back("VK_LAYER_LUNARG_monitor");
            }
        }

        // Retrieve the extensions we need to enable in order to use Vulkan with SFML
        std::vector<const char*> requiredExtensions = sf::Vulkan::getGraphicsRequiredInstanceExtensions();
        requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

        // Register our application information
        VkApplicationInfo applicationInfo  = VkApplicationInfo();
        applicationInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName   = "SFML Vulkan Test";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName        = "SFML Vulkan Test Engine";
        applicationInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.apiVersion         = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo    = VkInstanceCreateInfo();
        instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo        = &applicationInfo;
        instanceCreateInfo.enabledLayerCount       = static_cast<std::uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames     = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount   = static_cast<std::uint32_t>(requiredExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // Try to create a Vulkan instance with debug report enabled
        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

        // If an extension is missing, try disabling debug report
        if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            requiredExtensions.pop_back();

            instanceCreateInfo.enabledExtensionCount   = static_cast<std::uint32_t>(requiredExtensions.size());
            instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

            result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
        }

        // If instance creation still fails, give up
        if (result != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Load instance entry points
        gladLoadVulkan({}, getVulkanFunction);
    }

    // Setup our debug callback function to be called by Vulkan
    void setupDebugReportCallback()
    {
        // Don't try to register the callback if the extension is not available
        if (!vkCreateDebugReportCallbackEXT)
            return;

        // Register for warnings and errors
        VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = VkDebugReportCallbackCreateInfoEXT();
        debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                              VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
        debugReportCallbackCreateInfo.pfnCallback = debugCallback;

        // Create the debug callback
        if (vkCreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfo, nullptr, &debugReportCallback) !=
            VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Setup the SFML window Vulkan rendering surface
    void setupSurface()
    {
        if (!window.createVulkanSurface(instance, surface))
            vulkanAvailable = false;
    }

    // Select a GPU to use and query its capabilities
    void setupPhysicalDevice()
    {
        // Last sanity check
        if (!vkEnumeratePhysicalDevices || !vkCreateDevice || !vkGetPhysicalDeviceProperties)
        {
            vulkanAvailable = false;
            return;
        }

        // Retrieve list of GPUs
        std::uint32_t objectCount = 0;

        std::vector<VkPhysicalDevice> devices;

        if (vkEnumeratePhysicalDevices(instance, &objectCount, nullptr) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        devices.resize(objectCount);

        if (vkEnumeratePhysicalDevices(instance, &objectCount, devices.data()) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Look for a GPU that supports swapchains
        for (VkPhysicalDevice dev : devices)
        {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(dev, &deviceProperties);

            std::vector<VkExtensionProperties> extensions;

            if (vkEnumerateDeviceExtensionProperties(dev, nullptr, &objectCount, nullptr) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }

            extensions.resize(objectCount);

            if (vkEnumerateDeviceExtensionProperties(dev, nullptr, &objectCount, extensions.data()) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }

            bool supportsSwapchain = false;

            for (VkExtensionProperties& extension : extensions)
            {
                if (std::string_view(extension.extensionName) == VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                {
                    supportsSwapchain = true;
                    break;
                }
            }

            if (!supportsSwapchain)
                continue;

            // Prefer discrete over integrated GPUs if multiple are available
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                gpu = dev;
                break;
            }
            else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                gpu = dev;
            }
        }

        if (!gpu)
        {
            vulkanAvailable = false;
            return;
        }

        // Load physical device entry points
        gladLoadVulkan(gpu, getVulkanFunction);

        // Check what depth formats are available and select one
        VkFormatProperties formatProperties = VkFormatProperties();

        vkGetPhysicalDeviceFormatProperties(gpu, VK_FORMAT_D24_UNORM_S8_UINT, &formatProperties);

        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            depthFormat = VK_FORMAT_D24_UNORM_S8_UINT;
        }
        else
        {
            vkGetPhysicalDeviceFormatProperties(gpu, VK_FORMAT_D32_SFLOAT_S8_UINT, &formatProperties);

            if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                depthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
            }
            else
            {
                vkGetPhysicalDeviceFormatProperties(gpu, VK_FORMAT_D32_SFLOAT, &formatProperties);

                if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
                {
                    depthFormat = VK_FORMAT_D32_SFLOAT;
                }
                else
                {
                    vulkanAvailable = false;
                    return;
                }
            }
        }
    }

    // Setup logical device and device queue
    void setupLogicalDevice()
    {
        // Select a queue family that supports graphics operations and surface presentation
        std::uint32_t objectCount = 0;

        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &objectCount, nullptr);

        queueFamilyProperties.resize(objectCount);

        vkGetPhysicalDeviceQueueFamilyProperties(gpu, &objectCount, queueFamilyProperties.data());

        for (std::size_t i = 0; i < queueFamilyProperties.size(); ++i)
        {
            VkBool32 surfaceSupported = VK_FALSE;

            vkGetPhysicalDeviceSurfaceSupportKHR(gpu, static_cast<std::uint32_t>(i), surface, &surfaceSupported);

            if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (surfaceSupported == VK_TRUE))
            {
                queueFamilyIndex = static_cast<int>(i);
                break;
            }
        }

        if (queueFamilyIndex < 0)
        {
            vulkanAvailable = false;
            return;
        }

        const float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo deviceQueueCreateInfo = VkDeviceQueueCreateInfo();
        deviceQueueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueCount              = 1;
        deviceQueueCreateInfo.queueFamilyIndex        = static_cast<std::uint32_t>(queueFamilyIndex);
        deviceQueueCreateInfo.pQueuePriorities        = &queuePriority;

        // Enable the swapchain extension
        const char* extensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // Enable anisotropic filtering
        VkPhysicalDeviceFeatures physicalDeviceFeatures = VkPhysicalDeviceFeatures();
        physicalDeviceFeatures.samplerAnisotropy        = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo      = VkDeviceCreateInfo();
        deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.enabledExtensionCount   = 1;
        deviceCreateInfo.ppEnabledExtensionNames = extensions;
        deviceCreateInfo.queueCreateInfoCount    = 1;
        deviceCreateInfo.pQueueCreateInfos       = &deviceQueueCreateInfo;
        deviceCreateInfo.pEnabledFeatures        = &physicalDeviceFeatures;

        // Create our logical device
        if (vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Retrieve a handle to the logical device command queue
        vkGetDeviceQueue(device, static_cast<std::uint32_t>(queueFamilyIndex), 0, &queue);
    }

    // Query surface formats and set up swapchain
    void setupSwapchain()
    {
        // Select a surface format that supports RGBA color format
        std::uint32_t objectCount = 0;

        std::vector<VkSurfaceFormatKHR> surfaceFormats;

        if (vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &objectCount, nullptr) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        surfaceFormats.resize(objectCount);

        if (vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &objectCount, surfaceFormats.data()) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        if ((surfaceFormats.size() == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
        {
            swapchainFormat.format     = VK_FORMAT_B8G8R8A8_UNORM;
            swapchainFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
        else if (!surfaceFormats.empty())
        {
            for (const VkSurfaceFormatKHR& surfaceFormat : surfaceFormats)
            {
                if ((surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) &&
                    (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
                {
                    swapchainFormat.format     = VK_FORMAT_B8G8R8A8_UNORM;
                    swapchainFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

                    break;
                }
            }

            if (swapchainFormat.format == VK_FORMAT_UNDEFINED)
                swapchainFormat = surfaceFormats[0];
        }
        else
        {
            vulkanAvailable = false;
            return;
        }

        // Select a swapchain present mode
        std::vector<VkPresentModeKHR> presentModes;

        if (vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &objectCount, nullptr) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        presentModes.resize(objectCount);

        if (vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &objectCount, presentModes.data()) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Prefer mailbox over FIFO if it is available
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const VkPresentModeKHR& i : presentModes)
        {
            if (i == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = i;
                break;
            }
        }

        // Determine size and count of swapchain images
        VkSurfaceCapabilitiesKHR surfaceCapabilities;

        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surfaceCapabilities) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        swapchainExtent.width  = std::clamp(window.getSize().x,
                                           surfaceCapabilities.minImageExtent.width,
                                           surfaceCapabilities.maxImageExtent.width);
        swapchainExtent.height = std::clamp(window.getSize().y,
                                            surfaceCapabilities.minImageExtent.height,
                                            surfaceCapabilities.maxImageExtent.height);

        const auto imageCount = std::clamp(2u, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);

        VkSwapchainCreateInfoKHR swapchainCreateInfo = VkSwapchainCreateInfoKHR();
        swapchainCreateInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface                  = surface;
        swapchainCreateInfo.minImageCount            = imageCount;
        swapchainCreateInfo.imageFormat              = swapchainFormat.format;
        swapchainCreateInfo.imageColorSpace          = swapchainFormat.colorSpace;
        swapchainCreateInfo.imageExtent              = swapchainExtent;
        swapchainCreateInfo.imageArrayLayers         = 1;
        swapchainCreateInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.preTransform             = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode              = presentMode;
        swapchainCreateInfo.clipped                  = VK_TRUE;
        swapchainCreateInfo.oldSwapchain             = VK_NULL_HANDLE;

        // Create the swapchain
        if (vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Retrieve the swapchain images and create image views for them
    void setupSwapchainImages()
    {
        // Retrieve swapchain images
        std::uint32_t objectCount = 0;

        if (vkGetSwapchainImagesKHR(device, swapchain, &objectCount, nullptr) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        swapchainImages.resize(objectCount);
        swapchainImageViews.resize(objectCount);

        if (vkGetSwapchainImagesKHR(device, swapchain, &objectCount, swapchainImages.data()) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        VkImageViewCreateInfo imageViewCreateInfo           = VkImageViewCreateInfo();
        imageViewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                          = swapchainFormat.format;
        imageViewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;

        // Create an image view for each swapchain image
        for (std::size_t i = 0; i < swapchainImages.size(); ++i)
        {
            imageViewCreateInfo.image = swapchainImages[i];

            if (vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Load vertex and fragment shader modules
    void setupShaders()
    {
        VkShaderModuleCreateInfo shaderModuleCreateInfo = VkShaderModuleCreateInfo();
        shaderModuleCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        // Use the vertex shader SPIR-V code to create a vertex shader module
        {
            sf::FileInputStream file;

            if (!file.open("resources/shader.vert.spv"))
            {
                vulkanAvailable = false;
                return;
            }

            std::vector<std::uint32_t> buffer(static_cast<std::size_t>(file.getSize()) / sizeof(std::uint32_t));

            if (file.read(buffer.data(), file.getSize()) != file.getSize())
            {
                vulkanAvailable = false;
                return;
            }

            shaderModuleCreateInfo.codeSize = buffer.size() * sizeof(std::uint32_t);
            shaderModuleCreateInfo.pCode    = buffer.data();

            if (vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &vertexShaderModule) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }

        // Use the fragment shader SPIR-V code to create a fragment shader module
        {
            sf::FileInputStream file;

            if (!file.open("resources/shader.frag.spv"))
            {
                vulkanAvailable = false;
                return;
            }

            std::vector<std::uint32_t> buffer(static_cast<std::size_t>(file.getSize()) / sizeof(std::uint32_t));

            if (file.read(buffer.data(), file.getSize()) != file.getSize())
            {
                vulkanAvailable = false;
                return;
            }

            shaderModuleCreateInfo.codeSize = buffer.size() * sizeof(std::uint32_t);
            shaderModuleCreateInfo.pCode    = buffer.data();

            if (vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &fragmentShaderModule) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }

        // Prepare the shader stage information for later pipeline creation
        shaderStages[0]        = VkPipelineShaderStageCreateInfo();
        shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertexShaderModule;
        shaderStages[0].pName  = "main";

        shaderStages[1]        = VkPipelineShaderStageCreateInfo();
        shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragmentShaderModule;
        shaderStages[1].pName  = "main";
    }

    // Setup renderpass and its subpass dependencies
    void setupRenderpass()
    {
        VkAttachmentDescription attachmentDescriptions[2];

        // Color attachment
        attachmentDescriptions[0]                = VkAttachmentDescription();
        attachmentDescriptions[0].format         = swapchainFormat.format;
        attachmentDescriptions[0].samples        = VK_SAMPLE_COUNT_1_BIT;
        attachmentDescriptions[0].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDescriptions[0].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentDescriptions[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDescriptions[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDescriptions[0].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // Depth attachment
        attachmentDescriptions[1]                = VkAttachmentDescription();
        attachmentDescriptions[1].format         = depthFormat;
        attachmentDescriptions[1].samples        = VK_SAMPLE_COUNT_1_BIT;
        attachmentDescriptions[1].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDescriptions[1].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDescriptions[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDescriptions[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDescriptions[1].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentReference = {};
        colorAttachmentReference.attachment            = 0;
        colorAttachmentReference.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthStencilAttachmentReference = {};
        depthStencilAttachmentReference.attachment            = 1;
        depthStencilAttachmentReference.layout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // Set up the renderpass to depend on commands that execute before the renderpass begins
        VkSubpassDescription subpassDescription    = VkSubpassDescription();
        subpassDescription.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount    = 1;
        subpassDescription.pColorAttachments       = &colorAttachmentReference;
        subpassDescription.pDepthStencilAttachment = &depthStencilAttachmentReference;

        VkSubpassDependency subpassDependency = VkSubpassDependency();
        subpassDependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
        subpassDependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask       = 0;
        subpassDependency.dstSubpass          = 0;
        subpassDependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo = VkRenderPassCreateInfo();
        renderPassCreateInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount        = 2;
        renderPassCreateInfo.pAttachments           = attachmentDescriptions;
        renderPassCreateInfo.subpassCount           = 1;
        renderPassCreateInfo.pSubpasses             = &subpassDescription;
        renderPassCreateInfo.dependencyCount        = 1;
        renderPassCreateInfo.pDependencies          = &subpassDependency;

        // Create the renderpass
        if (vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Set up uniform buffer and texture sampler descriptor set layouts
    void setupDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[2];

        // Layout binding for uniform buffer
        descriptorSetLayoutBindings[0]                 = VkDescriptorSetLayoutBinding();
        descriptorSetLayoutBindings[0].binding         = 0;
        descriptorSetLayoutBindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorSetLayoutBindings[0].descriptorCount = 1;
        descriptorSetLayoutBindings[0].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;

        // Layout binding for texture sampler
        descriptorSetLayoutBindings[1]                 = VkDescriptorSetLayoutBinding();
        descriptorSetLayoutBindings[1].binding         = 1;
        descriptorSetLayoutBindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorSetLayoutBindings[1].descriptorCount = 1;
        descriptorSetLayoutBindings[1].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = VkDescriptorSetLayoutCreateInfo();
        descriptorSetLayoutCreateInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = 2;
        descriptorSetLayoutCreateInfo.pBindings    = descriptorSetLayoutBindings;

        // Create descriptor set layout
        if (vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Set up pipeline layout
    void setupPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = VkPipelineLayoutCreateInfo();
        pipelineLayoutCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount             = 1;
        pipelineLayoutCreateInfo.pSetLayouts                = &descriptorSetLayout;

        // Create pipeline layout
        if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Set up rendering pipeline
    void setupPipeline()
    {
        // Set up how the vertex shader pulls data out of our vertex buffer
        VkVertexInputBindingDescription vertexInputBindingDescription = VkVertexInputBindingDescription();
        vertexInputBindingDescription.binding                         = 0;
        vertexInputBindingDescription.stride                          = sizeof(float) * 9;
        vertexInputBindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

        // Set up how the vertex buffer data is interpreted as attributes by the vertex shader
        VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3];

        // Position attribute
        vertexInputAttributeDescriptions[0]          = VkVertexInputAttributeDescription();
        vertexInputAttributeDescriptions[0].binding  = 0;
        vertexInputAttributeDescriptions[0].location = 0;
        vertexInputAttributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[0].offset   = sizeof(float) * 0;

        // Color attribute
        vertexInputAttributeDescriptions[1]          = VkVertexInputAttributeDescription();
        vertexInputAttributeDescriptions[1].binding  = 0;
        vertexInputAttributeDescriptions[1].location = 1;
        vertexInputAttributeDescriptions[1].format   = VK_FORMAT_R32G32B32A32_SFLOAT;
        vertexInputAttributeDescriptions[1].offset   = sizeof(float) * 3;

        // Texture coordinate attribute
        vertexInputAttributeDescriptions[2]          = VkVertexInputAttributeDescription();
        vertexInputAttributeDescriptions[2].binding  = 0;
        vertexInputAttributeDescriptions[2].location = 2;
        vertexInputAttributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
        vertexInputAttributeDescriptions[2].offset   = sizeof(float) * 7;

        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = VkPipelineVertexInputStateCreateInfo();
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount   = 1;
        vertexInputStateCreateInfo.pVertexBindingDescriptions      = &vertexInputBindingDescription;
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 3;
        vertexInputStateCreateInfo.pVertexAttributeDescriptions    = vertexInputAttributeDescriptions;

        // We want to generate a triangle list with our vertex data
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = VkPipelineInputAssemblyStateCreateInfo();
        inputAssemblyStateCreateInfo.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

        // Set up the viewport
        VkViewport viewport = VkViewport();
        viewport.x          = 0.0f;
        viewport.y          = 0.0f;
        viewport.width      = static_cast<float>(swapchainExtent.width);
        viewport.height     = static_cast<float>(swapchainExtent.height);
        viewport.minDepth   = 0.0f;
        viewport.maxDepth   = 1.f;

        // Set up the scissor region
        VkRect2D scissor = VkRect2D();
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent   = swapchainExtent;

        VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = VkPipelineViewportStateCreateInfo();
        pipelineViewportStateCreateInfo.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipelineViewportStateCreateInfo.viewportCount = 1;
        pipelineViewportStateCreateInfo.pViewports    = &viewport;
        pipelineViewportStateCreateInfo.scissorCount  = 1;
        pipelineViewportStateCreateInfo.pScissors     = &scissor;

        // Set up rasterization parameters: fill polygons, no backface culling, front face is counter-clockwise
        VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = VkPipelineRasterizationStateCreateInfo();
        pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipelineRasterizationStateCreateInfo.depthClampEnable        = VK_FALSE;
        pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        pipelineRasterizationStateCreateInfo.polygonMode             = VK_POLYGON_MODE_FILL;
        pipelineRasterizationStateCreateInfo.lineWidth               = 1.0f;
        pipelineRasterizationStateCreateInfo.cullMode                = VK_CULL_MODE_NONE;
        pipelineRasterizationStateCreateInfo.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        pipelineRasterizationStateCreateInfo.depthBiasEnable         = VK_FALSE;

        // Enable depth testing and disable scissor testing
        VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = VkPipelineDepthStencilStateCreateInfo();
        pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        pipelineDepthStencilStateCreateInfo.depthTestEnable       = VK_TRUE;
        pipelineDepthStencilStateCreateInfo.depthWriteEnable      = VK_TRUE;
        pipelineDepthStencilStateCreateInfo.depthCompareOp        = VK_COMPARE_OP_LESS;
        pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        pipelineDepthStencilStateCreateInfo.stencilTestEnable     = VK_FALSE;

        // Enable multi-sampling
        VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = VkPipelineMultisampleStateCreateInfo();
        pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipelineMultisampleStateCreateInfo.sampleShadingEnable  = VK_FALSE;
        pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // Set up blending parameters
        VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = VkPipelineColorBlendAttachmentState();
        pipelineColorBlendAttachmentState.blendEnable                         = VK_TRUE;
        pipelineColorBlendAttachmentState.srcColorBlendFactor                 = VK_BLEND_FACTOR_SRC_ALPHA;
        pipelineColorBlendAttachmentState.dstColorBlendFactor                 = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        pipelineColorBlendAttachmentState.colorBlendOp                        = VK_BLEND_OP_ADD;
        pipelineColorBlendAttachmentState.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;
        pipelineColorBlendAttachmentState.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        pipelineColorBlendAttachmentState.alphaBlendOp                        = VK_BLEND_OP_ADD;
        pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                           VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = VkPipelineColorBlendStateCreateInfo();
        pipelineColorBlendStateCreateInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipelineColorBlendStateCreateInfo.logicOpEnable   = VK_FALSE;
        pipelineColorBlendStateCreateInfo.attachmentCount = 1;
        pipelineColorBlendStateCreateInfo.pAttachments    = &pipelineColorBlendAttachmentState;

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = VkGraphicsPipelineCreateInfo();
        graphicsPipelineCreateInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.stageCount                   = 2;
        graphicsPipelineCreateInfo.pStages                      = shaderStages;
        graphicsPipelineCreateInfo.pVertexInputState            = &vertexInputStateCreateInfo;
        graphicsPipelineCreateInfo.pInputAssemblyState          = &inputAssemblyStateCreateInfo;
        graphicsPipelineCreateInfo.pViewportState               = &pipelineViewportStateCreateInfo;
        graphicsPipelineCreateInfo.pRasterizationState          = &pipelineRasterizationStateCreateInfo;
        graphicsPipelineCreateInfo.pDepthStencilState           = &pipelineDepthStencilStateCreateInfo;
        graphicsPipelineCreateInfo.pMultisampleState            = &pipelineMultisampleStateCreateInfo;
        graphicsPipelineCreateInfo.pColorBlendState             = &pipelineColorBlendStateCreateInfo;
        graphicsPipelineCreateInfo.layout                       = pipelineLayout;
        graphicsPipelineCreateInfo.renderPass                   = renderPass;
        graphicsPipelineCreateInfo.subpass                      = 0;

        // Create our graphics pipeline
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &graphicsPipeline) !=
            VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Use our renderpass and swapchain images to create the corresponding framebuffers
    void setupFramebuffers()
    {
        swapchainFramebuffers.resize(swapchainImageViews.size());

        VkFramebufferCreateInfo framebufferCreateInfo = VkFramebufferCreateInfo();
        framebufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass              = renderPass;
        framebufferCreateInfo.attachmentCount         = 2;
        framebufferCreateInfo.width                   = swapchainExtent.width;
        framebufferCreateInfo.height                  = swapchainExtent.height;
        framebufferCreateInfo.layers                  = 1;

        for (std::size_t i = 0; i < swapchainFramebuffers.size(); ++i)
        {
            // Each framebuffer consists of a corresponding swapchain image and the shared depth image
            VkImageView attachments[] = {swapchainImageViews[i], depthImageView};

            framebufferCreateInfo.pAttachments = attachments;

            // Create the framebuffer
            if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Set up our command pool
    void setupCommandPool()
    {
        // We want to be able to reset command buffers after submitting them
        VkCommandPoolCreateInfo commandPoolCreateInfo = VkCommandPoolCreateInfo();
        commandPoolCreateInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex        = static_cast<std::uint32_t>(queueFamilyIndex);
        commandPoolCreateInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        // Create our command pool
        if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Helper to create a generic buffer with the specified size, usage and memory flags
    bool createBuffer(VkDeviceSize          size,
                      VkBufferUsageFlags    usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer&             buffer,
                      VkDeviceMemory&       memory)
    {
        // We only have a single queue so we can request exclusive access
        VkBufferCreateInfo bufferCreateInfo = VkBufferCreateInfo();
        bufferCreateInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size               = size;
        bufferCreateInfo.usage              = usage;
        bufferCreateInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

        // Create the buffer, this does not allocate any memory for it yet
        if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS)
            return false;

        // Check what kind of memory we need to request from the GPU
        VkMemoryRequirements memoryRequirements = VkMemoryRequirements();
        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        // Check what GPU memory type is available for us to allocate out of
        VkPhysicalDeviceMemoryProperties memoryProperties = VkPhysicalDeviceMemoryProperties();
        vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

        std::uint32_t memoryType = 0;

        for (; memoryType < memoryProperties.memoryTypeCount; ++memoryType)
        {
            if ((memoryRequirements.memoryTypeBits & static_cast<unsigned int>(1 << memoryType)) &&
                ((memoryProperties.memoryTypes[memoryType].propertyFlags & properties) == properties))
                break;
        }

        if (memoryType == memoryProperties.memoryTypeCount)
            return false;

        VkMemoryAllocateInfo memoryAllocateInfo = VkMemoryAllocateInfo();
        memoryAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize       = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex      = memoryType;

        // Allocate the memory out of the GPU pool for the required memory type
        if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &memory) != VK_SUCCESS)
            return false;

        // Bind the allocated memory to our buffer object
        if (vkBindBufferMemory(device, buffer, memory, 0) != VK_SUCCESS)
            return false;

        return true;
    }

    // Helper to copy the contents of one buffer to another buffer
    bool copyBuffer(VkBuffer dst, VkBuffer src, VkDeviceSize size)
    {
        // Allocate a primary command buffer out of our command pool
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
        commandBufferAllocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool                 = commandPool;
        commandBufferAllocateInfo.commandBufferCount          = 1;

        VkCommandBuffer commandBuffer;

        if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer) != VK_SUCCESS)
            return false;

        // Begin the command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo = VkCommandBufferBeginInfo();
        commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            return false;
        }

        // Add our buffer copy command
        VkBufferCopy bufferCopy = VkBufferCopy();
        bufferCopy.srcOffset    = 0;
        bufferCopy.dstOffset    = 0;
        bufferCopy.size         = size;

        vkCmdCopyBuffer(commandBuffer, src, dst, 1, &bufferCopy);

        // End and submit the command buffer
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo       = VkSubmitInfo();
        submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &commandBuffer;

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            return false;
        }

        // Ensure the command buffer has been processed
        if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            return false;
        }

        // Free the command buffer
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

        return true;
    }

    // Create our vertex buffer and upload its data
    void setupVertexBuffer()
    {
        // clang-format off
        constexpr std::array vertexData = {
            // X      Y      Z     R     G     B     A     U     V
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

             0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
        };
        // clang-format on

        // Create a staging buffer that is writable by the CPU
        VkBuffer       stagingBuffer       = {};
        VkDeviceMemory stagingBufferMemory = {};

        if (!createBuffer(sizeof(vertexData),
                          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          stagingBuffer,
                          stagingBufferMemory))
        {
            vulkanAvailable = false;
            return;
        }

        void* ptr;

        // Map the buffer into our address space
        if (vkMapMemory(device, stagingBufferMemory, 0, sizeof(vertexData), 0, &ptr) != VK_SUCCESS)
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the vertex data into the buffer
        std::memcpy(ptr, vertexData.data(), sizeof(vertexData));

        // Unmap the buffer
        vkUnmapMemory(device, stagingBufferMemory);

        // Create the GPU local vertex buffer
        if (!createBuffer(sizeof(vertexData),
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                          vertexBuffer,
                          vertexBufferMemory))
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the contents of the staging buffer into the GPU vertex buffer
        vulkanAvailable = copyBuffer(vertexBuffer, stagingBuffer, sizeof(vertexData));

        // Free the staging buffer and its memory
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        vkDestroyBuffer(device, stagingBuffer, nullptr);
    }

    // Create our index buffer and upload its data
    void setupIndexBuffer()
    {
        // clang-format off
        constexpr std::array<std::uint16_t, 36> indexData = {
            0,  1,  2,
            2,  3,  0,

            4,  5,  6,
            6,  7,  4,

            8,  9,  10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };
        // clang-format on

        // Create a staging buffer that is writable by the CPU
        VkBuffer       stagingBuffer       = {};
        VkDeviceMemory stagingBufferMemory = {};

        if (!createBuffer(sizeof(indexData),
                          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          stagingBuffer,
                          stagingBufferMemory))
        {
            vulkanAvailable = false;
            return;
        }

        void* ptr;

        // Map the buffer into our address space
        if (vkMapMemory(device, stagingBufferMemory, 0, sizeof(indexData), 0, &ptr) != VK_SUCCESS)
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the index data into the buffer
        std::memcpy(ptr, indexData.data(), sizeof(indexData));

        // Unmap the buffer
        vkUnmapMemory(device, stagingBufferMemory);

        // Create the GPU local index buffer
        if (!createBuffer(sizeof(indexData),
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                          indexBuffer,
                          indexBufferMemory))
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the contents of the staging buffer into the GPU index buffer
        vulkanAvailable = copyBuffer(indexBuffer, stagingBuffer, sizeof(indexData));

        // Free the staging buffer and its memory
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        vkDestroyBuffer(device, stagingBuffer, nullptr);
    }

    // Create our uniform buffer but don't upload any data yet
    void setupUniformBuffers()
    {
        // Create a uniform buffer for every frame that might be in flight to prevent clobbering
        for (std::size_t i = 0; i < swapchainImages.size(); ++i)
        {
            uniformBuffers.push_back({});
            uniformBuffersMemory.push_back({});

            // The uniform buffer will be host visible and coherent since we use it for streaming data every frame
            if (!createBuffer(sizeof(Matrix) * 3,
                              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                              uniformBuffers[i],
                              uniformBuffersMemory[i]))
            {
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Helper to create a generic image with the specified size, format, usage and memory flags
    bool createImage(std::uint32_t         width,
                     std::uint32_t         height,
                     VkFormat              format,
                     VkImageTiling         tiling,
                     VkImageUsageFlags     usage,
                     VkMemoryPropertyFlags properties,
                     VkImage&              image,
                     VkDeviceMemory&       imageMemory)
    {
        // We only have a single queue so we can request exclusive access
        VkImageCreateInfo imageCreateInfo = VkImageCreateInfo();
        imageCreateInfo.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType         = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width      = width;
        imageCreateInfo.extent.height     = height;
        imageCreateInfo.extent.depth      = 1;
        imageCreateInfo.mipLevels         = 1;
        imageCreateInfo.arrayLayers       = 1;
        imageCreateInfo.format            = format;
        imageCreateInfo.tiling            = tiling;
        imageCreateInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage             = usage;
        imageCreateInfo.samples           = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;

        // Create the image, this does not allocate any memory for it yet
        if (vkCreateImage(device, &imageCreateInfo, nullptr, &image) != VK_SUCCESS)
            return false;

        // Check what kind of memory we need to request from the GPU
        VkMemoryRequirements memoryRequirements = VkMemoryRequirements();
        vkGetImageMemoryRequirements(device, image, &memoryRequirements);

        // Check what GPU memory type is available for us to allocate out of
        VkPhysicalDeviceMemoryProperties memoryProperties = VkPhysicalDeviceMemoryProperties();
        vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

        std::uint32_t memoryType = 0;

        for (; memoryType < memoryProperties.memoryTypeCount; ++memoryType)
        {
            if ((memoryRequirements.memoryTypeBits & static_cast<unsigned int>(1 << memoryType)) &&
                ((memoryProperties.memoryTypes[memoryType].propertyFlags & properties) == properties))
                break;
        }

        if (memoryType == memoryProperties.memoryTypeCount)
            return false;

        VkMemoryAllocateInfo memoryAllocateInfo = VkMemoryAllocateInfo();
        memoryAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize       = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex      = memoryType;

        // Allocate the memory out of the GPU pool for the required memory type
        if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &imageMemory) != VK_SUCCESS)
            return false;

        // Bind the allocated memory to our image object
        if (vkBindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS)
            return false;

        return true;
    }

    // Create our depth image and transition it into the proper layout
    void setupDepthImage()
    {
        // Create our depth image
        if (!createImage(swapchainExtent.width,
                         swapchainExtent.height,
                         depthFormat,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         depthImage,
                         depthImageMemory))
        {
            vulkanAvailable = false;
            return;
        }

        // Allocate a command buffer
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
        commandBufferAllocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool                 = commandPool;
        commandBufferAllocateInfo.commandBufferCount          = 1;

        VkCommandBuffer commandBuffer;

        if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Begin the command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo = VkCommandBufferBeginInfo();
        commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkSubmitInfo submitInfo       = VkSubmitInfo();
        submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &commandBuffer;

        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vulkanAvailable = false;
            return;
        }

        // Submit a barrier to transition the image layout to depth stencil optimal
        VkImageMemoryBarrier barrier        = VkImageMemoryBarrier();
        barrier.sType                       = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout                   = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout                   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                       = depthImage;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT |
                                              ((depthFormat == VK_FORMAT_D32_SFLOAT) ? 0 : VK_IMAGE_ASPECT_STENCIL_BIT);
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        barrier.srcAccessMask                   = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        // End and submit the command buffer
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vulkanAvailable = false;
            return;
        }

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vulkanAvailable = false;
            return;
        }

        // Ensure the command buffer has been processed
        if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vulkanAvailable = false;
            return;
        }

        // Free the command buffer
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    // Create an image view for our depth image
    void setupDepthImageView()
    {
        VkImageViewCreateInfo imageViewCreateInfo = VkImageViewCreateInfo();
        imageViewCreateInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image                 = depthImage;
        imageViewCreateInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                = depthFormat;
        imageViewCreateInfo.subresourceRange
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT |
                          ((depthFormat == VK_FORMAT_D32_SFLOAT) ? 0 : VK_IMAGE_ASPECT_STENCIL_BIT);
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;

        // Create the depth image view
        if (vkCreateImageView(device, &imageViewCreateInfo, nullptr, &depthImageView) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Create an image for our texture data
    void setupTextureImage()
    {
        // Load the image data
        sf::Image imageData;

        if (!imageData.loadFromFile("resources/logo.png"))
        {
            vulkanAvailable = false;
            return;
        }

        // Create a staging buffer to transfer the data with
        const VkDeviceSize imageSize = imageData.getSize().x * imageData.getSize().y * 4;

        VkBuffer       stagingBuffer       = {};
        VkDeviceMemory stagingBufferMemory = {};
        createBuffer(imageSize,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer,
                     stagingBufferMemory);

        void* ptr;

        // Map the buffer into our address space
        if (vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &ptr) != VK_SUCCESS)
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the image data into the buffer
        std::memcpy(ptr, imageData.getPixelsPtr(), static_cast<std::size_t>(imageSize));

        // Unmap the buffer
        vkUnmapMemory(device, stagingBufferMemory);

        // Create a GPU local image
        if (!createImage(imageData.getSize().x,
                         imageData.getSize().y,
                         VK_FORMAT_R8G8B8A8_UNORM,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         textureImage,
                         textureImageMemory))
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Create a command buffer
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
        commandBufferAllocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool                 = commandPool;
        commandBufferAllocateInfo.commandBufferCount          = 1;

        VkCommandBuffer commandBuffer;

        if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer) != VK_SUCCESS)
        {
            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Begin the command buffer
        VkCommandBufferBeginInfo commandBufferBeginInfo = VkCommandBufferBeginInfo();
        commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkSubmitInfo submitInfo       = VkSubmitInfo();
        submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &commandBuffer;

        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Submit a barrier to transition the image layout to transfer destination optimal
        VkImageMemoryBarrier barrier            = VkImageMemoryBarrier();
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout                       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = textureImage;
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        barrier.srcAccessMask                   = 0;
        barrier.dstAccessMask                   = VK_ACCESS_TRANSFER_WRITE_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Ensure the command buffer has been processed
        if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Begin the command buffer
        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Copy the staging buffer contents into the image
        VkBufferImageCopy bufferImageCopy               = VkBufferImageCopy();
        bufferImageCopy.bufferOffset                    = 0;
        bufferImageCopy.bufferRowLength                 = 0;
        bufferImageCopy.bufferImageHeight               = 0;
        bufferImageCopy.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel       = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount     = 1;
        bufferImageCopy.imageOffset.x                   = 0;
        bufferImageCopy.imageOffset.y                   = 0;
        bufferImageCopy.imageOffset.z                   = 0;
        bufferImageCopy.imageExtent.width               = imageData.getSize().x;
        bufferImageCopy.imageExtent.height              = imageData.getSize().y;
        bufferImageCopy.imageExtent.depth               = 1;

        vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

        // End and submit the command buffer
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Ensure the command buffer has been processed
        if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Begin the command buffer
        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Submit a barrier to transition the image layout from transfer destination optimal to shader read-only optimal
        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        // End and submit the command buffer
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Ensure the command buffer has been processed
        if (vkQueueWaitIdle(queue) != VK_SUCCESS)
        {
            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            vkFreeMemory(device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(device, stagingBuffer, nullptr);

            vulkanAvailable = false;
            return;
        }

        // Free the command buffer
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

        vkFreeMemory(device, stagingBufferMemory, nullptr);
        vkDestroyBuffer(device, stagingBuffer, nullptr);
    }

    // Create an image view for our texture
    void setupTextureImageView()
    {
        VkImageViewCreateInfo imageViewCreateInfo           = VkImageViewCreateInfo();
        imageViewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image                           = textureImage;
        imageViewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                          = VK_FORMAT_R8G8B8A8_UNORM;
        imageViewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;

        // Create our texture image view
        if (vkCreateImageView(device, &imageViewCreateInfo, nullptr, &textureImageView) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Create a sampler for our texture
    void setupTextureSampler()
    {
        // Sampler parameters: linear min/mag filtering, 4x anisotropic
        VkSamplerCreateInfo samplerCreateInfo     = VkSamplerCreateInfo();
        samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
        samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.anisotropyEnable        = VK_TRUE;
        samplerCreateInfo.maxAnisotropy           = 4;
        samplerCreateInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
        samplerCreateInfo.compareEnable           = VK_FALSE;
        samplerCreateInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.mipLodBias              = 0.0f;
        samplerCreateInfo.minLod                  = 0.0f;
        samplerCreateInfo.maxLod                  = 0.0f;

        // Create our sampler
        if (vkCreateSampler(device, &samplerCreateInfo, nullptr, &textureSampler) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Set up our descriptor pool
    void setupDescriptorPool()
    {
        // We need to allocate as many descriptor sets as we have frames in flight
        VkDescriptorPoolSize descriptorPoolSizes[2];

        descriptorPoolSizes[0]                 = VkDescriptorPoolSize();
        descriptorPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorPoolSizes[0].descriptorCount = static_cast<std::uint32_t>(swapchainImages.size());

        descriptorPoolSizes[1]                 = VkDescriptorPoolSize();
        descriptorPoolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorPoolSizes[1].descriptorCount = static_cast<std::uint32_t>(swapchainImages.size());

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = VkDescriptorPoolCreateInfo();
        descriptorPoolCreateInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.poolSizeCount              = 2;
        descriptorPoolCreateInfo.pPoolSizes                 = descriptorPoolSizes;
        descriptorPoolCreateInfo.maxSets                    = static_cast<std::uint32_t>(swapchainImages.size());

        // Create the descriptor pool
        if (vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    // Set up our descriptor sets
    void setupDescriptorSets()
    {
        // Allocate a descriptor set for each frame in flight
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts(swapchainImages.size(), descriptorSetLayout);

        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = VkDescriptorSetAllocateInfo();
        descriptorSetAllocateInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.descriptorPool              = descriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount          = static_cast<std::uint32_t>(swapchainImages.size());
        descriptorSetAllocateInfo.pSetLayouts                 = descriptorSetLayouts.data();

        descriptorSets.resize(swapchainImages.size());

        if (vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets.data()) != VK_SUCCESS)
        {
            descriptorSets.clear();

            vulkanAvailable = false;
            return;
        }

        // For every descriptor set, set up the bindings to our uniform buffer and texture sampler
        for (std::size_t i = 0; i < descriptorSets.size(); ++i)
        {
            VkWriteDescriptorSet writeDescriptorSets[2];

            // Uniform buffer binding information
            VkDescriptorBufferInfo descriptorBufferInfo = VkDescriptorBufferInfo();
            descriptorBufferInfo.buffer                 = uniformBuffers[i];
            descriptorBufferInfo.offset                 = 0;
            descriptorBufferInfo.range                  = sizeof(Matrix) * 3;

            writeDescriptorSets[0]                 = VkWriteDescriptorSet();
            writeDescriptorSets[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSets[0].dstSet          = descriptorSets[i];
            writeDescriptorSets[0].dstBinding      = 0;
            writeDescriptorSets[0].dstArrayElement = 0;
            writeDescriptorSets[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSets[0].descriptorCount = 1;
            writeDescriptorSets[0].pBufferInfo     = &descriptorBufferInfo;

            // Texture sampler binding information
            VkDescriptorImageInfo descriptorImageInfo = VkDescriptorImageInfo();
            descriptorImageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptorImageInfo.imageView             = textureImageView;
            descriptorImageInfo.sampler               = textureSampler;

            writeDescriptorSets[1]                 = VkWriteDescriptorSet();
            writeDescriptorSets[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSets[1].dstSet          = descriptorSets[i];
            writeDescriptorSets[1].dstBinding      = 1;
            writeDescriptorSets[1].dstArrayElement = 0;
            writeDescriptorSets[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptorSets[1].descriptorCount = 1;
            writeDescriptorSets[1].pImageInfo      = &descriptorImageInfo;

            // Update the descriptor set
            vkUpdateDescriptorSets(device, 2, writeDescriptorSets, 0, nullptr);
        }
    }

    // Set up the command buffers we use for drawing each frame
    void setupCommandBuffers()
    {
        // We need a command buffer for every frame in flight
        commandBuffers.resize(swapchainFramebuffers.size());

        // These are primary command buffers
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
        commandBufferAllocateInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.commandPool                 = commandPool;
        commandBufferAllocateInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount          = static_cast<std::uint32_t>(commandBuffers.size());

        // Allocate the command buffers from our command pool
        if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            commandBuffers.clear();
            vulkanAvailable = false;
            return;
        }
    }

    // Set up the commands we need to issue to draw a frame
    void setupDraw()
    {
        // Set up our clear colors
        VkClearValue clearColors[2];

        // Clear color buffer to opaque black
        clearColors[0]                  = VkClearValue();
        clearColors[0].color.float32[0] = 0.0f;
        clearColors[0].color.float32[1] = 0.0f;
        clearColors[0].color.float32[2] = 0.0f;
        clearColors[0].color.float32[3] = 0.0f;

        // Clear depth to 1.0f
        clearColors[1]                      = VkClearValue();
        clearColors[1].depthStencil.depth   = 1.0f;
        clearColors[1].depthStencil.stencil = 0;

        VkRenderPassBeginInfo renderPassBeginInfo = VkRenderPassBeginInfo();
        renderPassBeginInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass            = renderPass;
        renderPassBeginInfo.renderArea.offset.x   = 0;
        renderPassBeginInfo.renderArea.offset.y   = 0;
        renderPassBeginInfo.renderArea.extent     = swapchainExtent;
        renderPassBeginInfo.clearValueCount       = 2;
        renderPassBeginInfo.pClearValues          = clearColors;

        // Simultaneous use: this command buffer can be resubmitted to a queue before a previous submission is completed
        VkCommandBufferBeginInfo commandBufferBeginInfo = VkCommandBufferBeginInfo();
        commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        // Set up the command buffers for each frame in flight
        for (std::size_t i = 0; i < commandBuffers.size(); ++i)
        {
            // Begin the command buffer
            if (vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }

            // Begin the renderpass
            renderPassBeginInfo.framebuffer = swapchainFramebuffers[i];

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Bind our graphics pipeline
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            // Bind our vertex buffer
            const VkDeviceSize offset = 0;

            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &vertexBuffer, &offset);

            // Bind our index buffer
            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            // Bind our descriptor sets
            vkCmdBindDescriptorSets(commandBuffers[i],
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipelineLayout,
                                    0,
                                    1,
                                    &descriptorSets[i],
                                    0,
                                    nullptr);

            // Draw our primitives
            vkCmdDrawIndexed(commandBuffers[i], 36, 1, 0, 0, 0);

            // End the renderpass
            vkCmdEndRenderPass(commandBuffers[i]);

            // End the command buffer
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Set up the semaphores we use to synchronize frames among each other
    void setupSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = VkSemaphoreCreateInfo();
        semaphoreCreateInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        // Create a semaphore to track when an swapchain image is available for each frame in flight
        for (std::size_t i = 0; i < maxFramesInFlight; ++i)
        {
            imageAvailableSemaphores.push_back({});

            if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS)
            {
                imageAvailableSemaphores.pop_back();
                vulkanAvailable = false;
                return;
            }
        }

        // Create a semaphore to track when rendering is complete for each frame in flight
        for (std::size_t i = 0; i < maxFramesInFlight; ++i)
        {
            renderFinishedSemaphores.push_back({});

            if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
            {
                renderFinishedSemaphores.pop_back();
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Set up the fences we use to synchronize frames among each other
    void setupFences()
    {
        // Create the fences in the signaled state
        VkFenceCreateInfo fenceCreateInfo = VkFenceCreateInfo();
        fenceCreateInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

        // Create a fence to track when queue submission is complete for each frame in flight
        for (std::size_t i = 0; i < maxFramesInFlight; ++i)
        {
            fences.push_back({});

            if (vkCreateFence(device, &fenceCreateInfo, nullptr, &fences[i]) != VK_SUCCESS)
            {
                fences.pop_back();
                vulkanAvailable = false;
                return;
            }
        }
    }

    // Update the matrices in our uniform buffer every frame
    void updateUniformBuffer(float elapsed)
    {
        // Construct the model matrix
        Matrix model = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};

        matrixRotateX(model, sf::degrees(elapsed * 59.0f));
        matrixRotateY(model, sf::degrees(elapsed * 83.0f));
        matrixRotateZ(model, sf::degrees(elapsed * 109.0f));

        // Translate the model based on the mouse position
        const sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        const sf::Vector2f windowSize    = sf::Vector2f(window.getSize());
        const float        x             = std::clamp(mousePosition.x * 2.f / windowSize.x - 1.f, -1.0f, 1.0f) * 2.0f;
        const float        y             = std::clamp(-mousePosition.y * 2.f / windowSize.y + 1.f, -1.0f, 1.0f) * 1.5f;

        model[3][0] -= x;
        model[3][2] += y;

        // Construct the view matrix
        const Vec3 eye    = {0.0f, 4.0f, 0.0f};
        const Vec3 center = {0.0f, 0.0f, 0.0f};
        const Vec3 up     = {0.0f, 0.0f, 1.0f};

        Matrix view;

        matrixLookAt(view, eye, center, up);

        // Construct the projection matrix
        const sf::Angle fov    = sf::degrees(45);
        const float     aspect = static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height);
        const float     nearPlane = 0.1f;
        const float     farPlane  = 10.0f;

        Matrix projection;

        matrixPerspective(projection, fov, aspect, nearPlane, farPlane);

        char* ptr;

        // Map the current frame's uniform buffer into our address space
        if (vkMapMemory(device, uniformBuffersMemory[currentFrame], 0, sizeof(Matrix) * 3, 0, reinterpret_cast<void**>(&ptr)) !=
            VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Copy the matrix data into the current frame's uniform buffer
        std::memcpy(ptr + sizeof(Matrix) * 0, model, sizeof(Matrix));
        std::memcpy(ptr + sizeof(Matrix) * 1, view, sizeof(Matrix));
        std::memcpy(ptr + sizeof(Matrix) * 2, projection, sizeof(Matrix));

        // Unmap the buffer
        vkUnmapMemory(device, uniformBuffersMemory[currentFrame]);
    }

    void draw()
    {
        std::uint32_t imageIndex = 0;

        // If the objects we need to submit this frame are still pending, wait here
        vkWaitForFences(device, 1, &fences[currentFrame], VK_TRUE, std::numeric_limits<std::uint64_t>::max());

        {
            // Get the next image in the swapchain
            const VkResult result = vkAcquireNextImageKHR(device,
                                                          swapchain,
                                                          std::numeric_limits<std::uint64_t>::max(),
                                                          imageAvailableSemaphores[currentFrame],
                                                          VK_NULL_HANDLE,
                                                          &imageIndex);

            // Check if we need to re-create the swapchain (e.g. if the window was resized)
            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                recreateSwapchain();
                swapchainOutOfDate = false;
                return;
            }

            if ((result != VK_SUCCESS) && (result != VK_TIMEOUT) && (result != VK_NOT_READY) &&
                (result != VK_SUBOPTIMAL_KHR))
            {
                vulkanAvailable = false;
                return;
            }
        }

        // Wait for the swapchain image to be available in the color attachment stage before submitting the queue
        const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        // Signal the render finished semaphore once the queue has been processed
        VkSubmitInfo submitInfo         = VkSubmitInfo();
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount   = 1;
        submitInfo.pWaitSemaphores      = &imageAvailableSemaphores[currentFrame];
        submitInfo.pWaitDstStageMask    = &waitStages;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffers[imageIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = &renderFinishedSemaphores[currentFrame];

        vkResetFences(device, 1, &fences[currentFrame]);

        if (vkQueueSubmit(queue, 1, &submitInfo, fences[currentFrame]) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Wait for rendering to complete before presenting
        VkPresentInfoKHR presentInfo   = VkPresentInfoKHR();
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = &renderFinishedSemaphores[currentFrame];
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &swapchain;
        presentInfo.pImageIndices      = &imageIndex;

        {
            // Queue presentation
            const VkResult result = vkQueuePresentKHR(queue, &presentInfo);

            // Check if we need to re-create the swapchain (e.g. if the window was resized)
            if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR) || swapchainOutOfDate)
            {
                recreateSwapchain();
                swapchainOutOfDate = false;
            }
            else if (result != VK_SUCCESS)
            {
                vulkanAvailable = false;
                return;
            }
        }

        // Make sure to use the next frame's objects next frame
        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    void run()
    {
        const sf::Clock clock;

        // Start game loop
        while (window.isOpen())
        {
            // Process events
            for (sf::Event event; window.pollEvent(event);)
            {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    window.close();

                // Escape key: exit
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                    window.close();

                // Re-create the swapchain when the window is resized
                if (event.type == sf::Event::Resized)
                    swapchainOutOfDate = true;
            }

            // Check that window was not closed before drawing to it
            if (vulkanAvailable && window.isOpen())
            {
                // Update the uniform buffer (matrices)
                updateUniformBuffer(clock.getElapsedTime().asSeconds());

                // Render the frame
                draw();
            }
        }
    }

private:
    sf::WindowBase window{sf::VideoMode({800, 600}), "SFML window with Vulkan", sf::Style::Default};

    bool vulkanAvailable{sf::Vulkan::isAvailable()};

    const unsigned int maxFramesInFlight{2};
    unsigned int       currentFrame{};
    bool               swapchainOutOfDate{};

    VkInstance                      instance{};
    VkDebugReportCallbackEXT        debugReportCallback{};
    VkSurfaceKHR                    surface{};
    VkPhysicalDevice                gpu{};
    int                             queueFamilyIndex{-1};
    VkDevice                        device{};
    VkQueue                         queue{};
    VkSurfaceFormatKHR              swapchainFormat{};
    VkExtent2D                      swapchainExtent{};
    VkSwapchainKHR                  swapchain{};
    std::vector<VkImage>            swapchainImages;
    std::vector<VkImageView>        swapchainImageViews;
    VkFormat                        depthFormat{VK_FORMAT_UNDEFINED};
    VkImage                         depthImage{};
    VkDeviceMemory                  depthImageMemory{};
    VkImageView                     depthImageView{};
    VkShaderModule                  vertexShaderModule{};
    VkShaderModule                  fragmentShaderModule{};
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkDescriptorSetLayout           descriptorSetLayout{};
    VkPipelineLayout                pipelineLayout{};
    VkRenderPass                    renderPass{};
    VkPipeline                      graphicsPipeline{};
    std::vector<VkFramebuffer>      swapchainFramebuffers;
    VkCommandPool                   commandPool{};
    VkBuffer                        vertexBuffer{};
    VkDeviceMemory                  vertexBufferMemory{};
    VkBuffer                        indexBuffer{};
    VkDeviceMemory                  indexBufferMemory{};
    std::vector<VkBuffer>           uniformBuffers;
    std::vector<VkDeviceMemory>     uniformBuffersMemory;
    VkImage                         textureImage{};
    VkDeviceMemory                  textureImageMemory{};
    VkImageView                     textureImageView{};
    VkSampler                       textureSampler{};
    VkDescriptorPool                descriptorPool{};
    std::vector<VkDescriptorSet>    descriptorSets;
    std::vector<VkCommandBuffer>    commandBuffers;
    std::vector<VkSemaphore>        imageAvailableSemaphores;
    std::vector<VkSemaphore>        renderFinishedSemaphores;
    std::vector<VkFence>            fences;
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    VulkanExample example;

    example.run();
}
