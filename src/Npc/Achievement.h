#pragma once

class AchievementInfo;
class GameDataHolderAccessor;

class Achievement {
public:
    Achievement(const AchievementInfo*);
    bool isGet(GameDataHolderAccessor) const;

private:
    bool mIsGet = false;
    const AchievementInfo* mInfo;
};
