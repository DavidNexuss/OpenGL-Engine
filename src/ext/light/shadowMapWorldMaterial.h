#include "world_material.h"
#include "framebuffer.h"
class ShadowMapWorldMaterial : public WorldMaterial {

    FrameBufferID shadowMapFramebuffer;
    public:
    ShadowMapWorldMaterial(FrameBufferID _shadowMapFramebuffer) : shadowMapFramebuffer(_shadowMapFramebuffer) { }
    inline void bind(MaterialID material) {
        if(material->uniforms[Standard::uShadowMap] != Standard::glInvalid) {
            Texture shadowMap = shadowMapFramebuffer->getDepthBuffer();
            TextureLoader::useTexture(shadowMap,Standard::tShadowMap,GL_TEXTURE_2D);
            glUniform1i(material->uniforms[Standard::uShadowMap],Standard::tShadowMap);
        }
    }
};