#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

class WorldMapParts : public al::LiveActor {
public:
    WorldMapParts(const char* name);

    void setWorldMtx(const sead::Matrix34f& srcMtx);
    void updatePose();
    void control() override;

    virtual void setLocalMtx(const sead::Matrix34f& srcMtx);

    static void initParts(WorldMapParts* mapParts, const char* arcName,
                          const al::ActorInitInfo& initInfo, const sead::Matrix34f* worldMtx,
                          const sead::Matrix34f& localMtx, const char* suffix);
    static WorldMapParts* create(const char* name, const char* arcName,
                                 const al::ActorInitInfo& initInfo, const sead::Matrix34f* worldMtx,
                                 const sead::Matrix34f& localMtx, const char* suffix);

protected:
    const sead::Matrix34f* mWorldMtx = nullptr;
    sead::Matrix34f mLocalMtx = sead::Matrix34f::ident;
};

class WorldMapPartsFloat : public WorldMapParts {
public:
    WorldMapPartsFloat(const char* name, const sead::Vector3f& offset, s32 period, f32 amplitude);

    void control() override;
    void setLocalMtx(const sead::Matrix34f& srcMtx) override;

private:
    sead::Vector3f mTranslation = sead::Vector3f::zero;
    sead::Vector3f mUpDir = sead::Vector3f::ey;
    sead::Vector3f mFloatOffset;
    s32 mFrameCount = 0;
    s32 mPeriod;
    f32 mAmplitude;
};

static_assert(sizeof(WorldMapPartsFloat) == 0x170);
