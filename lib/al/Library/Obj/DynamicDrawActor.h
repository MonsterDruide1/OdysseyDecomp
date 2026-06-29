#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {

class IUseFinalize {
public:
    virtual void finalize() = 0;
};

class DynamicDrawActor : public LiveActor, public IUseFinalize {
public:
    enum DrawCategory {
        DrawCategory_Default = 0,
        DrawCategory_Forward = 1,
        DrawCategory_Indirect = 2,
        DrawCategory_PreSilhouette = 3,
    };

    DynamicDrawActor(const ActorInitInfo& initInfo, s32 vertexNum, const char* archiveName,
                     DrawCategory drawCategory, bool isDrawDepthShadow);

    void drawDepthShadow() const;
    void draw() const override;
    void finalize() override;
    void setupHio();
    void begin();
    void normal(const sead::Vector3f& normal);
    void color(const sead::Color4f& color);
    void texCoord(const sead::Vector2f& texCoord, s32 index);
    void tangent(const sead::Vector4f& tangent);
    void weight(const sead::Vector4f& weight);
    void skinId(const sead::Vector4u& skinId);
    void vertex(const sead::Vector3f& position);
    void end();
    void beginModify();
    void normal(u32 index, const sead::Vector3f& normal);
    void color(u32 index, const sead::Color4f& color);
    void texCoord(u32 vertexIndex, const sead::Vector2f& texCoord, s32 coordIndex);
    void tangent(u32 index, const sead::Vector4f& tangent);
    void weight(u32 index, const sead::Vector4f& weight);
    void skinId(u32 index, const sead::Vector4u& skinId);
    void vertex(u32 index, const sead::Vector3f& position);
    void endModify();
    void movement() override;

private:
    u8 mDynamicDrawActorStorage[0x70];
};

static_assert(sizeof(DynamicDrawActor) == 0x180);

}  // namespace al
