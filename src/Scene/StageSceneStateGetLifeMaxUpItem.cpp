#include "Scene/StageSceneStateGetLifeMaxUpItem.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneUtil.h"

#include "Layout/StageSceneLayout.h"
#include "Scene/StageScene.h"
#include "System/GameDataFunction.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(StageSceneStateGetLifeMaxUpItem, DemoGetLifeMaxUpItem);
NERVES_MAKE_NOSTRUCT(StageSceneStateGetLifeMaxUpItem, DemoGetLifeMaxUpItem);
}  // namespace

StageSceneStateGetLifeMaxUpItem::StageSceneStateGetLifeMaxUpItem(const char* name, al::Scene* scene,
                                                                 StageSceneLayout* stageSceneLayout)
    : HostStateBase(name, scene), mLayout(stageSceneLayout) {
    initNerve(&DemoGetLifeMaxUpItem, 0);
}

void StageSceneStateGetLifeMaxUpItem::appear() {
    setDead(false);
    al::setNerve(this, &DemoGetLifeMaxUpItem);
}

void StageSceneStateGetLifeMaxUpItem::kill() {
    setDead(true);
}

void StageSceneStateGetLifeMaxUpItem::exeDemoGetLifeMaxUpItem() {
    if (al::isFirstStep(this)) {
        GameDataFunction::getLifeMaxUpItem(rs::getPlayerActor(getHost()));
        mLayout->tryStartDemoGetLifeMaxUpItem(false);
        al::startBgm(getHost(), "GetLifeMaxUp", -1, 0);
    }
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "プロジェクト用グラフィックス");
    al::updateKitList(getHost(), "通知レイアウト更新");
    al::updateKitList(getHost(), "２Ｄ");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());
    rs::updateDemoSystemOnly(getHost());

    if (mLayout->isEndDemoGetLifeMaxUpItem()) {
        rs::requestEndDemoGetLifeMaxUpItem(getHost());
        kill();
    }
}
