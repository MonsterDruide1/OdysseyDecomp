#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppear : public LayoutActor {
public:
    SimpleLayoutAppear(const char*, const char*, const LayoutInitInfo&, const char*);

    void appear() override;
};
}  // namespace al
