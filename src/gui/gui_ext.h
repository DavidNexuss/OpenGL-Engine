#pragma once
#include "gui.h"
#include <ImGuiColorTextEdit/TextEditor.h>
#include <io/resource.h>

namespace GUI
{
    namespace Util
    {
        void displayEditor(TextEditor& editor);
        void openResource(TextEditor& editor,Resource& resource);
    }
}