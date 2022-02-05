#pragma once
#include <renderer.h>
#include "material.h"
#include "model.h"
#include "material.h"
#include "mesh.h"

namespace GUI
{
    namespace Debug
    {

        //Engine units menu
        void renderUniformMenu(const char* uniformName,int uniformID,MaterialInstanceID materialInstanceID,bool* windowEnable);
        void renderUniformWidget(const char* uniformName,int uniformID,MaterialInstanceID materialInstanceID);

        void renderMaterialInstanceMenu(MaterialID materialID,MaterialInstanceID materialInstanceID,bool* windowEnable);
        void renderMaterialInstanceWidget(MaterialID materialID,MaterialInstanceID materialInstanceID);

        void renderMaterialMenu(int materialID,bool* windowEnable);
        void renderMaterialWidget(int materialID);

        void renderMeshMenu(int meshID,bool* windowEnable);
        void renderMeshWidget(int meshID);

        void renderModelMenu(int modelID,bool* windowEnable);
        void renderModelWidget(int modelID);
        
        void renderLightMenu(int lightID,bool* windowEnable);
        void renderLightWidget(int lightID);

        void renderFrameBufferWidget(int framebufferId);
        void renderFrameBufferMenu(int framebufferId,bool* windowEnable);

        void renderMaterialTree(bool* windowEnable);
        void renderModelTree(bool* windowEnable);
        void renderLightTree(bool* windowEnable);
        void renderFrameBufferTree(bool* windowEnable);
        
        //Engine global configurations menu
        
        void renderRenderConfigMenu(bool *windowEnable, RenderConfiguration& config);
        void loadDebugGUI();
    }
}
