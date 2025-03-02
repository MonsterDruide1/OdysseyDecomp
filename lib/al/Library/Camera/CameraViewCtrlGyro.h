#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

class GyroCameraCtrl {
public:
    GyroCameraCtrl();
    void reset(const sead::Vector3f, const sead::Vector3f&, const sead::Vector3f&);
    void update(const sead::Vector3f, const sead::Vector3f&, const sead::Vector3f&);
    void reduceSensitivity();

    void setIsValidGyro(bool isValidGyro) { mIsValidGyro = isValidGyro; }

    void setSensitivityScale(f32 sensitivityScale) { mSensitivityScale = sensitivityScale; }

private:
    sead::Matrix34f _0;
    sead::Vector3f _30;
    sead::Vector3f _3c;
    sead::Vector3f _48;
    sead::Vector3f _54;
    sead::Vector3f _60;
    sead::Vector3f _6c;
    f32 _78;
    f32 _7c;
    f32 _80;
    f32 _84;
    f32 _88;
    f32 _8c;
    f32 _90;
    f32 _94;
    f32 _98;
    s32 mSensitivityLevel;
    f32 _a0;
    f32 _a4;
    f32 mSensitivityScale;
    bool mIsValidGyro;
    f32 _b0;
    f32 _b4;
    f32 _b8;
    f32 _bc;
    f32 _c0;
    f32 _c4;
};

static_assert(sizeof(GyroCameraCtrl) == 0xC8);

}  // namespace al

