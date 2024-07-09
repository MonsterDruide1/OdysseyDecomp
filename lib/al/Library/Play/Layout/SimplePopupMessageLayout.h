#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimplePopupMessageLayout : public LayoutActor {
public:
    SimplePopupMessageLayout(const char* name, const char* layoutName, const LayoutInitInfo& info,
                             const char* archiveName, bool localize);
    void appear() override;
    void end();
    void startWait();
    void exeAppear();
    void refreshPos();
    void exeWait();
    void exeEnd();
    bool isWait() const;
    bool isAppearOrWait() const;

private:
    s32 mLifetime = -1;
    sead::Vector3f mWorldPos;
};
}  // namespace al
