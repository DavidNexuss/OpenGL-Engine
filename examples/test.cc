#include <iostream>
#include <engine.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gui/gui_ext.h>
#include <light.h>
#include <gui/debug/gui_debug.h>
#include <mesh/sphereMesh.h>
#include <mesh/voxel/voxelMesh.h>
#include <mesh/voxel/hexelMesh.h>
#include <mesh/multiResPlane.h>

#include <obj.h>
#include <mesh_builder.h>
#include <debug.h>


using namespace std;
using namespace glm;


float randf()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
Mesh createQuad()
{
    return PrimitiveMesh::generateFromBuffers({
        {Standard::aPosition,2,{
            -1.0,1.0,
             1.0,1.0,
            -1.0,-1.0,

             1.0,1.0,
             1.0,-1.0,
             -1.0,-1.0,

        }}
    });
}
void loadSpecificWorld()
{
    
    auto reflectiveMaterial = MaterialLoader::loadMaterial(Material("ushader_tiled",{"ka","kd","ks","specularStrength","reflectionStrength","tileStep","tileIndex"}));
    auto screen = MaterialLoader::loadMaterial(Material("screen"));

    TextureData tile("tileset.png");
    glm::vec2 tileStep = glm::vec2(16,16) / glm::vec2(tile.width,tile.height);
    auto reflectiveMaterialInstance = MaterialInstanceLoader::loadMaterialInstance(
        MaterialInstance({
            glm::vec3(0.4),glm::vec3(0.4),glm::vec3(0.4),50.0f,0.2f,tileStep,glm::vec2(0.0f)
        }));

    
    MaterialInstanceLoader::materialInstances[reflectiveMaterialInstance].setTexture(TextureLoader::loadTexture(tile,false),2);
    
    /*VoxelMeshWorld voxelWorld;
    voxelWorld.at(0,0,0) = 1;
    voxelWorld.at(1,0,0) = 1;
    voxelWorld.at(2,0,0) = 1;
    voxelWorld.at(2,1,0) = 1;
    voxelWorld.at(2,2,0) = 1;
    voxelWorld.at(2,3,0) = 1;
    voxelWorld.at(2,3,1) = 1;
    voxelWorld.at(2,3,2) = 1;
    voxelWorld.at(2,2,2) = 1;
    voxelWorld.at(1,2,2) = 1;
    
    auto result = voxelWorld.rasterize();
    std::cout << result.size() << std::endl;
    
    Model cube = Model(MeshLoader::loadMesh(result[0].mesh));*/

    HexelMeshGenerator meshGenerator;
    meshGenerator.create();
    meshGenerator.addHex(0,0,0);
    meshGenerator.addHex(1,0,0);
    meshGenerator.addHex(0,1,1);
    
    
    Model cube = Model(MeshLoader::loadMesh(meshGenerator.generateMesh()));
    cube.materialID = reflectiveMaterial;
    cube.materialInstanceID = reflectiveMaterialInstance;
    Loader::loadModel(std::move(cube));
    ModelLoader::loadModel(std::move(cube));

    Renderer::worldMaterial.setSkyBox(TextureLoader::loadCubemap({
        TextureData("canyon/right.png"),
        TextureData("canyon/left.png"),
        TextureData("canyon/top.png"),
        TextureData("canyon/bottom.png"),
        TextureData("canyon/front.png"),
        TextureData("canyon/back.png")
    }));

    ModelGroup donut = loadMeshFromFile("donut.obj",reflectiveMaterial);
    donut[0].materialInstanceID = reflectiveMaterialInstance;
    donut[0].transformMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,5.0));
    ModelLoader::loadModel(donut);

/*
    MeshID sphere = MeshLoader::loadMesh(SphereMesh(2.0,64,64).generateMesh());
    Model sphereModel = Model(sphere,cube.materialID);
    sphereModel.materialInstanceID = cube.materialInstanceID;
    ModelLoader::loadModel(sphereModel);
*/

}

void loadPlaneWorld()
{
    auto reflectiveMaterial = MaterialLoader::loadMaterial(Material("heightmap",
    {"ka","kd","ks","specularStrength","reflectionStrength","smoothStep","hieghtScale"}));
    
    auto reflectiveMaterialInstance = MaterialInstanceLoader::loadMaterialInstance(
        MaterialInstance({
            glm::vec3(0.4),glm::vec3(0.4),glm::vec3(0.4),50.0f,0.2f,0.5f,1.0f
        }));

    TextureData height("height.png");
    MaterialInstanceLoader::materialInstances[reflectiveMaterialInstance].setTexture(TextureLoader::loadTexture(height,false),0);

     Renderer::worldMaterial.setSkyBox(TextureLoader::loadCubemap({
        TextureData("canyon/right.png"),
        TextureData("canyon/left.png"),
        TextureData("canyon/top.png"),
        TextureData("canyon/bottom.png"),
        TextureData("canyon/front.png"),
        TextureData("canyon/back.png")
    }));

    Model plane = Model(MeshLoader::loadMesh(MultiResPlane(200).generateMesh()));
    plane.transformMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,-0.5,-0.5f));
    plane.materialID = reflectiveMaterial;
    plane.materialInstanceID = reflectiveMaterialInstance;
    ModelLoader::loadModel(std::move(plane));

    Light::load(glm::vec3(8.0),glm::vec3(1.0,1.0,1.0));
}


int main(int argc, char** argv)
{
    Engine::createEngine("Test Engine",{
        .mssaLevel = 4,
        .openglMajorVersion = 4,
        .openglMinorVersion = 5,
        .windowWidth = 1024,
        .windowHeight = 768
    });

    //loadSpecificWorld();
    loadPlaneWorld();
    GUI::addUnit(GUI::makeSimpleGuiUnit([](){
        bool window = true;
        GUI::Debug::renderModelTree(&window);
        GUI::Debug::renderMaterialInstanceMenu(0,0,nullptr);
        return true;
    }));


    GUI::addUnit(GUI::makeSimpleGuiUnit([&](){
        
        static int corner = 0;
        static bool p_open = true;
        if(p_open)
        {
            ImGui::Begin("FrameRate",&p_open, GUI::Extra::overlayFlags(0,100,0));
            ImGui::Text((string("Framerate:  ") + to_string(Debug::getFrameRate())).c_str());
            ImGui::End();
        }
        return true;
    }));
    /*
    Renderer::setRenderPipeline(
        (new RenderNode(MaterialLoader::loadMaterial(Material("screen")),1))->setChildren({new RenderNode(1)}));
*/
   // Renderer::addPostProcess(MaterialLoader::loadMaterial(Material("screen")));
/*
    GUI::Extra::EngineTextEditor* editor = new GUI::Extra::EngineTextEditor();
    Resource text = Resource(ResourceHeader::fromFile("res/materials/base_vertex.glsl"));
    editor->openResource(text);
    GUI::addUnit(std::shared_ptr<GUI::GuiUnitObject>((GUI::GuiUnitObject*)editor));
*/
    return Engine::renderLoop();
}
