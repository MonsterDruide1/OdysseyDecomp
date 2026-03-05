#include "System/AchievementSaveData.h"

#include <time/seadDateTime.h>

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "Npc/AchievementInfoReader.h"

AchievementSaveData::AchievementSaveData(const AchievementInfoReader* reader)
    : mInfoReader(reader) {
    mEntries = new AchievementEntry[mInfoReader->size()];
}

void AchievementSaveData::init() {
    for (s32 i = 0; i < mInfoReader->size(); i++)
        mEntries[i] = {};
}

void AchievementSaveData::getAchievement(const char* name) {
    s32 index = mInfoReader->tryFindIndexByName(name);
    mEntries[index].isGotten = true;

    sead::DateTime time(0LL);
    time.setNow();
    mEntries[index].getTime = time.getUnixTime();
}

s32 AchievementSaveData::findAchievementIndex(const char* name) const {
    return mInfoReader->tryFindIndexByName(name);
}

bool AchievementSaveData::checkGetAchievement(const char* name) const {
    s32 index = mInfoReader->tryFindIndexByName(name);
    return mEntries[index].isGotten;
}

bool AchievementSaveData::checkGetAchievement(s32 index) const {
    return mEntries[index].isGotten;
}

s64 AchievementSaveData::findAchievementGetTime(s32 index) const {
    return static_cast<s64>(mEntries[index].getTime);
}

s32 AchievementSaveData::calcGetAchievementNum() const {
    s32 achievementNum = 0;
    for (s32 i = 0; i < mInfoReader->size(); i++)
        achievementNum += mEntries[i].isGotten;
    return achievementNum;
}

s32 AchievementSaveData::getAchievementNum() const {
    return mInfoReader->size();
}

void AchievementSaveData::write(al::ByamlWriter* writer) {
    writer->pushArray("AchievementSaveData");

    for (s32 i = 0; i < mInfoReader->size(); i++) {
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
