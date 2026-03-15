#include "System/AchievementSaveData.h"

#include <time/seadDateTime.h>

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "Npc/AchievementInfoReader.h"

AchievementSaveData::AchievementSaveData(const AchievementInfoReader* reader)
    : mInfoReader(reader) {
    mEntries = new AchievementEntry[getAchievementNum()];
}

void AchievementSaveData::init() {
    for (s32 i = 0; i < getAchievementNum(); i++)
        mEntries[i] = {};
}

void AchievementSaveData::getAchievement(const char* name) {
    s32 index = mInfoReader->tryFindIndexByName(name);
    mEntries[index].isGotten = true;

    sead::DateTime time;
    time.setNow();
    mEntries[index].getTime = time.getUnixTime();
}

s32 AchievementSaveData::findAchievementIndex(const char* name) const {
    return mInfoReader->tryFindIndexByName(name);
}

bool AchievementSaveData::checkGetAchievement(const char* name) const {
    return checkGetAchievement(findAchievementIndex(name));
}

bool AchievementSaveData::checkGetAchievement(s32 index) const {
    return mEntries[index].isGotten;
}

u64 AchievementSaveData::findAchievementGetTime(s32 index) const {
    return mEntries[index].getTime;
}

s32 AchievementSaveData::calcGetAchievementNum() const {
    s32 getAchievementCount = 0;
    for (s32 i = 0; i < getAchievementNum(); i++)
        getAchievementCount += mEntries[i].isGotten;
    return getAchievementCount;
}

s32 AchievementSaveData::getAchievementNum() const {
    return mInfoReader->size();
}

void AchievementSaveData::write(al::ByamlWriter* writer) {
    writer->pushArray("AchievementSaveData");

    for (s32 i = 0; i < getAchievementNum(); i++) {
        writer->pushHash();
        writer->addBool("IsGet", mEntries[i].isGotten);
        writer->addUInt64("GetTime", mEntries[i].getTime);
        writer->addString("Name", mInfoReader->unsafeAt(i)->name);
        writer->pop();
    }

    writer->pop();
}

void AchievementSaveData::read(const al::ByamlIter& save) {
    init();

    al::ByamlIter achievementsIter{};
    save.tryGetIterByKey(&achievementsIter, "AchievementSaveData");

    for (s32 i = 0; i < achievementsIter.getSize(); i++) {
        al::ByamlIter entryIter{};
        achievementsIter.tryGetIterByIndex(&entryIter, i);

        const char* name = nullptr;
        if (!entryIter.tryGetStringByKey(&name, "Name"))
            break;

        s32 index = mInfoReader->tryFindIndexByName(name);
        if (index == -1)
            continue;

        entryIter.tryGetBoolByKey(&mEntries[index].isGotten, "IsGet");
        entryIter.tryGetUInt64ByKey(&mEntries[index].getTime, "GetTime");
    }
}
