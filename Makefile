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

#CXX = g++
#CXX = clang++

EXE = logpad
OUTPUT_DIR = dist

FRAMEWORK_DIR = src/framework
WINDOWS_DIR = src/windows
INPUT_DIR = src/input
GLFW_DIR = src/platform/glfw

IMGUI_DIR = imgui
GL3W_DIR = imgui/examples/libs/gl3w
GLAD_DIR = imgui/examples/libs/glad
ADDONS_DIR = imgui-addons/FileBrowser

SOURCES = src/main.cpp
SOURCES += $(WINDOWS_DIR)/mainwindow.cpp
SOURCES += $(WINDOWS_DIR)/searchwindow.cpp
SOURCES += $(INPUT_DIR)/filereader.cpp
SOURCES += $(GLFW_DIR)/glfwbackend.cpp

SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(ADDONS_DIR)/ImGuiFileBrowser.cpp

OBJS = $(addprefix $(OUTPUT_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(ADDONS_DIR) -Isrc -I.
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## OPENGL LOADER / OPENGL ES
##---------------------------------------------------------------------

## See below for OpenGL ES option (no loader required) - comment out
## the following if you want to use OpenGL ES instead of Desktop GL.

## Using OpenGL loader: gl3w [default]
SOURCES += $(GL3W_DIR)/GL/gl3w.c
CXXFLAGS += -I$(GL3W_DIR) -DIMGUI_IMPL_OPENGL_LOADER_GL3W

## Using OpenGL loader: glew
## (This assumes a system-wide installation)
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_GLEW
# LIBS += -lGLEW

## Using OpenGL loader: glad
# SOURCES += ../libs/glad/src/glad.c
# CXXFLAGS += -I../libs/glad/include -DIMGUI_IMPL_OPENGL_LOADER_GLAD

## Using OpenGL loader: glad2
# SOURCES += ../libs/glad/src/gl.c
# CXXFLAGS += -I../libs/glad/include -DIMGUI_IMPL_OPENGL_LOADER_GLAD2

## Using OpenGL loader: glbinding
## This assumes a system-wide installation
## of either version 3.0.0 (or newer)
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_GLBINDING3
# LIBS += -lglbinding
## or the older version 2.x
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_GLBINDING2
# LIBS += -lglbinding

## Using OpenGL ES, no loader required
## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
	CXXFLAGS += -std=c++20
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
	CXXFLAGS += -std=c++20
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
	CXXFLAGS += -std=c++20
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.cpp : %.hpp

$(OUTPUT_DIR)/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(WINDOWS_DIR)/%.cpp $(WINDOWS_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(INPUT_DIR)/%.cpp $(INPUT_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(GLFW_DIR)/%.cpp $(GLFW_DIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(GL3W_DIR)/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(GLAD_DIR)/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(ADDONS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -Wno-unused-function -c -o $@ $<

all: dist $(OUTPUT_DIR)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

dist:
	mkdir dist

$(OUTPUT_DIR)/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

run: dist $(OUTPUT_DIR)/$(EXE)
	dist/logpad.exe

clean:
	rm -f $(OUTPUT_DIR)/$(EXE) $(OUTPUT_DIR)/*.o
