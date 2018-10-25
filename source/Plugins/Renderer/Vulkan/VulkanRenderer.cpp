#include "VulkanRenderer.h"
#include "vulkan/vulkan.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include "Core/ServiceLocator.h"
#include "Memory/MemoryManager.h"
#include "Window/WindowManager.h"



using namespace std;
using namespace Viper;
using namespace Viper::Gameplay;
using namespace Viper::Memory;
using namespace Viper::Window;


namespace Viper
{
	namespace Graphics
	{
#pragma region VulkanRendererPrivate
		class VulkanRendererPrivate
		{
		public:
			VulkanRendererPrivate(ServiceLocator& serviceLocator);
			void CreateInstance();
			void SetupDebugCallback();
			void CreateSurface();
			void PickPhysicalDevice();
			void CreateLogicalDevice();
			void CreateSwapChain(const WindowContext& windowContext);
			void CreateImageViews();
			void CreateRenderPass();
			void CreateGraphicsPipeline();
			void CreateFramebuffers();
			void CreateCommandPool();
			void CreateCommandBuffers();
			void CreateSyncObjects();
		private:
			struct QueueFamilyIndices
			{
				uint32_t graphicsFamily;
				uint32_t presentFamily;

				QueueFamilyIndices() : graphicsFamily(static_cast<uint32_t>(-1)), presentFamily(static_cast<uint32_t>(-1))
				{
				}

				bool IsComplete()
				{
					return graphicsFamily != -1 && presentFamily != -1;
				}
			};

			struct SwapChainSupportDetails
			{
				VkSurfaceCapabilitiesKHR capabilities;
				vector<VkSurfaceFormatKHR> formats;
				vector<VkPresentModeKHR> presentModes;
			};

#pragma region PrivateMembers
			ServiceLocator& serviceLocator;

			VkInstance instance;
			VkDebugUtilsMessengerEXT callback;
			VkSurfaceKHR surface;

			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkDevice device;

			VkQueue graphicsQueue;
			VkQueue presentQueue;

			VkSwapchainKHR swapChain;
			vector<VkImage> swapChainImages;
			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;
			vector<VkImageView> swapChainImageViews;
			vector<VkFramebuffer> swapChainFramebuffers;

			VkRenderPass renderPass;
			VkPipelineLayout pipelineLayout;
			VkPipeline graphicsPipeline;

			VkCommandPool commandPool;
			vector<VkCommandBuffer> commandBuffers;

			vector<VkSemaphore> imageAvailableSemaphores;
			vector<VkSemaphore> renderFinishedSemaphores;
			vector<VkFence> inFlightFences;
			size_t currentFrame = 0;

			bool framebufferResized = false;

			VkShaderModule CreateShaderModule(const vector<char>& code);
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR> availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const WindowContext& windowContext);
			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
			bool IsDeviceSuitable(VkPhysicalDevice device);
			bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
#pragma endregion

#pragma region StaticMembers
			static const int MaxFramesInFlight;
			static const vector<const char*> ValidationLayers;
			static const vector<const char*> DeviceExtensions;
			static const bool EnableValidationLayers;

			static vector<const char*> GetRequiredExtensions();
			static bool CheckValidationLayerSupport();
			static vector<char> ReadFile(const string& filename);

			static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pCallback);
			static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
#pragma endregion
		};

#pragma region CallableByVulkanRenderer
		VulkanRendererPrivate::VulkanRendererPrivate(ServiceLocator& serviceLocator) : serviceLocator(serviceLocator)
		{
		}

		void VulkanRendererPrivate::CreateInstance()
		{
			if (EnableValidationLayers && !CheckValidationLayerSupport())
			{
				throw GameException("validation layers requested, but not available!");
			}

			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = GetRequiredExtensions();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();

			if (EnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
				createInfo.ppEnabledLayerNames = ValidationLayers.data();
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			{
				throw GameException("failed to create instance!");
			}
		}

		void VulkanRendererPrivate::SetupDebugCallback()
		{
			if (!EnableValidationLayers) return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = DebugCallback;

			if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
			{
				throw GameException("failed to set up debug callback!");
			}
		}

		void VulkanRendererPrivate::CreateSurface()
		{
			//TODO: GLFW
			/*if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
			{
				throw GameException("failed to create window surface!");
			}*/
		}

		void VulkanRendererPrivate::PickPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			if (deviceCount == 0)
			{
				throw GameException("failed to find GPUs with Vulkan support!");
			}

			vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			for (const auto& deviceTemp : devices)
			{
				if (IsDeviceSuitable(deviceTemp))
				{
					physicalDevice = deviceTemp;
					break;
				}
			}

			if (physicalDevice == VK_NULL_HANDLE)
			{
				throw GameException("failed to find a suitable GPU!");
			}
		}

		void VulkanRendererPrivate::CreateLogicalDevice()
		{
			QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

			vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
			createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

			if (EnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
				createInfo.ppEnabledLayerNames = ValidationLayers.data();
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
			{
				throw GameException("failed to create logical device!");
			}

			vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
			vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
		}

		void VulkanRendererPrivate::CreateSwapChain(const WindowContext& windowContext)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, windowContext);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface;

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
			uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

			if (indices.graphicsFamily != indices.presentFamily)
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
			{
				throw GameException("failed to create swap chain!");
			}

			vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
			swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

			swapChainImageFormat = surfaceFormat.format;
			swapChainExtent = extent;
		}

		void VulkanRendererPrivate::CreateImageViews()
		{
			swapChainImageViews.resize(swapChainImages.size());

			for (size_t i = 0; i < swapChainImages.size(); ++i)
			{
				VkImageViewCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = swapChainImages[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = swapChainImageFormat;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
				{
					throw GameException("failed to create image views!");
				}
			}
		}

		void VulkanRendererPrivate::CreateRenderPass()
		{
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = swapChainImageFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
			{
				throw GameException("failed to create render pass!");
			}
		}

		void VulkanRendererPrivate::CreateGraphicsPipeline()
		{
			auto vertShaderCode = ReadFile("shaders/vert.spv");
			auto fragShaderCode = ReadFile("shaders/frag.spv");

			VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
			VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.vertexAttributeDescriptionCount = 0;

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)swapChainExtent.width;
			viewport.height = (float)swapChainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;

			if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			{
				throw GameException("failed to create pipeline layout!");
			}

			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
			{
				throw GameException("failed to create graphics pipeline!");
			}

			vkDestroyShaderModule(device, fragShaderModule, nullptr);
			vkDestroyShaderModule(device, vertShaderModule, nullptr);
		}

		void VulkanRendererPrivate::CreateFramebuffers()
		{
			swapChainFramebuffers.resize(swapChainImageViews.size());

			for (size_t i = 0; i < swapChainImageViews.size(); ++i)
			{
				VkImageView attachments[] = {
					swapChainImageViews[i]
				};

				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = swapChainExtent.width;
				framebufferInfo.height = swapChainExtent.height;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
				{
					throw GameException("failed to create framebuffer!");
				}
			}
		}

		void VulkanRendererPrivate::CreateCommandPool()
		{
			QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);
			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

			if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
			{
				throw GameException("failed to create command pool!");
			}
		}

		void VulkanRendererPrivate::CreateCommandBuffers()
		{
			commandBuffers.resize(swapChainFramebuffers.size());

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

			if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			{
				throw GameException("failed to allocate command buffers!");
			}

			for (size_t i = 0; i < commandBuffers.size(); ++i)
			{
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
				{
					throw GameException("failed to begin recording command buffer!");
				}

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass;
				renderPassInfo.framebuffer = swapChainFramebuffers[i];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = swapChainExtent;

				VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

				vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

				vkCmdEndRenderPass(commandBuffers[i]);

				if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				{
					throw GameException("failed to record command buffer!");
				}
			}
		}

		void VulkanRendererPrivate::CreateSyncObjects()
		{
			imageAvailableSemaphores.resize(MaxFramesInFlight);
			renderFinishedSemaphores.resize(MaxFramesInFlight);
			inFlightFences.resize(MaxFramesInFlight);

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (size_t i = 0; i < MaxFramesInFlight; ++i)
			{
				if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
					vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
					vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
				{
					throw GameException("failed to create synchronization objects for a frame!");
				}
			}
		}
#pragma endregion

#pragma region VulkanRendererPrivatePrivateMethods
		VkShaderModule VulkanRendererPrivate::CreateShaderModule(const vector<char>& code)
		{
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				throw runtime_error("failed to create shader module!");
			}
			return shaderModule;
		}

		VkSurfaceFormatKHR VulkanRendererPrivate::ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats)
		{
			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
			{
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : availableFormats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return availableFormat;
				}
			}
			return availableFormats[0];
		}

		VkPresentModeKHR VulkanRendererPrivate::ChooseSwapPresentMode(const vector<VkPresentModeKHR> availablePresentModes)
		{
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
				else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					bestMode = availablePresentMode;
				}
			}
			return bestMode;
		}

		VkExtent2D VulkanRendererPrivate::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const WindowContext& windowContext)
		{
			if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max())
			{
				return capabilities.currentExtent;
			}
			else
			{
				uint32_t width = 0, height = 0;
				serviceLocator.Get<WindowManager>().GetFrameBufferSize(windowContext, width, height);

				VkExtent2D actualExtent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};

				actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));
				return actualExtent;
			}
		}

		VulkanRendererPrivate::SwapChainSupportDetails VulkanRendererPrivate::QuerySwapChainSupport(VkPhysicalDevice deviceTemp)
		{
			SwapChainSupportDetails details;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(deviceTemp, surface, &details.capabilities);
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(deviceTemp, surface, &formatCount, nullptr);

			if (formatCount != 0)
			{
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(deviceTemp, surface, &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(deviceTemp, surface, &presentModeCount, nullptr);
			if (presentModeCount != 0)
			{
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(deviceTemp, surface, &presentModeCount, details.presentModes.data());
			}
			return details;
		}

		bool VulkanRendererPrivate::IsDeviceSuitable(VkPhysicalDevice deviceTemp)
		{
			QueueFamilyIndices indices = FindQueueFamilies(deviceTemp);
			bool extensionsSupported = CheckDeviceExtensionSupport(deviceTemp);
			bool swapChainAdequate = false;
			if (extensionsSupported)
			{
				SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(deviceTemp);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}
			return indices.IsComplete() && extensionsSupported && swapChainAdequate;
		}

		bool VulkanRendererPrivate::CheckDeviceExtensionSupport(VkPhysicalDevice deviceTemp)
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(deviceTemp, nullptr, &extensionCount, nullptr);
			vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(deviceTemp, nullptr, &extensionCount, availableExtensions.data());

			set<string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
			for (const auto& extension : availableExtensions)
			{
				requiredExtensions.erase(extension.extensionName);
			}
			return requiredExtensions.empty();
		}

		VulkanRendererPrivate::QueueFamilyIndices VulkanRendererPrivate::FindQueueFamilies(VkPhysicalDevice deviceTemp)
		{
			QueueFamilyIndices indices;
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(deviceTemp, &queueFamilyCount, nullptr);
			vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(deviceTemp, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(deviceTemp, i, surface, &presentSupport);
				if (queueFamily.queueCount > 0 && presentSupport)
				{
					indices.presentFamily = i;
				}

				if (indices.IsComplete())
				{
					break;
				}
				++i;
			}
			return indices;
		}
#pragma endregion

#pragma region VulkanRendererPrivateStaticMembers
		const int VulkanRendererPrivate::MaxFramesInFlight = 2;

		const vector<const char*> VulkanRendererPrivate::ValidationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		const vector<const char*> VulkanRendererPrivate::DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef NDEBUG
		const bool VulkanRendererPrivate::EnableValidationLayers = false;
#else
		const bool VulkanRendererPrivate::EnableValidationLayers = true;
#endif

		vector<const char*> VulkanRendererPrivate::GetRequiredExtensions()
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions = nullptr;

			//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			if (EnableValidationLayers)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
			return extensions;
		}

		bool VulkanRendererPrivate::CheckValidationLayerSupport()
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : ValidationLayers)
			{
				bool layerFound = false;
				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					return false;
				}
			}
			return true;
		}

		vector<char> VulkanRendererPrivate::ReadFile(const string& filename)
		{
			ifstream file(filename, ios::ate | ios::binary);
			if (!file.is_open())
			{
				throw runtime_error("failed to open file!");
			}

			size_t fileSize = (size_t)file.tellg();
			vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}

		VkResult VulkanRendererPrivate::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}
			else
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void VulkanRendererPrivate::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(instance, callback, pAllocator);
			}
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRendererPrivate::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
			VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
		{
			cerr << "validation layer: " << pCallbackData->pMessage << endl;
			return VK_FALSE;
		}
#pragma endregion

#pragma endregion

		VulkanRenderer::VulkanRenderer(ServiceLocator& serviceLocator) :
			serviceLocator(serviceLocator),
			vulkanRendererPrivate(new (serviceLocator.Get<MemoryAllocator>().Allocate<VulkanRendererPrivate>()) VulkanRendererPrivate(serviceLocator))
		{
		}

		void VulkanRenderer::Initialize()
		{
			vulkanRendererPrivate->CreateInstance();
			vulkanRendererPrivate->SetupDebugCallback();
			vulkanRendererPrivate->CreateSurface();
			vulkanRendererPrivate->PickPhysicalDevice();
			vulkanRendererPrivate->CreateLogicalDevice();
			vulkanRendererPrivate->CreateCommandPool();
			vulkanRendererPrivate->CreateSyncObjects();
		}

		void VulkanRenderer::SetViewport(const WindowContext& windowContext)
		{
			vulkanRendererPrivate->CreateSwapChain(windowContext);
			vulkanRendererPrivate->CreateImageViews();
			vulkanRendererPrivate->CreateRenderPass();
			vulkanRendererPrivate->CreateGraphicsPipeline();
			vulkanRendererPrivate->CreateFramebuffers();
			vulkanRendererPrivate->CreateCommandBuffers();
		}

		Shader VulkanRenderer::LoadShaderSource(const string&, ShaderType)
		{
			return Shader();
		}

		Shader VulkanRenderer::LoadShaderFile(const string&, ShaderType)
		{
			return Shader();
		}

		void VulkanRenderer::DeleteShader(const Shader&)
		{
		}

		void VulkanRenderer::UseShader(const Shader&)
		{
		}

		void VulkanRenderer::UseShaders(const vector<Shader>&)
		{
		}

		GpuTextureResource* VulkanRenderer::CreateTextureResource(const TextureDescription&)
		{
			return nullptr;
		}

		bool VulkanRenderer::FreeTextureResource(GpuTextureResource&)
		{
			return false;
		}

		void VulkanRenderer::LoadMesh(const Mesh&)
		{
		}

		void VulkanRenderer::AddActorToScene(const Actor&)
		{
		}

		void VulkanRenderer::Update()
		{
		}

		void VulkanRenderer::Shutdown()
		{
		}
	}
}
