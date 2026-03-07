#include "System/SequenceDemoSkipData.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "System/WorldList.h"

void SequenceDemoSkipData::WorldData::init() {
    isFirstWorldIntroCamera = true;
    for (s32 i = 0; i < 10; i++)
        isFirstScenarioStartCamera[i] = true;
}

SequenceDemoSkipData::SequenceDemoSkipData(const WorldList* worldList) : mWorldList(worldList) {
    s32 worldNum = worldList->getWorldNum();
    mWorldData.allocBuffer(worldNum, nullptr);
    for (s32 i = 0; i < worldNum; i++)
        mWorldData.pushBack(new WorldData());
}

void SequenceDemoSkipData::init() {
    for (s32 i = 0; i < mWorldData.size(); i++)
        mWorldData[i]->init();
    mIsShowDemoWorldWarpHole = false;
}

void SequenceDemoSkipData::showWorldIntroCamera(s32 worldId) {
    mWorldData(worldId)->isFirstWorldIntroCamera = false;
}

void SequenceDemoSkipData::showScenarioStartCamera(s32 worldId, s32 questId) {
    mWorldData(worldId)->isFirstScenarioStartCamera[questId] = false;
}

bool SequenceDemoSkipData::isAlreadyShowWorldIntroCamera(s32 worldId) const {
    return !mWorldData[worldId]->isFirstWorldIntroCamera;
}

bool SequenceDemoSkipData::isAlreadyShowScenarioStartCamera(s32 worldId, s32 questId) const {
    return !mWorldData[worldId]->isFirstScenarioStartCamera[questId];
}

void SequenceDemoSkipData::write(al::ByamlWriter* writer) {
    writer->pushHash("SequenceDemoSkipData");

    writer->pushArray("WorldData");
    for (s32 i = 0; i < mWorldData.size(); i++) {
        writer->pushHash();
        writer->addBool("IsFirstWorldIntroCamera", mWorldData[i]->isFirstWorldIntroCamera);
        writer->pushArray("IsFirstScenarioStartCamera");
        for (s32 j = 0; j < 10; j++)
            writer->addBool(mWorldData[i]->isFirstScenarioStartCamera[j]);
        writer->pop();
        writer->pop();
    }
    writer->pop();

    writer->addBool("IsShowDemoWorldWarpHole", mIsShowDemoWorldWarpHole);
    writer->pop();
}

void SequenceDemoSkipData::read(const al::ByamlIter& iter) {
    init();
    al::ByamlIter root;
    iter.tryGetIterByKey(&root, "SequenceDemoSkipData");

    al::ByamlIter worldDataArrayIter;
    root.tryGetIterByKey(&worldDataArrayIter, "WorldData");
    for (s32 i = 0; i < worldDataArrayIter.getSize(); i++) {
        al::ByamlIter worldDataIter;
        worldDataArrayIter.tryGetIterByIndex(&worldDataIter, i);
        worldDataIter.tryGetBoolByKey(&mWorldData[i]->isFirstWorldIntroCamera,
                                      "IsFirstWorldIntroCamera");
        al::ByamlIter firstIter;
        worldDataIter.tryGetIterByKey(&firstIter, "IsFirstScenarioStartCamera");
        for (s32 j = 0; j < 10; j++)
            firstIter.tryGetBoolByIndex(&mWorldData[i]->isFirstScenarioStartCamera[j], j);
    }

    root.tryGetBoolByKey(&mIsShowDemoWorldWarpHole, "IsShowDemoWorldWarpHole");
}
