#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace sead {
class LookAtCamera;
}  // namespace sead

namespace al {
class ByamlIter;
class CameraPoser;

class CameraParamMoveLimit {
public:
    static CameraParamMoveLimit* create(const CameraPoser* poser);

    CameraParamMoveLimit();

    void load(const ByamlIter& iter);
    void apply(sead::LookAtCamera* camera) const;

private:
    sead::Vector3f mPlus = {100000.0f, 100000.0f, 100000.0f};
    sead::Vector3f mMinus = {-100000.0f, -100000.0f, -100000.0f};
    bool mHasPlusX = false;
    bool mHasMinusX = false;
    bool mHasPlusY = false;
    bool mHasMinusY = false;
    bool mHasPlusZ = false;
    bool mHasMinusZ = false;
    sead::Matrix34f mViewMtx = sead::Matrix34f::ident;
    sead::Matrix34f mInvViewMtx = sead::Matrix34f::ident;
    f32 mRotYDegree = 0;
};

static_assert(sizeof(CameraParamMoveLimit) == 0x84);

}  // namespace al
