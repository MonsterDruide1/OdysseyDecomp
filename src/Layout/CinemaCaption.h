#pragma once

#include <basis/seadTypes.h>

namespace al {
class LayoutInitInfo;
}  // namespace al

class CaptionInfo;
class CaptionInfoHolder;

class CinemaCaption {
public:
    CinemaCaption(const al::LayoutInitInfo& initInfo);

    bool tryAppear(CaptionInfoHolder* captionInfoHolder);
    void setCaptionMsg(const char16* message);
    s32 calcShowFrame();
    bool tryEnd();
    void exeAppear();
    void updateMessageCore();
    void exeWait();
    void exeEnd();
    bool trySetNextCaptionMsg(CaptionInfo* captionInfo);
    bool tryHideCurrentCaptionMsg(CaptionInfo* captionInfo);
};

namespace CinemaCaptionFunction {
CaptionInfoHolder* createCaptionInfoHolder(const char* captionName);
}  // namespace CinemaCaptionFunction
