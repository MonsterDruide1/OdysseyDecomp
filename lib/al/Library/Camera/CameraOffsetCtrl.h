#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;

class CameraOffsetCtrl : public HioNode {
public:
    virtual void load(const ByamlIter& iter) = 0;
    virtual const sead::Vector3f& getOffset() const = 0;
};

class CameraOffsetCtrlY : public CameraOffsetCtrl {
public:
    CameraOffsetCtrlY() = default;

    void load(const ByamlIter& iter) override;
    const sead::Vector3f& getOffset() const override;

private:
    sead::Vector3f mOffset = sead::Vector3f::zero;
};

static_assert(sizeof(CameraOffsetCtrlY) == 0x18);

}  // namespace al
