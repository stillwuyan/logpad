.PHONY : all clean debug

#CXX = g++
#CXX = clang++

EXE = logpad
OUTPUT_DIR = dist

IMGUI_DIR = 3rdparty/imgui
WINDOWS_DIR = src/windows
INPUT_DIR = src/inputs
JSON_DIR = 3rdparty/json
DIRENT_DIR = 3rdparty/dirent
FILE_BROWSER_DIR = 3rdparty/file-browser

SOURCES = $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp
SOURCES += $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(FILE_BROWSER_DIR)/ImGuiFileBrowser.cpp
SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/*/*.cpp)

CFLAGS += -I$(IMGUI_DIR)
CFLAGS += -I$(FILE_BROWSER_DIR)
CFLAGS += -I$(DIRENT_DIR)/include
CFLAGS += -I$(JSON_DIR)/include
CFLAGS += -Isrc
CFLAGS += -g -Wall -Wformat

LIBS =

# Options: gl3, vulkan
# USE_BACKEND = vulkan
USE_BACKEND = gl3

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
DEPS = $(OBJS:.o=.d)

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: dist $(DEPS) $(OUTPUT_DIR)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(OUTPUT_DIR)/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

include $(DEPS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.d: $(WINDOWS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.o: $(WINDOWS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.d: $(INPUT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.o: $(INPUT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.d: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OUTPUT_DIR)/%.d: $(FILE_BROWSER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

$(OUTPUT_DIR)/%.o: $(FILE_BROWSER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

dist:
	mkdir dist

run: all
	@cd dist/ && ./logpad.exe

debug: all
	@cd dist/ && gdb logpad.exe

clean:
	rm -f $(OUTPUT_DIR)/$(EXE) $(OUTPUT_DIR)/*.o $(OUTPUT_DIR)/*.d