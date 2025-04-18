/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern PFN_vkCreateCommandPool vkCreateCommandPool;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern PFN_vkCreateShaderModule vkCreateShaderModule;
extern PFN_vkCreateFence vkCreateFence;
extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkQueueSubmit vkQueueSubmit;
extern PFN_vkWaitForFences vkWaitForFences;
extern PFN_vkResetFences vkResetFences;
extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
extern PFN_vkCreateImageView vkCreateImageView;
extern PFN_vkDestroyImageView vkDestroyImageView;
extern PFN_vkCreateBuffer vkCreateBuffer;
extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
extern PFN_vkAllocateMemory vkAllocateMemory;
extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkBindBufferMemory vkBindBufferMemory;
extern PFN_vkCreateSemaphore vkCreateSemaphore;
extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern PFN_vkDestroySemaphore vkDestroySemaphore;
extern PFN_vkDestroyFence vkDestroyFence;
extern PFN_vkCreateImage vkCreateImage;
extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
extern PFN_vkBindImageMemory vkBindImageMemory;
extern PFN_vkCreateRenderPass vkCreateRenderPass;
extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdDraw vkCmdDraw;
extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkDestroyPipeline vkDestroyPipeline;
extern PFN_vkCreateEvent vkCreateEvent;
extern PFN_vkDestroyEvent vkDestroyEvent;
extern PFN_vkCmdSetEvent vkCmdSetEvent;
extern PFN_vkGetEventStatus vkGetEventStatus;
extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
extern PFN_vkCreateSampler vkCreateSampler;
extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkCmdSetViewport vkCmdSetViewport;
extern PFN_vkCmdSetScissor vkCmdSetScissor;
extern PFN_vkFreeMemory vkFreeMemory;
extern PFN_vkDestroyBuffer vkDestroyBuffer;
extern PFN_vkCmdCopyImage vkCmdCopyImage;
extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
extern PFN_vkDestroyImage vkDestroyImage;
extern PFN_vkCmdDispatch vkCmdDispatch;
extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern PFN_vkDestroySampler vkDestroySampler;
extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
extern PFN_vkCreateQueryPool vkCreateQueryPool;
extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
extern PFN_vkCmdPushConstants vkCmdPushConstants;

// Win32 Vulkan extensions.
extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
extern PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
extern PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
extern PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
extern PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
extern PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
extern PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
extern PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
extern PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
extern PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
extern PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
extern PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;

namespace traktor::render
{

bool initializeVulkanApi();

bool initializeVulkanExtensions(VkInstance instance);

void finalizeVulkanApi();

}
