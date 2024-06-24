#include "Layout/MenuSelectParts.h"

#include "Library/Layout/LayoutActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataHolderAccessor.h"

namespace {
NERVE_IMPL(MenuSelectParts, Hide);
NERVE_IMPL(MenuSelectParts, Appear);
NERVE_IMPL(MenuSelectParts, Select);
NERVE_IMPL(MenuSelectParts, DecideEnd);
NERVE_IMPL(MenuSelectParts, DecideParts);
NERVE_IMPL_(MenuSelectParts, SelectSecond, Select);
NERVE_IMPL(MenuSelectParts, DecideInterval);

NERVES_MAKE_NOSTRUCT(MenuSelectParts, Hide, Appear, DecideEnd);
NERVES_MAKE_STRUCT(MenuSelectParts, Select, DecideParts, SelectSecond, DecideInterval);
}  // namespace

const int dword_71018AF55C[5] = {0, 1, 2, 3, 5};
const int dword_71018AF570[8] = {0, 1, 3, 4, 5, 0, 0, 0};

MenuSelectParts::MenuSelectParts(const char* name, al::LayoutActor* layoutActor,
                                 al::LiveActor* marioActor, const al::LayoutInitInfo& info,
                                 s32 menuItemCount)
    : al::NerveExecutor(name), mLayoutActor(layoutActor) {}

void MenuSelectParts::update() {
    updateNerve();
}

void MenuSelectParts::appear(s32 menuItemAmount) {
    field_58 = false;
    mMenuItemAmount = menuItemAmount;
    mCursorItemIndex = 0;
    field_38 = false;
}

void MenuSelectParts::startActionPartsIllustSelectIndex() {}

void MenuSelectParts::appearWait() {}

void MenuSelectParts::setSelectMessage(s32, const char16*) {}

bool MenuSelectParts::isDecideContinue() const {}

bool MenuSelectParts::isDecideEnd() const {}

bool MenuSelectParts::isSelectContinue() const {}

bool MenuSelectParts::isDecideSetting() const {}

bool MenuSelectParts::isSelectSetting() const {}

bool MenuSelectParts::isDecideSave() const {}

bool MenuSelectParts::isSelectSave() const {
    return calcPartsIndex(mCursorItemIndex) == 4;
}

bool MenuSelectParts::isDecideSeparatePlay() const {}

bool MenuSelectParts::isSelectSeparatePlay() const {}

bool MenuSelectParts::isDecideHelp() const {}

bool MenuSelectParts::isSelectHelp() const {}

bool MenuSelectParts::isDecideNewGame() const {}

bool MenuSelectParts::isSelectNewGame() const {}

s32 MenuSelectParts::calcPartsIndex(s32 selection) const {
    u32 absSelection = selection;
    const int* partsArray = !field_58 ? dword_71018AF570 : dword_71018AF55C;
    return partsArray[absSelection];
}

void MenuSelectParts::exeHide() {}

void MenuSelectParts::exeAppear() {}

void MenuSelectParts::startActionMarioSelectIndex() {}

void MenuSelectParts::exeSelect() {}

void MenuSelectParts::startActionMario(al::LiveActor*, const char*) {}

void MenuSelectParts::exeDecideParts() {}

bool MenuSelectParts::isInvalidSelect() const {
    GameDataHolderAccessor accessor(this->mLayoutActor);

    if (rs::isSceneStatusInvalidSave(accessor)) {
        if ((calcPartsIndex(mCursorItemIndex) & 0xFFFFFFFE) == 4)
            return true;
    }

    return false;
}

void MenuSelectParts::exeDecideInterval() {
    al::setNerveAtGreaterEqualStep(this, &DecideEnd, 0);
}

void MenuSelectParts::exeDecideEnd() {}
