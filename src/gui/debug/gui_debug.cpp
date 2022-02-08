#include "gui_debug.h"
#include <gui/gui.h>
#include <material_instance.h>
#include <uniform.h>
#include <material.h>
#include <model.h>
#include <light.h>
#include <list>
#include <framebuffer.h>

namespace GUI
{
    namespace Debug
    {
        void renderUniformMenu(const char* uniformName,int uniformID,int materialInstanceID,bool* windowEnable)
        {
            if(ImGui::Begin("UniformWidget",windowEnable)) {
                renderUniformWidget(uniformName,uniformID,materialInstanceID);
            }
            ImGui::End();
        }

        void renderUniformWidget(const char* uniformName,int uniformID,MaterialInstanceID materialInstance)
        {
            auto& uniform = materialInstance->getUniform(uniformID);

            const char* variableName = uniformName ? uniformName : "value";
            switch(uniform.type)
            {
                case UniformType::INT:      
                    ImGui::InputInt(variableName,&uniform.INT);
                    break;
                case UniformType::BOOL:     
                    ImGui::Checkbox(variableName,&uniform.BOOL);
                    break;
                case UniformType::FLOAT:    
                    ImGui::InputFloat(variableName,&uniform.FLOAT);
                    break;
                case UniformType::MAT2:  
                    ImGui::Text(variableName);   
                    ImGui::InputFloat2("0",&uniform.MAT2[0].x);
                    ImGui::InputFloat2("1",&uniform.MAT2[1].x);
                    break;
                case UniformType::MAT3:     
                    ImGui::Text(variableName); 
                    ImGui::InputFloat3("0",&uniform.MAT3[0].x);
                    ImGui::InputFloat3("1",&uniform.MAT3[1].x);
                    ImGui::InputFloat3("2",&uniform.MAT3[2].x);
                    break;
                case UniformType::MAT4:     
                    ImGui::Text(variableName); 
                    ImGui::InputFloat4("0",&uniform.MAT4[0].x);
                    ImGui::InputFloat4("1",&uniform.MAT4[1].x);
                    ImGui::InputFloat4("2",&uniform.MAT4[2].x);
                    ImGui::InputFloat4("3",&uniform.MAT4[3].x);
                    break;
                case UniformType::VEC2:
                    ImGui::InputFloat2(variableName,&uniform.VEC2.x); 
                    break;
                case UniformType::VEC3:
                    ImGui::ColorPicker3(variableName,&uniform.VEC4.x); 
                    break;
                case UniformType::VEC4:     
                    ImGui::ColorPicker4(variableName,&uniform.VEC4.x); 
                    break;
            }
        }

        void renderMaterialInstanceMenu(MaterialID materialID,MaterialInstanceID materialInstanceID,bool* windowEnable)
        {
            if(ImGui::Begin("MaterialInstanceWidget",windowEnable)) {
                renderMaterialInstanceWidget(materialID,materialInstanceID);
            }
            ImGui::End();
        }
        void renderMaterialInstanceWidget(MaterialID material,MaterialInstanceID materialInstance)
        {
            for (size_t i = 0; i < material->uniformNames.size(); i++)
            {
                const char* uniformName = material->uniformNames[i].c_str();
                renderUniformWidget(uniformName,i,materialInstance);   
            }
        }

        void renderMaterialMenu(int materialID,bool* windowEnable)
        {

        }
        void renderMaterialWidget(int materialID)
        {

        }

        void renderMeshMenu(int meshID,bool* windowEnable)
        {

        }
        void renderMeshWidget(int meshID)
        {

        }

        void renderModelMenu(int modelID,bool* windowEnable)
        {
            if(ImGui::Begin("ModelWidget",windowEnable)) {
                renderModelWidget(modelID);
            }
            ImGui::End();
        }
        void renderModelWidget(int modelID)
        {
            Model& model = Loader::models[modelID];
            ImGui::InputInt("meshID",(int*)&model.meshID);
            ImGui::InputInt("materialID",(int*)&model.materialID);
            ImGui::InputInt("materialInstanceID",(int*)&model.materialInstanceID);
            
            ImGui::Text("TransformMatrix"); 
            ImGui::InputFloat4("0",&model.transformMatrix[0].x);
            ImGui::InputFloat4("1",&model.transformMatrix[1].x);
            ImGui::InputFloat4("2",&model.transformMatrix[2].x);
            ImGui::InputFloat4("3",&model.transformMatrix[3].x);

            ImGui::Checkbox("DepthMask",&model.depthMask);
            ImGui::Checkbox("CullFrontFace",&model.cullFrontFace);
        }
        
        void renderLightMenu(int lightID,bool* windowEnable)
        {
            if(ImGui::Begin("LightWidget",windowEnable)) {
                renderLightWidget(lightID);
            }
            ImGui::End();
        }
        void renderLightWidget(int lightID)
        {
            glm::vec3* lightColor = &Loader::lights.lightsColor[lightID];
            glm::vec3* lightPosition = &Loader::lights.lightsPositions[lightID];

            ImGui::InputFloat3("position",&lightPosition->x);
            ImGui::ColorPicker3("color",&lightColor->x);
        }

        template <typename T>
        void renderTree(const char* treeName,bool* window,storage<T>& vec,void (*func)(int,bool*))
        {
            std::vector<int> windows;

            if(ImGui::Begin(treeName,window))
            {
                for (size_t i = 0; i < vec.size(); i++)
                {
                    //ImGui::TreeNode(modelVector[i].objectName.c_str());
                    ImGui::Checkbox(vec[i].name.c_str(),(bool*)&vec[i].editMenu);
                    //ImGui::TreePop();
                    if(vec[i].editMenu) windows.push_back(i);
                }
            }
            ImGui::End();

            for(const auto el : windows)  {
                renderModelMenu(el,(bool*)&vec[el].editMenu);
            }
        }


        void renderFrameBufferWidget(FrameBufferID framebuffer) {

            int width = 1920;
            int height = framebuffer->getHeight();
            ImVec2 displaySize = ImVec2(width/4, height/4);

            if (ImGui::TreeNode("Depth buffer")) {
                ImGui::Image((void*)(intptr_t)framebuffer->stencilDepthBuffer,displaySize);
            }

            if (ImGui::TreeNode("Color attachments")) {
                for (size_t i = 0; i < framebuffer->colorAttachments.size(); i++)
                {
                    if (ImGui::TreeNode(("Attachment " + std::to_string(i)).c_str())) {
                        ImGui::Image((void*)(intptr_t)framebuffer->colorAttachments[i],displaySize);       
                    }
                }
            }
        }
        void renderFrameBufferMenu(int framebufferId,bool* windowEnable) {
            if(ImGui::Begin("frameBuffer",windowEnable)) {
                renderFrameBufferWidget(framebufferId);
            }
            ImGui::End();
        }

        void renderMaterialInstanceMenu(bool* windowEnabled)
        {

        }
        void renderMaterialTree(bool* windowEnable)
        {
            renderTree("MaterialTree",windowEnable,Loader::materials,renderMaterialMenu);
        }
        void renderModelTree(bool* windowEnable)
        {
            renderTree("ModelTree",windowEnable,Loader::models,renderModelMenu);
        }
        void renderLightTree(bool* windowEnable)
        {
//            renderTree("LightTree",windowEnable,Light::lightsComponent,renderLightMenu);
        }

        void renderFrameBufferTree(bool* windowEnable)
        {
            if(ImGui::Begin("frameBufferTree",windowEnable)) {
                
                for (size_t i = 0; i < Loader::framebuffers.size(); i++) {
                    
                    renderFrameBufferWidget(i);
                }
                
            }
            ImGui::End();
        }
        void loadDebugGUI()
        {

        }

        void renderRenderConfigMenu(bool *windowEnable, RenderConfiguration& config) {
            if(ImGui::Begin("RenderConfiguration", windowEnable)) {
                ImGui::Checkbox("useWiredRendering",&config.wireRendering);
                ImGui::ColorPicker3("clearColor",&config.clearColor.x);
                ImGui::Checkbox("skipSkyBox",&config.skipSkybox);
                ImGui::InputInt("mssaLevel",&config.mssaLevel);
            }
            ImGui::End();
        }
    }
}
