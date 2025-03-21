#pragma once

#include <math/seadVector.h>
#include <prim/seadBitFlag.h>
#include <prim/seadSafeString.h>

class GameProgressData;
class GameDataHolder;

class GameDataFile {
public:
    class HintInfo {
    public:
        enum class HintStatus { NONE, NPC, AMIIBO };

        void clear();

        bool isDisableByWorldWarpHole(bool) const;
        bool isEnableUnlock(s32, bool, s32, bool) const;
        bool isHintStatusUnlock(s32, s32, bool) const;
        bool isHintStatusUnlockByNpc() const;
        bool isHintStatusUnlockByAmiibo() const;
        bool isEnableNameUnlockByScenario(s32, s32, bool) const;

        bool testFunc(s32, bool, s32, bool) const;

    private:
        sead::FixedSafeString<0x80> mStageName;
        sead::FixedSafeString<0x80> mObjId;
        sead::FixedSafeString<0x40> mScenarioName;
        const char* mObjectName;
        sead::Vector3f mTrans;
        sead::Vector3f mTransAgain;
        void* unkPtr1;
        void* unkPtr2;
        void* unkPtr3;
        void* unkPtr4;
        s32 mMainScenarioNo;
        s32 mWorldIndex;
        bool mIsMoonRock;
        bool unkBool1;
        bool mIsAchievement;
        bool mIsGrand;
        bool mIsShopMoon;
        s32 unkInt;
        HintStatus mHintStatus;
        s32 unkInt3;
        s32 unkInt4;
        void* unkPtr6;
        s32 mUniqueID;
        s32 mHintIdx;
        sead::FixedSafeString<0x20> mOptionalID;
        sead::BitFlag32 mProgressBitflag;
        bool unkBool2;
        bool unkBool3;
    };

    bool isGameClear() const;
    bool isUnlockedWorld(s32 index) const;
    bool isAlreadyGoWorld(s32 index) const;
    void wearCostume(const char*);
    void wearCap(const char*);

private:
    char filler0[0x6a8];
    GameProgressData* mGameProgressData;
    char filler1[0x1a0];
    GameDataHolder* mGameDataHolder;
    char filler2[0x198];
    s32 mCurWorldId;
    char filler3[0x170];
};

static_assert(sizeof(GameDataFile) == 0xb68);
