#pragma once

namespace GUI
{
    namespace Debug
    {
        void renderUniformMenu(const char* uniformName,int uniformID,int materialInstanceID,bool* windowEnable);
        void renderUniformWidget(const char* uniformName,int uniformID,int materialInstanceID);

        void renderMaterialInstanceMenu(int materialID,int materialInstanceID,bool* windowEnable);
        void renderMaterialInstanceWidget(int materialID,int materialInstanceID);

        void renderMaterialMenu(int materialID,bool* windowEnable);
        void renderMaterialWidget(int materialID);

        void renderMeshMenu(int meshID,bool* windowEnable);
        void renderMeshWidget(int meshID);

        void renderModelMenu(int modelID,bool* windowEnable);
        void renderModelWidget(int modelID);
        
        void renderLightMenu(int lightID,bool* windowEnable);
        void renderLightWidget(int lightID);

        void renderMaterialTree(bool* windowEnable);
        void renderModelTree(bool* windowEnable);
        void renderLightTree(bool* windowEnable);

        void loadDebugGUI();
    }
}