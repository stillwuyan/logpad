#ifndef FILE_DIALOG_HPP
#define FILE_DIALOG_HPP

#include <string>
#include <functional>
#include <imgui/imgui.h>
#include <imgui-addons/FileBrowser/ImGuiFileBrowser.h>

class FileDialog
{
public:
    FileDialog(std::function<void(const std::string&)>&& proc)
    : _proc(std::forward<decltype(proc)>(proc)) {}
    ~FileDialog() = default;

    bool Popup()
    {
        if (!ImGui::IsPopupOpen("Open File"))
        {
            ImGui::OpenPopup("Open File");
        }

        if (_file_dialog.showFileDialog("Open File", 
                                        imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                        ImVec2(800, 600),
                                        ".log,old,.bin"))
        {
            _proc(_file_dialog.selected_path);
        }

        return ImGui::IsPopupOpen("Open File");
    }

private:
    imgui_addons::ImGuiFileBrowser _file_dialog;
    std::function<void(const std::string&)> _proc;
};

#endif  // FILE_DIALOG_HPP