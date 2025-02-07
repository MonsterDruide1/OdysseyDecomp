#pragma once

#include <gfx/seadProjection.h>
#include <math/seadMatrix.h>

namespace al {

class Projection {
public:
    Projection();
    Projection(const Projection& other);
    Projection(f32 top, f32 bottom, f32 left, f32 right);

    void init();
    void calcMtx();
    void copyFrom(const Projection&);
    f32 getTop() const;
    f32 getBottom() const;
    f32 getLeft() const;
    f32 getRight() const;
    f32 getNear() const;
    f32 getFar() const;
    void setProjTBLRNF(f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);
    void setProj(f32 top, f32 bottom, f32 left, f32 right);
    void setFovy(f32 fovy);
    void setAspect(f32 aspect);
    void setNear(f32 near);
    void setFar(f32 far);
    f32 calcNearClipHeight();
    f32 calcNearClipWidth();
    void setTop(f32 top);
    void setBottom(f32 bottom);
    void setLeft(f32 left);
    void setRight(f32 right);
    void setOffset(const sead::Vector2f& offset);
    void merge(const Projection& proj1, const Projection& proj2);
    f32 getAspect() const;
    const sead::Vector2f& getOffset() const;
    f32 getFovy() const;
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjInvMtx() const;

    const sead::Matrix44f& getMtxStd() const { return mStdMtx; }

    const sead::Projection& getProjectionSead() const { return mBase; }

private:
    sead::PerspectiveProjection mBase;
    sead::Matrix44f _BC;
    sead::Matrix44f mInvMtx;
    sead::Matrix44f mStdMtx;
    sead::Matrix44f _17C;
    f32 mLeft;
    f32 mBottom;
    f32 mNear;
    f32 mRight;
    f32 mTop;
    f32 mFar;
    f32 mFovy;
    f32 mFocalLength;
    f32 mAspect;
    sead::Vector2f mOffset;
};

}  // namespace al
