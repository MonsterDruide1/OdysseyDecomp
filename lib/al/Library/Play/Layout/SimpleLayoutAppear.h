#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppear : public LayoutActor {
public:
    SimpleLayoutAppear(const char* name, const char* layoutName, const LayoutInitInfo& info,
                       const char* archiveName);

    void appear() override;
};
}  // namespace al
