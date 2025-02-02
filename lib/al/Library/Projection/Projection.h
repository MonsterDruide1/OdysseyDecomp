#pragma once

#include <gfx/seadProjection.h>
#include <math/seadMatrix.h>

namespace al {

class Projection : public sead::PerspectiveProjection {
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
    // const sead::Vector2f& getOffset() const; // compiler warnings...
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjInvMtx() const;

    const sead::Matrix44f& getMtxStd() const { return mMtxStd; }

private:
    char _8C[16];
    const sead::Matrix44f& _C0;
    const sead::Matrix44f& _100;
    const sead::Matrix44f& mMtxStd;
    const sead::Matrix44f& _180;
    f32 _1C0;
    f32 _1C4;
    f32 _1C8;
    f32 _1CC;
    f64 _1D0;
    f64 _1D8;
    f64 _1DC;
    f64 _1E4;
};

}  // namespace al
