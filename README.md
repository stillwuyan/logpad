## Description
This is an log viewer based on imgui.

## How to build
### Prepare environment
1. Install msys2
   ```
   https://www.msys2.org/
   ```
2. Install toolchain
   ```
   pacman -S pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```
3. Install GLFW
   ```
   pacman -S mingw-w64-ucrt-x86_64-glfw
   ```
4. Install Vulkan SDK
   ```
   https://vulkan.lunarg.com/sdk/home
   ```

### Get source code
1. Clone repository
   ```
   git clone --recurse-submodules git@github.com:stillwuyan/logpad.git
   ```
2. Compile the project
   ```
   cd logpad
   make
   ```
3. Check build result
   ```
   ls dist/logpad.exe
   make run
   ```

### Update imgui
1. Fetch latest source code
   ```
   pushd 3rdparty/imgui
   git pull origin docking
   git checkout docking
   popd
   ```
2. Update imgui submodule version
   ```
   git add imgui
   git commit -m "some messaage"
   git push origin master
   ```

### Add submodules
1. dirent
   ```
   git submodule add -b master --name dirent https://github.com/tronkko/dirent.git 3rdparty/dirent
   ```
2. imgui
   ```
   git submodule add -b docking --name imgui https://github.com/ocornut/imgui.git 3rdparty/imgui
   ```
3. json
   ```
   git submodule add -b master --name json https://github.com/nlohmann/json.git 3rdparty/json
   ```
