#pragma once

#include "game/MapObj/WorldMapParts.h"

class WorldMapRoute : public WorldMapParts {
public:
    WorldMapRoute* create(const char*, const char*, const al::ActorInitInfo&,
                          const sead::Matrix34<float>*, float);
    WorldMapRoute(char const*);

    void calcPose(const sead::Vector3<float>&, const sead::Vector3<float>&,
                  const sead::Vector3<float>&, const sead::Vector3<float>&, float, bool, bool, bool,
                  bool);
    void appear();
    void kill();
    void movement();
    void calcAnim();
    void openRoute(int);
    void exeWait();
    void exeOpen();

private:
    sead::Vector3f mJointEnd;
    sead::Vector3f mJointMiddle;
    sead::Vector3f field_158;
};
