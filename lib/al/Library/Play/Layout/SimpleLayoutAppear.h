#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppear : public al::LayoutActor {
public:
    SimpleLayoutAppear(const char*, const char*, const al::LayoutInitInfo&, const char*);

    void appear() override;
};
}  // namespace al
