#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class LiveActor;
}  // namespace al

class InformationWindowLayout : public al::LayoutActor {
public:
    InformationWindowLayout(const al::LayoutInitInfo& info);

    void appear() override;

    void changeSeparatePlay();
    bool isSeparateTutorial() const;
    bool isSingleTutorial() const;
    void changeSinglePlay();
    void setSeparatePlayerOnlyLayout();
    void setSinglePlayLayout();
    void setHackTutorial(const al::LiveActor* actor, const char* label);
    void setTutorial(const char* label);
    void setSeparatePlayTutorial(const char* leftLabel, const char* rightLabel);
    void setAreaTutorial(const char16* message);
    void end();
    const char* getTutorialMstxtName() const;
    bool isWait() const;
    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    s32 mPlayMode = 0;
};

static_assert(sizeof(InformationWindowLayout) == 0x130);
