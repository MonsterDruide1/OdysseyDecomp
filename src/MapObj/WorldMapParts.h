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

private:
    const sead::Matrix34f* mWorldMtx = nullptr;
    sead::Matrix34f mLocalMtx = sead::Matrix34f::ident;
};
