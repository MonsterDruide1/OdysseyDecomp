#include "Enemy/GotogotonFace.h"

#include "Library/Math/MathUtil.h"

GotogotonFace::GotogotonFace() {}

GotogotonEdge* GotogotonFace::getEdge(s32 index) {
    return &mEdges[index];
}

const GotogotonEdge* GotogotonFace::getEdge(s32 index) const {
    return &mEdges[index];
}

const GotogotonFace* GotogotonFace::getNextFace(s32 index) const {
    return mEdges[index].mNextFace;
}

void GotogotonFace::setOppositeFace(const GotogotonFace* face) {
    mOppositeFace = face;
}

void GotogotonFace::setCenter(const sead::Vector3f& vector) {
    mCenter = vector;
}

void GotogotonFace::setNormal(const sead::Vector3f& vector) {
    al::tryNormalizeOrZero(&mNormal, vector);
}
