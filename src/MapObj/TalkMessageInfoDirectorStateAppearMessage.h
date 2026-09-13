#pragma once

#include "Library/Base/StringUtil.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class TalkMessageInfoDirector;

class TalkMessageInfoDirectorStateAppearMessage
    : public al::HostStateBase<TalkMessageInfoDirector> {
public:
    TalkMessageInfoDirectorStateAppearMessage(TalkMessageInfoDirector* director,
                                              al::SimpleLayoutAppearWaitEnd* layout);

    void init() override;
    void appear() override;
    void kill() override;

    void exeAppearWait();
    void exeAppear();
    void exeFollowText();
    void exeWait();
    void exeEnd();

    void setWaitFrame(s32 waitFrame);
    void setDelay(s32 delay);
    void setLabelName(const char* labelName);
    const char* getLabelName() const;
    bool isActiveCapMessage(const char* labelName) const;
    bool isDelayCapMessage(const char* labelName) const;

private:
    al::SimpleLayoutAppearWaitEnd* mLayout = nullptr;
    s32 mWaitFrame = 90;
    s32 mDelay = 0;
    al::StringTmp<128> mLabelName;
};

static_assert(sizeof(TalkMessageInfoDirectorStateAppearMessage) == 0xC8);
