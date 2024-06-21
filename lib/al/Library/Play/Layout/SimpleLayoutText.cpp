#include "Library/Play/Layout/SimpleLayoutText.h"

#include <prim/seadStringUtil.h>

#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_IMPL(SimpleLayoutText, Wait);
NERVES_MAKE_NOSTRUCT(SimpleLayoutText, Wait);

}  // namespace

al::SimpleLayoutText::SimpleLayoutText(const LayoutInitInfo& info, const char* name,
                                       const char* paneName, const char* archiveName)
    : LayoutActor("テキストレイアウト"), mPaneName(paneName) {
    initLayoutActor(this, info, name, archiveName);
    initNerve(&Wait, 0);
    kill();
}

void al::SimpleLayoutText::start(const sead::Vector2f& pos, const char* str, s32 lifetime) {
    setLocalTrans(this, pos);
    mLifetime = lifetime;

    sead::WFixedSafeString<256> utf16str;

    sead::StringUtil::convertUtf8ToUtf16(utf16str.getBuffer(), utf16str.getBufferSize(), str, -1);

    setPaneString(this, mPaneName, utf16str.cstr(), 0);
    setNerve(this, &Wait);
    appear();
}

void al::SimpleLayoutText::setPos(const sead::Vector2f& pos) {
    setLocalTrans(this, pos);
}

void al::SimpleLayoutText::setText(const char*) {}

void al::SimpleLayoutText::start(const sead::Vector2f& pos, const char16* str, s32 lifetime) {
    setLocalTrans(this, pos);
    mLifetime = lifetime;
    setPaneString(this, mPaneName, str, 0);
    setNerve(this, &Wait);
    appear();
}

void al::SimpleLayoutText::setText(const char16* str) {
    setPaneString(this, mPaneName, str, 0);
}

void al::SimpleLayoutText::start(const sead::Vector2f& pos, const char* category, const char* key,
                                 s32 lifetime) {
    const char16* localizedStr = getSystemMessageString(this, category, key);
    start(pos, localizedStr, lifetime);
}

void al::SimpleLayoutText::setScale(f32 scale) {
    setLocalScale(this, scale);
}

void al::SimpleLayoutText::setColor(const sead::Color4u8& color) {
    setPaneVtxColor(this, mPaneName, color);
}

void al::SimpleLayoutText::setPositionCenterH() {
    setTextPositionCenterH(this, mPaneName);
}

void al::SimpleLayoutText::exeWait() {
    if (mLifetime >= 0 && !isLessStep(this, mLifetime))
        kill();
}

al::SimpleLayoutText* al::createSimpleLayoutText(const al::ActorInitInfo& info, const char* name,
                                                 const char* paneName, const char* archiveName) {
    return new SimpleLayoutText(*info.getLayoutInitInfo(), name, paneName, archiveName);
}
