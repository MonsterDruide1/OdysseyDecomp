#include "System/SaveObjInfo.h"

#include "Library/LiveActor/ActorInitInfo.h"

#include "System/GameDataFunction.h"

SaveObjInfo* SaveObjInfo::createSaveObjInfoWriteSaveData(const al::ActorInitInfo& initInfo,
                                                         const al::PlacementId* id) {
    return new SaveObjInfo(initInfo, id, SaveType::Write);
}

SaveObjInfo*
SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& initInfo,
                                                         const al::PlacementId* id) {
    return new SaveObjInfo(initInfo, id, SaveType::NoWriteInSameWorld);
}

SaveObjInfo* SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(
    const al::ActorInitInfo& initInfo, const al::PlacementId* id) {
    return new SaveObjInfo(initInfo, id, SaveType::NoWriteResetMinigame);
}

SaveObjInfo*
SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& initInfo,
                                                            const al::PlacementId* id) {
    return new SaveObjInfo(initInfo, id, SaveType::NoWriteInSameScenario);
}

SaveObjInfo::SaveObjInfo(const al::ActorInitInfo& initInfo, const al::PlacementId* placementId,
                         SaveType saveType)
    : mPlacementId(placementId), mSaveType(saveType) {
    mGameDataHolder = reinterpret_cast<GameDataHolder*>(initInfo.actorSceneInfo.gameDataHolder);
    mIsOn = checkIsOn();
}

void SaveObjInfo::on() {
    if (mIsOn)
        return;

    mIsOn = true;
    switch (mSaveType) {
    case SaveType::Write:
        GameDataFunction::setObjStarted(mGameDataHolder, mPlacementId);
        break;
    case SaveType::NoWriteInSameWorld:
        GameDataFunction::onObjNoWriteSaveData(mGameDataHolder, mPlacementId);
        break;
    case SaveType::NoWriteResetMinigame:
        GameDataFunction::onObjNoWriteSaveDataResetMiniGame(mGameDataHolder, mPlacementId);
        break;
    case SaveType::NoWriteInSameScenario:
        GameDataFunction::onObjNoWriteSaveDataInSameScenario(mGameDataHolder, mPlacementId);
        break;
    }
}

void SaveObjInfo::off() {
    if (!mIsOn)
        return;

    mIsOn = false;

    switch (mSaveType) {
    case SaveType::NoWriteInSameWorld:
        GameDataFunction::offObjNoWriteSaveData(mGameDataHolder, mPlacementId);
        break;
    case SaveType::NoWriteResetMinigame:
        GameDataFunction::offObjNoWriteSaveDataResetMiniGame(mGameDataHolder, mPlacementId);
        break;
    default:
        break;
    }
}

bool SaveObjInfo::isOn() const {
    return mIsOn;
}

void SaveObjInfo::resetByPlacementId(al::PlacementId* id) {
    mPlacementId = id;
    mIsOn = checkIsOn();
}

bool SaveObjInfo::checkIsOn() const {
    switch (mSaveType) {
    case SaveType::Write:
        return GameDataFunction::isObjStarted(mGameDataHolder, mPlacementId);
    case SaveType::NoWriteInSameWorld:
        return GameDataFunction::isOnObjNoWriteSaveData(mGameDataHolder, mPlacementId);
    case SaveType::NoWriteResetMinigame:
        return GameDataFunction::isOnObjNoWriteSaveDataResetMiniGame(mGameDataHolder, mPlacementId);
    case SaveType::NoWriteInSameScenario:
        return GameDataFunction::isOnObjNoWriteSaveDataInSameScenario(mGameDataHolder,
                                                                      mPlacementId);
    default:
        return false;
    }
}
