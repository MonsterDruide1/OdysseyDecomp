#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

#include "Library/Matrix/MatrixUtil.h"

namespace al {

class FrustumRadar {
public:
    FrustumRadar();
    void calcFrustumArea(const sead::Matrix34f&, f32, f32, f32, f32);
    void setLocalAxis(const sead::Matrix34f&);
    void setFactor(f32, f32);
    void calcFrustumArea(const sead::Matrix34f&, const sead::Matrix44f&, f32, f32);
    void setFactor(const sead::Matrix44f&);
    void calcFrustumAreaStereo(const sead::Matrix34f&, const sead::Matrix34f&,
                               const sead::Matrix44f&, f32, f32);
    void setLocalAxisStereo(const sead::Matrix34f&, const sead::Matrix34f&);
    void setFactorStereo(const sead::Matrix44f&);
    bool judgeInLeft(const sead::Vector3f&, f32) const;
    bool judgeInRight(const sead::Vector3f&, f32) const;
    bool judgeInTop(const sead::Vector3f&, f32) const;
    bool judgeInBottom(const sead::Vector3f&, f32) const;
    bool judgeInArea(const sead::Vector3f&, f32, f32, f32) const;
    bool judgeInArea(const sead::Vector3f&, f32, f32) const;
    bool judgeInArea(const sead::Vector3f&, f32) const;
    bool judgeInAreaNoFar(const sead::Vector3f&, f32) const;
    bool judgePointFlag(const sead::Vector3f&, f32, f32) const;
    bool judgeInAreaObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32, f32) const;
    bool judgeInAreaObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32) const;
    bool judgeInAreaObb(const sead::Matrix34f*, const sead::BoundBox3f&);
    bool judgeInAreaObbNoFar(const sead::Matrix34f*, const sead::BoundBox3f&) const;

private:
    sead::Vector3f mOrthoSide;
    sead::Vector3f mOrthoUp;
    sead::Vector3f mOrthoFront;
    sead::Vector3f mOrthoTrans;
    f32 _30;
    f32 _34;
    f32 _38;
    f32 _3c;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4c;
    f32 _50;
    f32 _54;
    f32 _58;

};

}  // namespace al
