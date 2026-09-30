#pragma once

#include "Library/Nerve/NerveStateBase.h"

class KakkuStateGlide;

namespace al {
class LiveActor;
}  // namespace al

class KakkuGlideAnimCtrl : public al::ActorStateBase {
public:
    KakkuGlideAnimCtrl(const char* name, al::LiveActor* actor, KakkuStateGlide* stateGlide);

    void appear() override;
    bool isNormal() const;

    void exeGlideNormal();
    void tryChangeAnim();
    void exeGlideLeftStart();
    void exeGlideLeft();
    void exeGlideRightStart();
    void exeGlideRight();

    bool isCurrentLeftAnim() const;
    bool isCurrentRightAnim() const;

private:
    void* _20 = nullptr;
    KakkuStateGlide* mStateGlide = nullptr;
};

static_assert(sizeof(KakkuGlideAnimCtrl) == 0x30);
