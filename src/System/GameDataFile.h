#pragma once

#include <math/seadVector.h>
#include <prim/seadBitFlag.h>
#include <time/seadDateTime.h>

#include "System/UniqObjInfo.h"

constexpr s32 sNumWorlds = 20;

namespace al {
class PlacementId;
}  // namespace al

class GameDataHolder;
class GameProgressData;
class SphinxQuizData;

class GameDataFile {
public:
    struct SaveObjS32 {
        UniqObjInfo objInfo;
        s32 value;
    };

    static_assert(sizeof(GameDataFile::SaveObjS32) == 0x138);

    enum class AchievementStatus { None, Unlocked, Got };

    struct HintInfo {
        enum class HintStatus { NONE, NPC, AMIIBO };

        void clear();

        bool isDisableByWorldWarpHole(bool condition) const;
        bool isEnableUnlock(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlock(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlockByNpc() const;
        bool isHintStatusUnlockByAmiibo() const;
        bool isEnableNameUnlockByScenario(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;

        bool testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;

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
        AchievementStatus mAchievementStatus;
        s32 unkInt4;
        void* unkPtr6;
        s32 mUniqueID;
        s32 mHintIdx;
        sead::FixedSafeString<0x20> mOptionalID;
        sead::BitFlag32 mProgressBitflag;
        bool mIsDisabled;
        bool unkBool3;
    };

    template <typename T, s32 Size>
    class FixedHeapArray {
    public:
        void alloc() { mPtr = new T[Size]; }

        s32 size() const { return Size; }

        T& operator[](s32 index) { return mPtr[index]; }

        const T& operator[](s32 index) const { return mPtr[index]; }

        T* begin() const { return mPtr; }

        T* end() const { return mPtr + Size; }

    private:
        T* mPtr = nullptr;
    };

    void unlockAchievementShineName();
    bool isKidsMode() const;
    void updateWorldMapIndex();
    void updateWorldWarpIndex();
    s32 getScenarioNo(s32 worldId) const;
    s32 getMainScenarioNo(s32 worldId) const;
    bool isEmpty() const;
    bool isGameClear() const;
    bool isUnlockedWorld(s32 worldId) const;
    bool isAlreadyGoWorld(s32 worldId) const;
    void updateSaveTimeForDisp();
    sead::DateTime getLastUpdateTime() const;
    void generateSaveDataIdForPrepoForWrite();
    bool checkIsHomeStage(const char* stageName) const;
    const char* getStageNameCurrent() const;
    const char* tryGetStageNameCurrent() const;
    const char* getStageNameNext() const;
    s32 getTotalShopShineNum() const;
    s32 getPayShineNum(s32 worldId) const;
    bool isStartedObj(const char* stageName, const char* objId) const;
    const SaveObjS32* tryFindSaveObjS32(const al::PlacementId* placementId);
    void useCoinCollect(s32 purplesUsed);
    void answerCorrectSphinxQuiz();
    void answerCorrectSphinxQuizAll();
    bool isAnswerCorrectSphinxQuiz(s32 index) const;
    bool isAnswerCorrectSphinxQuizAll(s32 index) const;
    bool isTalkAlreadyLocalLanguage() const;
    s32 calcGetCheckpointNum() const;
    s32 calcWorldWarpHoleThroughNum() const;
    void wearCostume(const char* name);
    void wearCap(const char* name);
    void enableHintById(s32 shineIndex);

private:
    char _000[24];
    FixedHeapArray<s32, sNumWorlds> mShopShineNum;
    FixedHeapArray<s32, sNumWorlds> mMainScenarioNo;
    char _028[768];
    sead::FixedSafeString<128> mCurrentStageName;
    char _3c0[160];
    FixedHeapArray<bool, sNumWorlds> mIsWorldWarpHoleThrough;
    sead::DateTime mSaveTimeForDisp;
    sead::DateTime mSaveTime;
    char _478[8];
    s64 mSaveDataIdForPrepoForWrite;
    FixedHeapArray<UniqObjInfo, 320> mGotCheckpoint;
    char _490[280];
    FixedHeapArray<s32, sNumWorlds> mPayShineNum;
    char _5b0[16];
    FixedHeapArray<UniqObjInfo, 512> mStartedObj;
    FixedHeapArray<SaveObjS32, 256> mSaveObjS32;
    char _5d0[24];
    FixedHeapArray<s32, sNumWorlds> mUseCoinCollectNum;
    FixedHeapArray<s32, sNumWorlds> mUnlockedWorldId;
    bool mIsEmpty;
    char _5f9[23];
    SphinxQuizData* mSphinxQuizData;
    FixedHeapArray<bool, sNumWorlds> mIsTalkLocalLanguage;
    char _620[115];
    bool mHasUnlockedAchievementNames;
    char _694[20];
    GameProgressData* mGameProgressData;
    char _6b0[96];
    sead::FixedSafeString<64> mCurrentCostumeName;
    sead::FixedSafeString<64> mCurrentCapName;
    char _7c0[24];
    FixedHeapArray<s32, sNumWorlds> mScenarioNo;
    char _7e0[72];
    bool mIsAssistMode;
    char _829[39];
    GameDataHolder* mGameDataHolder;
    char _858[16];
    sead::BufferedSafeString mNextStageName;
    char _880[288];
    FixedHeapArray<HintInfo, 1024> mHintList;
    char _9a8[72];
    s32 mCurrentWorldId;
    char _9f4[332];
    FixedHeapArray<s32, sNumWorlds> mWorldMapIndex;
    s32 mWorldMapNum;
    char _b4c[4];
    FixedHeapArray<s32, sNumWorlds> mWorldWarpIndex;
    s32 mWorldWarpNum;
    char _b5c[12];

    inline s32 getCurrentWorldIdNoDevelop() const;
};

static_assert(sizeof(GameDataFile) == 0xb68);
