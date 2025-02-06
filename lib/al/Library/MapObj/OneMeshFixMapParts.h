#pragma once

#include "Library/MapObj/FixMapParts.h"

namespace al {
class OneMeshFixMapParts : public FixMapParts {
public:
    OneMeshFixMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
};
}  // namespace al
