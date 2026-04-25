#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserBossBattle : public CameraPoser {
public:
    CameraPoserBossBattle(const char* name, const sead::Vector3f* axis);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void makeLookAtCamera(sead::LookAtCamera* camera) const override;
    void update() override;
    bool isEnableRotateByPad() const override;

    void setPosPtr(const sead::Vector3f* posPtr);
    bool tryChangeFollowCamera();
    bool isCameraTargetOutOfRangeY() const;
    bool tryChangeTowerCamera();
    f32 calcOutOfRangeDistance() const;
    void exeTower();
    void exeFollow();
    void endFollow();
    void exeFollowNear();

private:
    u8 _140[0x78];
};

static_assert(sizeof(CameraPoserBossBattle) == 0x1B8);

}  // namespace al
