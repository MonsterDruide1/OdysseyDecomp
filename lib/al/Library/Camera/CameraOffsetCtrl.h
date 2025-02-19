#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;

class CameraOffsetCtrl : public HioNode {
public:
    CameraOffsetCtrl();

    virtual void load(const ByamlIter& iter);
};

}  // namespace al
