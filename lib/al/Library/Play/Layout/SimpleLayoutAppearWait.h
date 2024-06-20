#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWait : public al::LayoutActor {
public:
    SimpleLayoutAppearWait(const char*, const char*, const al::LayoutInitInfo&, const char*);
    SimpleLayoutAppearWait(al::LayoutActor*, const char*, const char*, const al::LayoutInitInfo&,
                           const char*);

    void appear() override;

    bool isWait() const;
    void exeAppear();
    void exeWait();
};
}  // namespace al
