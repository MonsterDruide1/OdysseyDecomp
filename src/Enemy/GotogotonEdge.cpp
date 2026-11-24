#include "Enemy/GotogotonEdge.h"

GotogotonEdge::GotogotonEdge() {}

void GotogotonEdge::setTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void GotogotonEdge::setAxis(const sead::Vector3f& axis) {
    mAxis = axis;
}

void GotogotonEdge::setNextFace(const GotogotonFace* face) {
    mNextFace = face;
}
