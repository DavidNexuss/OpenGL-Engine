#pragma once
#include <string>
#include <cstddef>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace GUI
{

    extern std::vector<ImFont*> fonts;

    /**
     * @brief loads a font using its name and desired font size
     * @return font identifier
     */
    size_t loadFont(const std::string& fontname,int size);
    
    /**
     * @brief loads a font using its name and desired font size and loads it with a given index
     * @return font identifier
     */
    size_t loadFont(const std::string& fontname,int size,size_t id);

    void setFont(int id);

}