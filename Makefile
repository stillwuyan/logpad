#CXX = g++
#CXX = clang++

EXE = logpad
OUTPUT_DIR = dist

IMGUI_DIR = 3rdparty/imgui
WINDOWS_DIR = src/windows
INPUT_DIR = src/sources
JSON_DIR = 3rdparty/json
DIRENT_DIR = 3rdparty/dirent
FILE_BROWSER_DIR = 3rdparty/file-browser

SOURCES = $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += src/main.cpp
SOURCES += $(WINDOWS_DIR)/mainwindow.cpp
SOURCES += $(WINDOWS_DIR)/searchwindow.cpp
SOURCES += $(INPUT_DIR)/filefilter.cpp
SOURCES += $(INPUT_DIR)/filereader.cpp
SOURCES += $(FILE_BROWSER_DIR)/ImGuiFileBrowser.cpp

CFLAGS += -I$(IMGUI_DIR)
CFLAGS += -I$(FILE_BROWSER_DIR)
CFLAGS += -I$(DIRENT_DIR)/include
CFLAGS += -I$(JSON_DIR)/include
CFLAGS += -Isrc
CFLAGS += -g -Wall -Wformat

LIBS =

# Options: gl3, vulkan
USE_BACKEND = vulkan
# USE_BACKEND = gl3

ifeq ($(USE_BACKEND), gl3)
	include gl3.mk
	LIBS += $(LIBS_GL3)
	CFLAGS += $(CFLAGS_GL3)
	CFLAGS += -D USE_GL3
endif

ifeq ($(USE_BACKEND), vulkan)
	include vulkan.mk
	LIBS += $(LIBS_VULKAN)
	CFLAGS += $(CFLAGS_VULKAN)
	CFLAGS += -D USE_VULKAN
endif

CXXFLAGS = -std=c++20 $(CFLAGS)

OBJS = $(addprefix $(OUTPUT_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

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

$(OUTPUT_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(IMGUI_BACKEND_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.o:$(FILE_BROWSER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: dist $(OUTPUT_DIR)/$(EXE) 
	@echo Build complete for $(ECHO_MESSAGE)

dist:
	mkdir dist

$(OUTPUT_DIR)/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

run: dist $(OUTPUT_DIR)/$(EXE)
	@cd dist/ && ./logpad.exe

debug: dist $(OUTPUT_DIR)/$(EXE)
	@cd dist/ && gdb logpad.exe

clean:
	rm -f $(OUTPUT_DIR)/$(EXE) $(OUTPUT_DIR)/*.o

hehe:
	echo ${VK_SDK_PATH}
