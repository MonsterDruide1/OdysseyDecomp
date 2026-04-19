#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserBossBattle : public CameraPoser {
public:
    CameraPoserBossBattle(const char* name, const sead::Vector3f* posPtr = nullptr);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void makeLookAtCamera(sead::LookAtCamera* cam) const override;
    void update() override;
    bool isEnableRotateByPad() const override;

    void setPosPtr(const sead::Vector3f* posPtr);
    bool tryChangeFollowCamera();
    bool isCameraTargetOutOfRangeY() const;
    bool tryChangeTowerCamera();
    sead::Vector3f calcOutOfRangeDistance() const;

    void exeTower();
    void exeFollow();
    void endFollow();
    void exeFollowNear();

private:
    s8 filler[0x1B8 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserBossBattle) == 0x1B8, "al::CameraPoserBossBattle size");
