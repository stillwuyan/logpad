#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

IMGUI_DIR = 3rdparty/imgui
IMGUI_BACKEND_DIR = 3rdparty/imgui/backends

GLFW_DIR = src/backend/glfw
GL3W_DIR = 3rdparty/imgui/examples/libs/gl3w
GLAD_DIR = 3rdparty/imgui/examples/libs/glad

SOURCES += $(GLFW_DIR)/gl3backend.cpp
SOURCES += $(IMGUI_BACKEND_DIR)/imgui_impl_glfw.cpp $(IMGUI_BACKEND_DIR)/imgui_impl_opengl3.cpp

UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CFLAGS_GL3 = -I$(IMGUI_BACKEND_DIR)
LIBS_GL3 =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS_GL3 += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`
	CFLAGS_GL3 += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS_GL3 += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS_GL3 += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	LIBS_GL3 += -lglfw
	CFLAGS_GL3 += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS_GL3 += -lglfw3 -lgdi32 -lopengl32 -limm32
	CFLAGS_GL3 += $(shell pkg-config --cflags glfw3)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OUTPUT_DIR)/%.o:$(GLFW_DIR)/%.cpp $(GLFW_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(GL3W_DIR)/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(GLAD_DIR)/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<