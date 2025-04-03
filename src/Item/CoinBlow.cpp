#include "Item/CoinBlow.h"

#include "Library/Base/StringUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

CoinBlow::CoinBlow(const char* name) : al::LiveActor(name) {}

void CoinBlow::init(const al::ActorInitInfo& initInfo) {
    using CoinBlowFunctor = al::FunctorV0M<CoinBlow*, void (CoinBlow::*)()>;

    al::initActorWithArchiveName(this, initInfo, "CoinBlow", nullptr);
    al::listenStageSwitchOnStart(this, CoinBlowFunctor(this, &CoinBlow::listenStart));
    al::tryGetStringArg(&mBlowSize, initInfo, "BlowSize");
    makeActorDead();
}

void CoinBlow::listenStart() {
    if (mBlowSize == nullptr) {
        al::appearItemTiming(this, "小");
        return;
    }
    if (al::isEqualString(mBlowSize, "VerySmall")) {
        al::appearItemTiming(this, "極小");
        return;
    }
    if (al::isEqualString(mBlowSize, "Little")) {
        al::appearItemTiming(this, "小");
        return;
    }
    if (al::isEqualString(mBlowSize, "Middle")) {
        al::appearItemTiming(this, "中");
        return;
    }
    if (al::isEqualString(mBlowSize, "Large")) {
        al::appearItemTiming(this, "大");
        return;
    }
    if (al::isEqualString(mBlowSize, "BlowUp")) {
        al::appearItemTiming(this, "吹き出し");
        return;
    }
    if (al::isEqualString(mBlowSize, "BlowUpLittle")) {
        al::appearItemTiming(this, "吹き出し[小]");
        return;
    }
}
