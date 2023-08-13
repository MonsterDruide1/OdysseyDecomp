#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class ByamlSave : public al::HioNode {
public:
    virtual void write(al::ByamlWriter*) = 0;
    virtual void read(const al::ByamlIter&) = 0;
};
