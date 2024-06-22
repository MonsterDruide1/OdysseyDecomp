#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class SimpleLayoutAppearWait : public LayoutActor {
public:
    SimpleLayoutAppearWait(const char* name, const char* layoutName, const LayoutInitInfo& info,
                           const char* archiveName);
    SimpleLayoutAppearWait(LayoutActor* parentActor, const char* name, const char* layoutName,
                           const LayoutInitInfo& info, const char* archiveName);

    void appear() override;

    bool isWait() const;
    void exeAppear();
    void exeWait();
};
}  // namespace al
