#include "Boss/Koopa/KoopaHintHolder.h"

#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/CapMessageShowInfo.h"

namespace {
NERVE_IMPL(KoopaHintHolder, Wait)
NERVE_IMPL(KoopaHintHolder, HintCapReflect)
NERVE_IMPL(KoopaHintHolder, MessageDamage)
NERVE_IMPL(KoopaHintHolder, HintCapAttachBomb)

NERVES_MAKE_NOSTRUCT(KoopaHintHolder, HintCapReflect, HintCapAttachBomb, Wait, MessageDamage)
}  // namespace

KoopaBombHintRequestInfo::KoopaBombHintRequestInfo() : isRequested(false) {}

KoopaHintHolder::KoopaHintHolder(al::SceneObjHolder* sceneObjHolder)
    : al::NerveExecutor("クッパヒント保持"), mSceneObjHolder(sceneObjHolder),
      mIsHintCapAttachBombRequested(false), mIsHintCapReflectEnabled(true), mBombHeadHintIndex(-1) {
    initNerve(&Wait, 0);
}

void KoopaHintHolder::update() {
    updateNerve();
}

bool KoopaHintHolder::tryAppearHintCapReflect() {
    if (!mIsHintCapReflectEnabled)
        return false;

    mIsHintCapReflectEnabled = false;
    al::setNerve(this, &HintCapReflect);
    return true;
}

bool KoopaHintHolder::tryAppearMessageDamage(s32 damageType) {
    if (!al::isNerve(this, &Wait))
        return false;

    rs::showCapMessageBossDamage(this, al::StringTmp<64>("Koopa_Damage_%d", damageType).cstr(), 30,
                                 30);
    al::setNerve(this, &MessageDamage);
    return true;
}

void KoopaHintHolder::exeWait() {
    if (!mIsHintCapAttachBombRequested)
        return;

    mIsHintCapAttachBombRequested = false;
    al::setNerve(this, &HintCapAttachBomb);
}

void KoopaHintHolder::exeHintCapReflect() {
    if (al::isFirstStep(this))
        rs::showCapMessage(this, "KoopaHintCapReflect", 60, 30);

    al::setNerveAtGreaterEqualStep(this, &Wait, 60);
}

void KoopaHintHolder::exeHintCapAttachBomb() {
    if (al::isFirstStep(this)) {
        mBombHeadHintIndex = al::modi(mBombHeadHintIndex + 4, 3);
        al::StringTmp<64> label("KoopaHintCapAttachBombHead_%02d", mBombHeadHintIndex);
        rs::showCapMessage(this, label.cstr(), 60, 0);
    }

    al::setNerveAtGreaterEqualStep(this, &Wait, 60);
}

void KoopaHintHolder::exeMessageDamage() {
    al::setNerveAtGreaterEqualStep(this, &Wait, 30);
}
