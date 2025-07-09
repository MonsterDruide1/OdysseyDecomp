#pragma once


#include "gfx/seadColor.h"
#include "math/seadVector.h"

namespace al {
    class EffectLight {
    public:
        void set(const sead::Vector3<float>& pos, float intensity, const sead::Color4f& color, bool enabled);
    private:
    
        sead::Vector3<float> mPosition; 
        float mIntensity;       
        sead::Color4f mColor;   
        bool mEnabled;

    };
}