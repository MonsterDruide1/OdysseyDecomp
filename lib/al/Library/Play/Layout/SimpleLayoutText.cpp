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

namespace al {

SimpleLayoutText::SimpleLayoutText(const LayoutInitInfo& info, const char* name,
                                   const char* paneName, const char* archiveName)
    : LayoutActor("テキストレイアウト"), mPaneName(paneName) {
    initLayoutActor(this, info, name, archiveName);
    initNerve(&Wait, 0);
    kill();
}

void SimpleLayoutText::start(const sead::Vector2f& pos, const char* str, s32 lifetime) {
    setPos(pos);
    mLifetime = lifetime;

    setText(str);
    setNerve(this, &Wait);
    appear();
}

void SimpleLayoutText::setPos(const sead::Vector2f& pos) {
    setLocalTrans(this, pos);
}

void SimpleLayoutText::setText(const char* text) {
    sead::WFixedSafeString<256> utf16str;

    sead::StringUtil::convertUtf8ToUtf16(utf16str.getBuffer(), utf16str.getBufferSize(), text, -1);

    // doesn't match with call to setText(const char16*)
    setPaneString(this, mPaneName, utf16str.cstr(), 0);
}

void SimpleLayoutText::start(const sead::Vector2f& pos, const char16* str, s32 lifetime) {
    setPos(pos);
    mLifetime = lifetime;
    setText(str);
    setNerve(this, &Wait);
    appear();
}

void SimpleLayoutText::setText(const char16* str) {
    setPaneString(this, mPaneName, str, 0);
}

void SimpleLayoutText::start(const sead::Vector2f& pos, const char* category, const char* key,
                             s32 lifetime) {
    const char16* localizedStr = getSystemMessageString(this, category, key);
    start(pos, localizedStr, lifetime);
}

void SimpleLayoutText::setScale(f32 scale) {
    setLocalScale(this, scale);
}

void SimpleLayoutText::setColor(const sead::Color4u8& color) {
    setPaneVtxColor(this, mPaneName, color);
}

void SimpleLayoutText::setPositionCenterH() {
    setTextPositionCenterH(this, mPaneName);
}

void SimpleLayoutText::exeWait() {
    if (mLifetime >= 0 && !isLessStep(this, mLifetime))
        kill();
}

SimpleLayoutText* createSimpleLayoutText(const ActorInitInfo& info, const char* name,
                                         const char* paneName, const char* archiveName) {
    return new SimpleLayoutText(*info.layoutInitInfo, name, paneName, archiveName);
}

}  // namespace al
