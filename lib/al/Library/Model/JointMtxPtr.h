#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
template <typename T>
class Matrix43;
using Matrix43f = Matrix43<f32>;

class JointMtxPtr {
public:
    JointMtxPtr();

    void setNull();
    void set(const sead::Matrix34f*);
    void set(const Matrix43f*);
    void getTranslation(sead::Vector3f*) const;
    void calcMtxScale(sead::Vector3f*) const;
    void copyTo(sead::Matrix34f*) const;

private:
    const void* mMtxPtr;
    bool mIsMatrix43;
};

static_assert(sizeof(JointMtxPtr) == 0x10);
}  // namespace al
