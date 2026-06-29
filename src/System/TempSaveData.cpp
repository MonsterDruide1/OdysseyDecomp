#include "System/TempSaveData.h"

#include "Library/Base/Macros.h"
#include "Library/Placement/PlacementId.h"

#include "System/UniqObjInfo.h"

TempSaveData::TempSaveData() {
    mWorldObjects.alloc();
    mMiniGameObjects.alloc();
    mScenarioObjects.alloc();
    mWorldValues.allocBuffer(maxObjEntries, nullptr);
}

void TempSaveData::init() {
    for (s32 i = 0; i < maxObjEntries; i++) {
        mWorldObjects[i].clear();
        mMiniGameObjects[i].clear();
        mScenarioObjects[i].clear();
        // NOTE: Clearing multiple times the hashed values
        mWorldValues.clear();
    }
}

void TempSaveData::initForScenario() {
    for (s32 i = 0; i < mScenarioObjects.size(); i++)
        mScenarioObjects[i].clear();
}

void TempSaveData::resetMiniGame() {
    for (s32 i = 0; i < mMiniGameObjects.size(); i++)
        mMiniGameObjects[i].clear();
}

static ALWAYS_INLINE s32 getUniqObjId(const UniqObjInfo* objInfo,
                                      const al::PlacementId* placementId, const char* stageName) {
    al::StringTmp<128> str;
    placementId->makeString(&str);

    s32 id = -1;
    for (s32 i = 0; i < TempSaveData::maxObjEntries; i++) {
        if (!objInfo[i].stageName.isEmpty() &&
            al::isEqualString(objInfo[i].stageName.cstr(), stageName) &&
            al::isEqualString(objInfo[i].objId, str)) {
            id = i;
            break;
        }
    }
    return id;
}

static ALWAYS_INLINE void writeUniqObj(UniqObjInfo* objInfo, const al::PlacementId* placementId,
                                       const char* stageName) {
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id != -1)
        return;

    for (s32 i = 0; i < TempSaveData::maxObjEntries; i++) {
        if (objInfo[i].stageName.isEmpty()) {
            placementId->makeString(&objInfo[i].objId);
            objInfo[i].stageName.format("%s", stageName);
            break;
        }
    }
}

void deleteUniqObj(UniqObjInfo* objInfo, const al::PlacementId* placementId,
                   const char* stageName) {
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id == -1)
        return;

    objInfo[id].stageName.clear();
    objInfo[id].objId.clear();
}

void TempSaveData::setInfo(s32 worldIndex, s32 scenarioIndex) {
    mWorldIndex = worldIndex;
    mScenarioIndex = scenarioIndex;
}

void TempSaveData::writeInWorld(const al::PlacementId* placementId, const char* stageName) {
    writeUniqObj(mWorldObjects.begin(), placementId, stageName);
}

void TempSaveData::deleteInWorld(const al::PlacementId* placementId, const char* stageName) {
    deleteUniqObj(mWorldObjects.begin(), placementId, stageName);
}

bool TempSaveData::isOnInWorld(const al::PlacementId* placementId, const char* stageName) const {
    return getUniqObjId(mWorldObjects.begin(), placementId, stageName) != -1;
}

void TempSaveData::writeInWorldResetMiniGame(const al::PlacementId* placementId,
                                             const char* stageName) {
    writeUniqObj(mMiniGameObjects.begin(), placementId, stageName);
}

void TempSaveData::deleteInWorldResetMiniGame(const al::PlacementId* placementId,
                                              const char* stageName) {
    deleteUniqObj(mMiniGameObjects.begin(), placementId, stageName);
}

bool TempSaveData::isOnInWorldResetMiniGame(const al::PlacementId* placementId,
                                            const char* stageName) const {
    return getUniqObjId(mMiniGameObjects.begin(), placementId, stageName) != -1;
}

void TempSaveData::writeInScenario(const al::PlacementId* placementId, const char* stageName) {
    writeUniqObj(mScenarioObjects.begin(), placementId, stageName);
}

bool TempSaveData::isOnInScenario(const al::PlacementId* placementId, const char* stageName) const {
    return getUniqObjId(mScenarioObjects.begin(), placementId, stageName) != -1;
}

void TempSaveData::writeHashInWorld(const char* hash, bool value) {
    // TODO: Replace this with StrTreeMap::put
    {
        sead::SafeString str(hash);
        auto* node = mWorldValues.find(str);
        if (node) {
            node->value() = value;
            return;
        }
    }

    mWorldValues.insert(hash, value);
}

bool TempSaveData::findHashValueInWorld(const char* hash) const {
    auto* node = mWorldValues.find(hash);
    return node && node->value();
}
