#include "MapObj/SubActorLodFixPartsScenarioAction.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/MapObj/SubActorLodExecutor.h"
#include "Library/MapObj/SupportFreezeSyncGroupHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Scene/SceneEventNotifyFunction.h"
#include "Scene/SceneObjFactory.h"
#include "System/GameDataFunction.h"

namespace {
class SupportFreezeSyncGroupHolderSceneObj : public al::SupportFreezeSyncGroupHolder {
public:
    static constexpr s32 sSceneObjId = SceneObjID__3d;
};
}  // namespace

SubActorLodFixPartsScenarioAction::SubActorLodFixPartsScenarioAction(const char* name)
    : al::LiveActor(name) {}

void SubActorLodFixPartsScenarioAction::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);

    al::StringTmp<64> actionName("Scenario%d", GameDataFunction::getScenarioNo(this));
    mIsActionStarted = al::tryStartAction(this, actionName.cstr());

    al::LiveActor* subActor = al::getSubActor(this, 0);
    mIsActionStarted |= al::tryStartAction(subActor, actionName.cstr());

    mSubActorLodExecutor = new al::SubActorLodExecutor(this, info, 0);
    al::trySyncStageSwitchAppearAndKill(this);

    using SnapShotFunctor = al::FunctorV0M<SubActorLodFixPartsScenarioAction*,
                                           void (SubActorLodFixPartsScenarioAction::*)()>;
    rs::listenSnapShotModeOnOff(
        this, SnapShotFunctor(this, &SubActorLodFixPartsScenarioAction::listenStartSnapShotMode),
        SnapShotFunctor(this, &SubActorLodFixPartsScenarioAction::listenEndSnapShotMode));

    makeActorAlive();
}

void SubActorLodFixPartsScenarioAction::listenStartSnapShotMode() {
    al::showModelIfHide(this);
    mSubActorLodExecutor->getLodSubActor()->kill();
}

void SubActorLodFixPartsScenarioAction::listenEndSnapShotMode() {}

void SubActorLodFixPartsScenarioAction::movement() {
    mSubActorLodExecutor->control();

    if (mIsActionStarted)
        al::LiveActor::movement();
}

void SubActorLodFixPartsScenarioAction::calcAnim() {
    if (mIsActionStarted) {
        al::LiveActor::calcAnim();
        return;
    }

    al::calcViewModel(this);
}

bool SubActorLodFixPartsScenarioAction::receiveMsg(const al::SensorMsg* message,
                                                   al::HitSensor* other, al::HitSensor* self) {
    return al::isMsgAskSafetyPoint(message);
}

namespace al {
void registSupportFreezeSyncGroup(LiveActor* actor, const ActorInitInfo& info) {
    if (actor->getHitSensorKeeper() &&
        alPlacementFunction::isEnableLinkGroupId(info, "SupportFreezeSyncGroup"))
        createSceneObj<SupportFreezeSyncGroupHolderSceneObj>(actor)->regist(actor, info);
}
}  // namespace al
