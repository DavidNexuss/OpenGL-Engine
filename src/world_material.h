#pragma once
#include "material.h"

struct WorldMaterial {
    bool _needsFrameUpdate = false;

    virtual void bind(MaterialID currentMaterial) = 0;
    virtual void update() { }
    
    inline void setFrameUpdate(bool p_needsFrameUpdate) {
        _needsFrameUpdate = p_needsFrameUpdate;
    }

    inline bool needsFrameUpdate() const { return _needsFrameUpdate; }
};
