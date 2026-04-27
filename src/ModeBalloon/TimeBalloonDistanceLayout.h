#pragma once

#include <basis/seadTypes.h>

namespace al {
class LayoutInitInfo;
class LayoutActor;
class LiveActor;
}  // namespace al

class TimeBalloonDistanceLayout {
public:
    TimeBalloonDistanceLayout(const al::LayoutInitInfo& info);

    void tryAppear();
    void tryEnd();
    f32 calcDistance(const al::LiveActor* actor);
};

namespace TimeBalloon {
void appearAndStartAction(al::LayoutActor* layout, const char* actionName);
void appearGuideLayout(al::LayoutActor* layout, const char* actionName);
}  // namespace TimeBalloon
