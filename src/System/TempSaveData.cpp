#include "System/TempSaveData.h"

#include "Library/Placement/PlacementId.h"

#include "System/UniqObjInfo.h"

TempSaveData::TempSaveData() {
    mWordObjects = new UniqObjInfo[64];
    mMiniGameObjects = new UniqObjInfo[64];
    mScenarioObjects = new UniqObjInfo[64];
    mWorldValues.allocBuffer(64, nullptr);
}

void TempSaveData::init() {
    for (s32 i = 0; i < 64; i++) {
        mWordObjects[i].clear();
        mMiniGameObjects[i].clear();
        mScenarioObjects[i].clear();
        // BUG: Clearing multiple times the hashed values
        mWorldValues.clear();
    }
}

void TempSaveData::initForScenario() {
    for (s32 i = 0; i < 64; i++)
        mScenarioObjects[i].clear();
}

void TempSaveData::resetMiniGame() {
    for (s32 i = 0; i < 64; i++)
        mMiniGameObjects[i].clear();
}

inline s32 getUniqObjId(const UniqObjInfo* objInfo, const al::PlacementId* placementId,
                        const char* stageName) {
    al::StringTmp<128> str;
    placementId->makeString(&str);

    s32 id = -1;
    for (s32 i = 0; i < 64; i++) {
        if (!objInfo[i].getStageName().isEmpty() &&
            al::isEqualString(objInfo[i].getStageName().cstr(), stageName) &&
            al::isEqualString(objInfo[i].getObjId(), str)) {
            id = i;
            break;
        }
    }
    return id;
}

void deleteUniqObj(UniqObjInfo* objInfo, const al::PlacementId* placementId,
                   const char* stageName) {
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id == -1)
        return;

    objInfo[id].getStageName()->clear();
    objInfo[id].getObjId()->clear();
}

void TempSaveData::setInfo(s32 worldIndex, s32 unknown) {
    mWorldIndex = worldIndex;
    _1c = unknown;
}

void TempSaveData::writeInWorld(const al::PlacementId* placementId, const char* stageName) {
    UniqObjInfo* objInfo = mWordObjects;
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id != -1)
        return;

    for (s32 i = 0; i < 64; i++) {
        if (objInfo[i].getStageName()->isEmpty()) {
            placementId->makeString(objInfo[i].getObjId());
            objInfo[i].getStageName()->format("%s", stageName);
            break;
        }
    }
}

void TempSaveData::deleteInWorld(const al::PlacementId* placementId, const char* stageName) {
    deleteUniqObj(mWordObjects, placementId, stageName);
}

bool TempSaveData::isOnInWorld(const al::PlacementId* placementId, const char* stageName) const {
    return getUniqObjId(mWordObjects, placementId, stageName) != -1;
}

void TempSaveData::writeInWorldResetMiniGame(const al::PlacementId* placementId,
                                             const char* stageName) {
    UniqObjInfo* objInfo = mMiniGameObjects;
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id != -1)
        return;

    for (s32 i = 0; i < 64; i++) {
        if (objInfo[i].getStageName()->isEmpty()) {
            placementId->makeString(objInfo[i].getObjId());
            objInfo[i].getStageName()->format("%s", stageName);
            break;
        }
    }
}

void TempSaveData::deleteInWorldResetMiniGame(const al::PlacementId* placementId,
                                              const char* stageName) {
    deleteUniqObj(mMiniGameObjects, placementId, stageName);
}

bool TempSaveData::isOnInWorldResetMiniGame(const al::PlacementId* placementId,
                                            const char* stageName) const {
    return getUniqObjId(mMiniGameObjects, placementId, stageName) != -1;
}

void TempSaveData::writeInScenario(const al::PlacementId* placementId, const char* stageName) {
    UniqObjInfo* objInfo = mScenarioObjects;
    s32 id = getUniqObjId(objInfo, placementId, stageName);
    if (id != -1)
        return;

    for (s32 i = 0; i < 64; i++) {
        if (objInfo[i].getStageName()->isEmpty()) {
            placementId->makeString(objInfo[i].getObjId());
            objInfo[i].getStageName()->format("%s", stageName);
            break;
        }
    }
}

bool TempSaveData::isOnInScenario(const al::PlacementId* placementId, const char* stageName) const {
    return getUniqObjId(mScenarioObjects, placementId, stageName) != -1;
}

// NON_MATCHING: Wrong loading order https://decomp.me/scratch/OCYs2
void TempSaveData::writeHashInWorld(const char* hash, bool value) {
    auto* node = mWorldValues.find(hash);
    if (node) {
        node->value() = value;
        return;
    }

    mWorldValues.insert(hash, value);
}

bool TempSaveData::findHashValueInWorld(const char* hash) const {
    auto* node = mWorldValues.find(hash);
    return node && node->value();
}
