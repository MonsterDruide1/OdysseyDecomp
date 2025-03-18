#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class MiniGameLayout : public al::LayoutActor {
public:
    MiniGameLayout(const char* name, const al::LayoutInitInfo& info);

    void appear() override;
    void kill() override;

    void startJumprope();
    void startRace();
    void startVolleyball();
    void end();
    void setBestCount(s32 count);
    void setTodayCount(s32 count);
    void setCount(s32 count);
    void startNewRecord();
    void startNewRecordWait();
    void startNewRecordToday();
    bool isEnd() const;

    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    sead::WFixedSafeString<32> mScoreText;
};
