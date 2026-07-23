#include "System/WorldList.h"

#include "Library/Base/StringUtil.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"

template <>
s32 sead::PtrArray<ShinePosInfo>::compareT(const ShinePosInfo* a, const ShinePosInfo* b) {
    if (a->uniqueId < b->uniqueId)
        return -1;
    return a->uniqueId != b->uniqueId;
}

// NON_MATCHING: StrTreeMap::allocBuffer major issues https://decomp.me/scratch/znUi3
WorldList::WorldList() {
    al::ByamlIter worldListIter(al::tryGetBymlFromArcName("SystemData/WorldList", "WorldList"));

    s32 worldCount = worldListIter.getSize();
    mWorldList.allocBuffer(worldCount, nullptr);
    for (s32 i = 0; i < worldCount; i++) {
        al::ByamlIter worldIter;
        const char* name = nullptr;
        worldListIter.tryGetIterByIndex(&worldIter, i);

        worldIter.tryGetStringByKey(&name, "Name");
        const char* worldName = nullptr;
        worldIter.tryGetStringByKey(&worldName, "WorldName");
        s32 scenarioNum = 1;
        worldIter.tryGetIntByKey(&scenarioNum, "ScenarioNum");
        s32 clearMainScenarioNo = 1;
        worldIter.tryGetIntByKey(&clearMainScenarioNo, "ClearMainScenario");
        s32 afterEndingScenarioNo = 1;
        worldIter.tryGetIntByKey(&afterEndingScenarioNo, "AfterEndingScenario");
        s32 moonRockScenarioNo = 1;
        worldIter.tryGetIntByKey(&moonRockScenarioNo, "MoonRockScenario");

        WorldListEntry* entry = new WorldListEntry();
        entry->mainStageName = name;
        entry->worldDevelopName = worldName;
        entry->worldScenarioNum = scenarioNum;
        s32* scnum = new s32[scenarioNum];
        entry->clearMainScenarioNo = clearMainScenarioNo;
        entry->afterEndingScenarioNo = afterEndingScenarioNo;
        entry->mainQuestIndexes = scnum;
        entry->moonRockScenarioNo = moonRockScenarioNo;

        al::ByamlIter questIter;
        worldIter.tryGetIterByKey(&questIter, "MainQuestInfo");

        for (s32 j = 0; j < scenarioNum; j++) {
            s32 questIdx = -1;
            questIter.tryGetIntByIndex(&questIdx, j);
            entry->mainQuestIndexes[j] = questIdx;
        }

        mWorldList.pushBack(entry);
    }

    al::ByamlIter worldListDbIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "WorldListFromDb"));
    s32 worldDbCount = worldListDbIter.getSize();
    for (s32 i = 0; i < worldDbCount; i++) {
        al::ByamlIter worldIter;
        worldListDbIter.tryGetIterByIndex(&worldIter, i);
        al::ByamlIter stageListIter;
        worldIter.tryGetIterByKey(&stageListIter, "StageList");

        mWorldList[i]->stageList.allocBuffer(stageListIter.getSize(), nullptr);

        for (s32 j = 0; j < stageListIter.getSize(); j++) {
            al::ByamlIter stageIter;
            stageListIter.tryGetIterByIndex(&stageIter, j);

            const char* name = nullptr;
            stageIter.tryGetStringByKey(&name, "name");

            StageDBEntry* stageEntry = new StageDBEntry();
            stageEntry->name.format("%s", name);

            const char* category = nullptr;
            if (stageIter.tryGetStringByKey(&category, "category")) {
                stageEntry->category.format("%s", category);

                if (al::isEqualString(stageEntry->category.cstr(), "MainStage"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "MainRouteStage"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "ExStage"))
                    stageEntry->useScenarioNo = 1;
                else if (al::isEqualString(stageEntry->category.cstr(), "SmallStage"))
                    stageEntry->useScenarioNo = 1;
                else if (al::isEqualString(stageEntry->category.cstr(), "PathwayStage"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "ShopStage"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "MoonExStage"))
                    stageEntry->useScenarioNo = 1;
                else if (al::isEqualString(stageEntry->category.cstr(), "Demo"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "MoonFarSideExStage"))
                    stageEntry->useScenarioNo = 2;
                else if (al::isEqualString(stageEntry->category.cstr(), "BossRevenge"))
                    stageEntry->useScenarioNo = 1;
                else if (al::isEqualString(stageEntry->category.cstr(), "MiniGame"))
                    stageEntry->useScenarioNo = -1;
                else if (al::isEqualString(stageEntry->category.cstr(), "Zone"))
                    stageEntry->useScenarioNo = -1;
            } else {
                stageEntry->category.clear();
                stageEntry->useScenarioNo = -1;
            }

            mWorldList[i]->stageList.pushBack(stageEntry);
        }
    }
    {
        al::ByamlIter worldListPosIter(
            al::tryGetBymlFromArcName("SystemData/WorldList", "StagePosList"));
        s32 posCount = worldListPosIter.getSize();

        mStagePosList.allocBuffer(posCount, nullptr);

        for (s32 i = 0; i < posCount; i++) {
            al::ByamlIter entryIter;
            const char* keyName = 0;
            worldListPosIter.tryGetIterAndKeyNameByIndex(&entryIter, &keyName, i);

            StagePosInfo* posInfo = new StagePosInfo();
            // BUG: posList should be entryIter size
            posInfo->posList.allocBuffer(worldCount, nullptr);
            posInfo->mask = 0;

            s32 size = entryIter.getSize();
            for (s32 j = 0; j < size; j++) {
                al::ByamlIter biter;
                entryIter.tryGetIterByKey(&biter, al::StringTmp<32>("%d", j + 1).cstr());
                sead::Vector3f trans = sead::Vector3f::zero;

                if (biter.tryGetFloatByKey(&trans.x, "X"))
                    posInfo->mask |= 1 << (j + 1);
                biter.tryGetFloatByKey(&trans.y, "Y");
                biter.tryGetFloatByKey(&trans.z, "Z");
                posInfo->posList.emplaceBack(trans);
            }

            mStagePosList.insert(keyName, posInfo);
        }
    }
    al::ByamlIter shinePosIter(al::tryGetBymlFromArcName("SystemData/ShineInfo", "ShinePosList"));
    s32 shineCount = shinePosIter.getSize();
    mShinePosList.allocBuffer(shineCount, nullptr);
    for (s32 i = 0; i < shineCount; i++) {
        al::ByamlIter iter;
        shinePosIter.tryGetIterByIndex(&iter, i);

        s32 uniqueId = -1;
        iter.tryGetIntByKey(&uniqueId, "UniqueId");

        ShinePosInfo* info = new ShinePosInfo();
        info->uniqueId = uniqueId;
        info->posList.allocBuffer(20, nullptr);
        info->mask = 0;

        al::ByamlIter biter;
        for (s32 j = 1; j < 20; j++) {
            if (iter.tryGetIterByKey(&biter, al::StringTmp<32>("%d", j).cstr())) {
                sead::Vector3f trans = sead::Vector3f::zero;

                if (biter.tryGetFloatByKey(&trans.x, "X")) {
                    biter.tryGetFloatByKey(&trans.y, "Y");
                    biter.tryGetFloatByKey(&trans.z, "Z");
                    info->posList.emplaceBack(trans);
                    info->mask |= 1 << j;
                } else {
                    info->posList.emplaceBack(sead::Vector3f::zero);
                }
            } else {
                info->posList.emplaceBack(sead::Vector3f::zero);
            }
        }
        mShinePosList.pushBack(info);
    }
    mShinePosList.sort();
}

s32 WorldList::getWorldNum() const {
    return mWorldList.size();
}

s32 WorldList::getMainQuestMin(s32 worldId, s32 questId) const {
    if (worldId < 0)
        worldId = 0;
    return mWorldList[worldId]->mainQuestIndexes[questId - 1];
}

const char* WorldList::getMainStageName(s32 worldId) const {
    return mWorldList[worldId]->mainStageName;
}

s32 WorldList::tryFindWorldIndexByMainStageName(const char* mainStageName) const {
    for (s32 i = 0; i < getWorldNum(); i++)
        if (al::isEqualString(getMainStageName(i), mainStageName))
            return i;

    return -1;
}

s32 WorldList::tryFindWorldIndexByStageName(const char* stageName) const {
    s32 worldIndex = tryFindWorldIndexByMainStageName(stageName);

    if (worldIndex != -1)
        return worldIndex;

    for (s32 i = 0; i < getWorldNum(); i++) {
        for (s32 j = 0; j < mWorldList[i]->stageList.size(); j++)
            if (al::isEqualString(stageName, mWorldList[i]->stageList[j]->name.cstr()))
                return i;
    }

    return -1;
}

s32 WorldList::tryFindWorldIndexByDevelopName(const char* developName) const {
    for (s32 i = 0; i < getWorldNum(); i++)
        if (al::isEqualString(getWorldDevelopName(i), developName))
            return i;

    return -1;
}

bool WorldList::isEqualClearMainScenarioNo(s32 worldId, s32 scenarioNo) const {
    return mWorldList[worldId]->clearMainScenarioNo == scenarioNo;
}

s32 WorldList::getAfterEndingScenarioNo(s32 worldId) const {
    return mWorldList[worldId]->afterEndingScenarioNo;
}

bool WorldList::isEqualAfterEndingScenarioNo(s32 worldId, s32 scenarioNo) const {
    return getAfterEndingScenarioNo(worldId) == scenarioNo;
}

s32 WorldList::getMoonRockScenarioNo(s32 worldId) const {
    return mWorldList[worldId]->moonRockScenarioNo;
}

bool WorldList::isEqualMoonRockScenarioNo(s32 worldId, s32 scenarioNo) const {
    return getMoonRockScenarioNo(worldId) == scenarioNo;
}

const char* WorldList::getWorldDevelopName(s32 worldId) const {
    if (worldId < 0)
        worldId = 0;
    return mWorldList[worldId]->worldDevelopName;
}

s32 WorldList::getWorldScenarioNum(s32 worldId) const {
    return mWorldList[worldId]->worldScenarioNum;
}

s32 WorldList::findUseScenarioNo(const char* stageName) const {
    if (al::isEqualString(stageName, "CurrentWorldHome"))
        return -1;

    s32 worldListSize = mWorldList.size();
    for (s32 i = 0; i < worldListSize; i++) {
        WorldListEntry* worldList = mWorldList[i];
        s32 stageListSize = worldList->stageList.size();
        for (s32 j = 0; j < stageListSize; j++)
            if (al::isEqualString(worldList->stageList[j]->name.cstr(), stageName))
                return worldList->stageList[j]->useScenarioNo;
    }

    return -1;
}

bool WorldList::checkNeedTreasureMessageStage(const char* stageName) const {
    s32 worldListSize = mWorldList.size();
    if (al::isEqualString(stageName, "ForestWorldWoodsStage") ||
        al::isEqualString(stageName, "SnowWorldLobbyExStage"))
        return false;

    for (s32 i = 0; i < worldListSize; i++) {
        WorldListEntry* world = mWorldList[i];
        s32 stageListSize = world->stageList.size();
        for (s32 j = 0; j < stageListSize; j++) {
            if (!al::isEqualString(world->stageList[j]->name.cstr(), stageName))
                continue;

            const char* category = world->stageList[j]->category.cstr();
            if (!al::isEqualString(category, "BossRevenge") &&
                !al::isEqualString(category, "MainRouteStage") &&
                !al::isEqualString(category, "MainStage") &&
                !al::isEqualString(category, "ShopStage") &&
                !al::isEqualString(category, "MiniGame") &&
                !al::isEqualString(category, "SmallStage"))
                return true;

            return false;
        }
    }

    return false;
}

bool WorldList::checkIsMainStage(const char* mainStageName) const {
    return tryFindWorldIndexByMainStageName(mainStageName) != -1;
}

bool WorldList::tryFindTransOnMainStageByStageName(sead::Vector3f* outTrans, const char* stageName,
                                                   s32 index) const {
    sead::StrTreeMap<128, StagePosInfo*>::Node* posInfo = mStagePosList.find(stageName);

    if (!posInfo)
        return false;

    outTrans->set(*posInfo->value()->posList[index - 1]);
    return posInfo->value()->mask & (1 << index);
}

s32 WorldList::findHintByScenarioNo(s32 scenarioNo) const {
    s32 size = mShinePosList.size();
    for (s32 i = 0; i < size; i++)
        if (mShinePosList[i]->uniqueId == scenarioNo)
            return i;
    return -1;
}

bool WorldList::tryFindHintTransByScenarioNo(sead::Vector3f* outTrans, s32 scenarioNo,
                                             s32 index) const {
    s32 hintId = findHintByScenarioNo(scenarioNo);
    if (hintId == -1)
        return false;

    ShinePosInfo* posInfo = mShinePosList[hintId];

    if ((1 << index & posInfo->mask) == 0)
        return false;

    outTrans->set(*posInfo->posList[index - 1]);
    return true;
}
