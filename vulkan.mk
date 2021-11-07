# Windows GLFW & Vulkan SDK

IMGUI_DIR = 3rdparty/imgui
IMGUI_BACKEND_DIR = 3rdparty/imgui/backends

GLFW_DIR = src/backend/glfw

SOURCES += $(GLFW_DIR)/vulkanbackend.cpp
SOURCES += $(IMGUI_BACKEND_DIR)/imgui_impl_glfw.cpp $(IMGUI_BACKEND_DIR)/imgui_impl_vulkan.cpp

CFLAGS_VULKAN = -I$(IMGUI_BACKEND_DIR)
LIBS_VULKAN =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
# 	LIBS_VULKAN += 
# 	CFLAGS_GL3 += 
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
# 	LIBS_VULKAN += 
# 	CFLAGS_VULKAN += 
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS_VULKAN += -lglfw3 -L$(VULKAN_SDK)\lib -lvulkan-1
	CFLAGS_VULKAN += -I$(VULKAN_SDK)\include $(shell pkg-config --cflags glfw3)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OUTPUT_DIR)/%.o:$(GLFW_DIR)/%.cpp $(GLFW_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<