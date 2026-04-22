#include "Scene/QuestInfo.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

#include "System/GameDataFunction.h"
#include "Util/StageLayoutFunction.h"

QuestInfo::QuestInfo() = default;

void QuestInfo::clear() {
    mQuestNo = -1;
    mTrans.set(sead::Vector3f::zero);
    mIsMainQuest = false;
    mIsInvalid = false;
    mSceneObjHolder = nullptr;
    mScenarioName.clear();
    mStageName.clear();
    mPlacementStageName.clear();
    mPlacementId.clear();
    mIsSingle = false;
}

void QuestInfo::init(const al::ActorInitInfo& initInfo) {
    init(al::getPlacementInfo(initInfo), initInfo);
}

void QuestInfo::init(const al::PlacementInfo& placementInfo, const al::ActorInitInfo& initInfo) {
    init(placementInfo, initInfo.actorSceneInfo.sceneObjHolder);
}

void QuestInfo::init(const al::PlacementInfo& placementInfo, al::SceneObjHolder* holder) {
    mIsMainQuest = true;
    mSceneObjHolder = holder;
    al::tryGetTrans(&mTrans, placementInfo);
    al::tryGetArg(&mQuestNo, placementInfo, "QuestNo");
    al::tryGetArg(&mIsMainQuest, placementInfo, "IsMainQuest");
    al::tryGetArg(&mIsSingle, placementInfo, "IsSingle");

    al::PlacementId placementId;
    al::tryGetPlacementId(&placementId, placementInfo);
    mPlacementId = al::makeStringPlacementId(&placementId);

    mPlacementStageName.format("%s", rs::getPlacementStageName(this, placementInfo));

    const char* className = nullptr;
    if (!al::tryGetClassName(&className, placementInfo) ||
        !al::searchSubString(className, "Shine")) {
        mScenarioName.clear();
        mStageName.clear();
        return;
    }

    if (al::isObjectName(placementInfo, "ShineDummy")) {
        const char* stageName = nullptr;
        const char* label = nullptr;
        al::getStringArg(&stageName, placementInfo, "StageName");
        al::getStringArg(&label, placementInfo, "ObjId");
        setStageName(stageName);
        setLabel(label);
        return;
    }

    rs::makeMessageLabel(&mScenarioName, placementInfo, "ScenarioName");
    // NOTE: variable format string
    mStageName.format(rs::getPlacementStageName(this, placementInfo));
}

void QuestInfo::setStageName(const char* stageName) {
    mStageName.format("%s", stageName);
}

void QuestInfo::setLabel(const char* label) {
    mScenarioName.format("ScenarioName_%s", label);
}

void QuestInfo::copy(const QuestInfo* quest) {
    // BUG: does not copy `mIsInvalid
    mTrans.set(quest->getTrans());
    mQuestNo = quest->getQuestNo();
    mIsMainQuest = quest->isMainQuest();
    mSceneObjHolder = quest->getSceneObjHolder();
    mScenarioName = quest->getScenarioName();
    mStageName = quest->getStageName();
    mIsSingle = quest->isSingle();
    mPlacementId = quest->getPlacementId();
    mPlacementStageName = quest->getPlacementStageName();
}

void QuestInfo::end() {
    // NOTE: special handling to story moon in Metro's festival
    mIsInvalid = !(GameDataFunction::isWorldCity(this) && mQuestNo == 8);
}

bool QuestInfo::isEqual(const QuestInfo* quest) const {
    if (mQuestNo != quest->getQuestNo() || mIsMainQuest != quest->isMainQuest())
        return false;

    if (!mScenarioName.isEmpty() && !mStageName.isEmpty()) {
        return al::isEqualString(mScenarioName.cstr(), quest->getScenarioName()) &&
               al::isEqualString(mStageName.cstr(), quest->getStageName());
    }

    if (mIsSingle && quest->isSingle())
        return true;

    if (mPlacementStageName == quest->getPlacementStageName() &&
        mPlacementId == quest->getPlacementId())
        return true;

    return al::isNear(mTrans, quest->getTrans());
}
