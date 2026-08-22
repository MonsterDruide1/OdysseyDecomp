#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class FriendsProfileDataHolder;

namespace al {
class FriendsProfileData;
}  // namespace al

namespace nn::nex {
template <typename K, typename V>
class qMap;
template <typename T>
class qVector;
class qBuffer;
struct DataStoreRatingInfo;
struct DataStoreSearchBalloonResult;
}  // namespace nn::nex

namespace TimeBalloon {
class BalloonDataServer;

class BalloonData {
public:
    void setRatingInfo(const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>& ratingInfoMap);
    void setGotIdList(const nn::nex::qVector<nn::nex::qBuffer>& gotIdList);
    void setPlayedIdList(const nn::nex::qVector<nn::nex::qBuffer>& playedIdList);
    const nn::nex::qVector<nn::nex::qBuffer>& getGotIdList() const;
    const nn::nex::qVector<nn::nex::qBuffer>& getPlayedIdList() const;
};

struct BalloonDataMetaBinary;

class RankAchievement {
public:
    void end();
};

void initBalloonDataServer(BalloonDataServer* server);

bool isGotBalloon(const BalloonData& balloonData);
s32 getPlayerRank(const BalloonData& balloonData);
s64 getPlayedNum(const BalloonData& balloonData);
s32 calcGotCoin(const BalloonData& balloonData);
s32 calcProtectNum(const BalloonData& balloonData);
s64 calcProtectCoin(const BalloonData& balloonData);
s64 calcProtectCoin(s32 protectNum);
al::FriendsProfileData* tryGetProfileData(const BalloonData& balloonData);
void tryGetNickName(const char** outNickName, const BalloonData& balloonData);
const char* tryGetNickName(const BalloonData& balloonData);
void getBalloonPos(sead::Vector3f* outBalloonPos, const BalloonData& balloonData);

void setRatingInfo(BalloonData* balloonData,
                   const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>& ratingInfoMap);
void createBalloonData(BalloonData* balloonData,
                       const nn::nex::DataStoreSearchBalloonResult& searchBalloonResult,
                       FriendsProfileDataHolder* profileDataHolder, bool isGotBalloon);
void createBalloonData(BalloonData* balloonData, u64 dataId, u64 ownerId, u16 playerRank,
                       const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>& ratingInfoMap,
                       const BalloonDataMetaBinary* metaBinary,
                       FriendsProfileDataHolder* profileDataHolder, bool isGotBalloon);

s32 getBalloonGotSlotId();
s32 getBalloonPlayedSlotId();

}  // namespace TimeBalloon
