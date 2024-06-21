#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWaitEnd : public LayoutActor {
public:
    SimpleLayoutAppearWaitEnd(const char*, const char*, const LayoutInitInfo&, const char*, bool);
    SimpleLayoutAppearWaitEnd(LayoutActor*, const char*, const char*, const LayoutInitInfo&,
                              const char*);

    void appear() override;
    void end();
    void startWait();

    void exeAppear();
    void exeWait();
    void exeEnd();
    bool isWait() const;
    bool isAppearOrWait() const;

private:
    s32 mLifetime = -1;
};
}  // namespace al
