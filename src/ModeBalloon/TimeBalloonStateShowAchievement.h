#pragma once

#include "Library/Base/StringUtil.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
}

class TimeBalloonStateShowAchievement : public al::NerveStateBase {
public:
    TimeBalloonStateShowAchievement(const al::ActorInitInfo& info);

    void appear() override;

    void exeShow();
    void exeClose();

private:
    void* field_18 = nullptr;
    void* field_20 = nullptr;
    void* field_28 = nullptr;
    al::WStringTmp<64> field_30;
    al::WStringTmp<64> field_c8;
};
