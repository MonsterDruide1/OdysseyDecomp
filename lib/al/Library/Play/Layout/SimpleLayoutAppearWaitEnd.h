#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWaitEnd : public al::LayoutActor {
public:
    SimpleLayoutAppearWaitEnd(const char*, const char*, const al::LayoutInitInfo&, const char*,
                              bool);
    SimpleLayoutAppearWaitEnd(al::LayoutActor*, const char*, const char*, const al::LayoutInitInfo&,
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
    s32 field_12C = -1;
};
}  // namespace al
