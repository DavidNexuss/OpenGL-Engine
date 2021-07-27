#pragma once
#include "gui.h"
#include <ImGuiColorTextEdit/TextEditor.h>
#include <io/resource.h>

namespace GUI
{
    namespace Util
    {
        void renderEditor(TextEditor& editor);
        void openResource(TextEditor& editor,Resource& resource);

        ImGuiWindowFlags overlayFlags(int corner,float x,float y);
    }
}