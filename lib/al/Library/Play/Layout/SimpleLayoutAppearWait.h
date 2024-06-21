#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWait : public LayoutActor {
public:
    SimpleLayoutAppearWait(const char*, const char*, const LayoutInitInfo&, const char*);
    SimpleLayoutAppearWait(LayoutActor*, const char*, const char*, const LayoutInitInfo&,
                           const char*);

    void appear() override;

    bool isWait() const;
    void exeAppear();
    void exeWait();
};
}  // namespace al
