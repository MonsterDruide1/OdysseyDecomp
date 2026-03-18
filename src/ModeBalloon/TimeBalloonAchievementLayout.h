#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

class IBalloonFindMyAchievementHolder;

namespace al {
class LayoutInitInfo;
class FriendsProfileData;
}  // namespace al

namespace nn {
namespace nex {
template <typename K, typename V>
class qMap;
template <typename T>
class qVector;
class qBuffer;
struct DataStoreRatingInfo;
struct DataStoreSearchBalloonResult;
}  // namespace nex
}  // namespace nn

class FriendsProfileDataHolder;

namespace TimeBalloon {
class BalloonDataServer {
public:
    BalloonDataServer();
    virtual ~BalloonDataServer();
};

class BalloonData;
struct BalloonDataMetaBinary;

void initBalloonDataServer(BalloonDataServer*);

class RankAchievement {
public:
    void end();
};

void initBalloonDataServer(BalloonDataServer* server);

// BalloonData API
void setRatingInfo(BalloonData*, const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>&);
void setRatingInfo(BalloonData*, s32);
void setGotIdList(BalloonData*, const nn::nex::qVector<nn::nex::qBuffer>&);
void setPlayedIdList(BalloonData*, const nn::nex::qVector<nn::nex::qBuffer>&);
const nn::nex::qVector<nn::nex::qBuffer>& getGotIdList(const BalloonData&);
const nn::nex::qVector<nn::nex::qBuffer>& getPlayedIdList(const BalloonData&);

bool isGotBalloon(const BalloonData&);
s32 getPlayerRank(const BalloonData&);
s32 getPlayedNum(const BalloonData&);
s32 calcGotCoin(const BalloonData&);
s32 calcProtectNum(const BalloonData&);
s32 calcProtectCoin(const BalloonData&);
s32 calcProtectCoin(s32);
al::FriendsProfileData* tryGetProfileData(const BalloonData&);
const char* tryGetNickName(const BalloonData&);
const char* tryGetNickName(const char** out, const BalloonData&);
sead::Vector3f getBalloonPos(const BalloonData&);

bool createBalloonData(BalloonData*, const nn::nex::DataStoreSearchBalloonResult&,
                       FriendsProfileDataHolder*, bool);
bool createBalloonData(BalloonData*, u64, u64, u16,
                       const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>&,
                       const BalloonDataMetaBinary*, FriendsProfileDataHolder*, bool);

bool createBalloonData(BalloonData*, u64, u64, u16,
                       const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>&,
                       const BalloonDataMetaBinary*, FriendsProfileDataHolder*, bool);

s32 getBalloonGotSlotId();
s32 getBalloonPlayedSlotId();

}  // namespace TimeBalloon

class TimeBalloonAchievementLayout : public al::SimpleLayoutAppearWaitEnd {
public:
    TimeBalloonAchievementLayout(const al::LayoutInitInfo& info);

    void appear() override;
    void update();
    void setAchievement(const IBalloonFindMyAchievementHolder* holder);

private:
    u8 _130[0x288 - 0x130];
};

static_assert(sizeof(TimeBalloonAchievementLayout) == 0x288);
