#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;

class CameraOffsetCtrl : public HioNode {
public:
    virtual void load(const ByamlIter& iter) = 0;
    virtual f32 getOffset() const = 0;
};

}  // namespace al
