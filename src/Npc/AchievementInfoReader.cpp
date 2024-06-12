#include "Npc/AchievementInfoReader.h"

#include "Library/Base/StringUtil.h"
#include "Library/Resource/ResourceUtil.h"
#include "Library/Yaml/ByamlIter.h"

AchievementInfoReader::AchievementInfoReader() = default;

void AchievementInfoReader::init() {  // TODO minor mismatches during loop
    al::Resource* achievementInfoResource =
        al::findOrCreateResource("SystemData/AchievementInfo", nullptr);
    const char* byamlFileName = al::StringTmp<256>{"%s.byml", "AchievementInfo"}.cstr();

    if (!achievementInfoResource->isExistFile(byamlFileName))
        return;

    al::ByamlIter achievementInfo = achievementInfoResource->getByml("AchievementInfo");
    al::ByamlIter achievementInfoArray;
    if (achievementInfo.tryGetIterByKey(&achievementInfoArray, "AchievementInfoArray")) {
        auto size = achievementInfoArray.getSize();
        mAchievements.allocBuffer(size, nullptr);

        for (u32 i = 0; i < size; i++) {
            al::ByamlIter iter;
            if (achievementInfoArray.tryGetIterByIndex(&iter, i)) {
                const char* name = nullptr;
                iter.tryGetStringByKey(&name, "Name");
                const char* note = nullptr;
                iter.tryGetStringByKey(&note, "Note");
                s32 num = 1;
                iter.tryGetIntByKey(&num, "Num");
                s32 level = -1;
                iter.tryGetIntByKey(&level, "Level");

                mAchievements.pushBack(new AchievementInfo(name, num, level, note));
            }
        }
    }
}

s32 AchievementInfoReader::tryFindIndexByName(const char* name) const {
    for (s32 i = 0; i < mAchievements.size(); i++)
        if (al::isEqualString(name, mAchievements[i]->mName))
            return i;
    return -1;
}
