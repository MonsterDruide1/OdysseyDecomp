#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserEntrance : public CameraPoser {
public:
    CameraPoserEntrance(const char* name);

    void initParam(f32, f32, f32, const sead::Vector3f&);
    void initParam(f32, const sead::Vector3f&, const sead::Vector3f&);
    void initLookAtPosDirect(const sead::Vector3f&);
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void movement() override;
    void update() override;

    void exeKeepByFlag();
    void exeKeepInAir();
    void exeWait();

    bool isEnableRotateByPad() const override { return true; }

private:
    s8 filler[0x178 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserEntrance) == 0x178, "al::CameraPoserEntrance size");
