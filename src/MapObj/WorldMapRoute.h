#pragma once

#include "MapObj/WorldMapParts.h"

class WorldMapRoute : public WorldMapParts {
public:
    WorldMapRoute* create(const char*, const char*, const al::ActorInitInfo&,
                          const sead::Matrix34f*, f32);
    WorldMapRoute(const char*);

    void calcPose(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                  const sead::Vector3f&, f32, bool, bool, bool, bool);
    void appear();
    void kill();
    void movement();
    void calcAnim();
    void openRoute(s32);
    void exeWait();
    void exeOpen();

private:
    sead::Vector3f mJointEnd;
    sead::Vector3f mJointMiddle;
    sead::Vector3f field_158;
};
