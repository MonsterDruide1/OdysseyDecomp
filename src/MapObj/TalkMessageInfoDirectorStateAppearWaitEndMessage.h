#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SimpleLayoutAppearWaitEnd;
}

class TalkMessageInfoDirector;

class TalkMessageInfoDirectorStateAppearWaitEndMessage
    : public al::HostStateBase<TalkMessageInfoDirector> {
public:
    TalkMessageInfoDirectorStateAppearWaitEndMessage(TalkMessageInfoDirector* director,
                                                     al::SimpleLayoutAppearWaitEnd* layout);

    void appear() override;
    void kill() override;

    void exeAppear();
    void exeWait();
    void exeEnd();
    void end();
    bool isActiveCapMessage() const;

private:
    al::SimpleLayoutAppearWaitEnd* mLayout = nullptr;
};

static_assert(sizeof(TalkMessageInfoDirectorStateAppearWaitEndMessage) == 0x28);
