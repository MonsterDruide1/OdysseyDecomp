#pragma once

#include <heap/seadDisposer.h>

#include "Library/HostIO/HioNode.h"

namespace sead {
class Heap;
}  // namespace sead

namespace al {

class ShaderHolder : public HioNode {
    SEAD_SINGLETON_DISPOSER(ShaderHolder)
public:
    // incomplete
private:
    // missing
};

}  // namespace al
