#pragma once
#include "gui.h"
#include "unit.h"
#include <imguiText/TextEditor.h>
#include <io/resource.h>

namespace GUI
{
    namespace Extra
    {
        /**
         * @class EngineTextEditor using TextEditor class
         * Permits user to edit a resource using the the GUI interface TextEditor
         */
        class EngineTextEditor : public GuiUnitObject
        {
            std::shared_ptr<TextEditor> editor;
            
            public:
            EngineTextEditor();
            virtual bool render() override;
            void openResource(Resource& resource);
        };
        
        /**
         * @brief Returns ImGuiWindowFlags for creating an overlay window given its position and its corner anchor
         * @returns the flags
         */
        ImGuiWindowFlags overlayFlags(int corner,float x,float y);
    }
}
