#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWaitEnd : public LayoutActor {
public:
    SimpleLayoutAppearWaitEnd(const char* name, const char* layoutName, const LayoutInitInfo& info,
                              const char* archiveName, bool localize);
    SimpleLayoutAppearWaitEnd(LayoutActor* parentActor, const char* name, const char* layoutName,
                              const LayoutInitInfo& info, const char* archiveName);

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
