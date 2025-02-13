#include "Library/Projection/Projection.h"

namespace al {

f32 Projection::getTop() const {
    return mBase.getTop();
}

f32 Projection::getBottom() const {
    return mBase.getBottom();
}

f32 Projection::getLeft() const {
    return mBase.getLeft();
}

f32 Projection::getRight() const {
    return mBase.getRight();
}

f32 Projection::getNear() const {
    return mBase.getNear();
}

f32 Projection::getFar() const {
    return mBase.getFar();
}

/*
void Projection::setProjTBLRNF(f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far) {
    mNear = near;
    mFar = far;
    mBase.setNear(near);
    mBase.setFar(far);
    mBase.setDirty();
    mBase.setTBLR(top, bottom, left, right);
    mBase.top
}*/

void Projection::setFovy(f32 fovy) {
    mFovy = fovy;
    mFocalLength = tanf(fovy * 0.5f);
}

void Projection::setAspect(f32 aspect) {
    mBase.setAspect(aspect);
    mAspect = aspect;
}

void Projection::setNear(f32 near) {
    mBase.setNear(near);
    mNear = near;
}

void Projection::setFar(f32 far) {
    mBase.setFar(far);
    mFar = far;
}

f32 Projection::calcNearClipHeight() {
    return 2 * getNear() * mFocalLength;
}

f32 Projection::calcNearClipWidth() {
    return calcNearClipHeight() * mBase.getAspect();
}

void Projection::setTop(f32 top) {
    mTop = top;
}

void Projection::setBottom(f32 bottom) {
    mBottom = bottom;
}

void Projection::setLeft(f32 left) {
    mLeft = left;
}

void Projection::setRight(f32 right) {
    mRight = right;
}

void Projection::setOffset(const sead::Vector2f& offset) {
    mBase.setOffset(offset);
    mOffset.set(offset);
}

f32 Projection::getAspect() const {
    return mBase.getAspect();
}

f32 Projection::getFovy() const {
    return mBase.getFovy();
}

const sead::Matrix44f& Projection::getProjMtx() const {
    return mBase.getDeviceProjectionMatrix();
}

const sead::Matrix44f& Projection::getProjInvMtx() const {
    return mInvMtx;
}

}  // namespace al
