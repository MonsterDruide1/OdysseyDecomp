#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>
#include <prim/seadSafeString.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Ranking/IUseRanking.h"

#include "ModeBalloon/RankingCategory.h"

namespace al {
struct ActorInitInfo;
class IUseDataStore;
class RankingDirector;
class RankingStateUploadCommonData;
class RankingStateUploadScore;
}  // namespace al

namespace nn::nex {
class BufferQueueParam;
class qBuffer;
class DataStoreRatingInitParam;
template <typename T1, typename T2>
class qMap;
template <typename T>
class qVector;
}  // namespace nn::nex

namespace TimeBalloon {
class BalloonDataServer;
class NoticeDataServer;
}  // namespace TimeBalloon

class NexRankingUploader : public al::IUseRanking, public al::LiveActor {
public:
    NexRankingUploader(const char*, const char*);
    virtual ~NexRankingUploader();

    void init(const al::ActorInitInfo&) override;
    al::RankingDirector* getRankingDirector() const override;
    bool isAvailableRanking() const override;
    void uploadScore(u32, RankingCategory, bool);
    void uploadName(const sead::WFixedSafeString<128>&);
    void uploadAccountNickname();
    bool tryLoadNickname();
    bool isSuccess() const;
    bool isFailure() const;
    void exeUploadScore();
    void exeUploadCommonData();
    void exeEnd();
    void requestInitializeNetwork(al::IUseRanking*);

private:
    al::RankingStateUploadScore* _110;
    al::RankingStateUploadCommonData* _118;
    sead::FixedSafeString<128>* _120;
    const char* _128;
};

namespace rs {
void restorePrincipalId(u64*, const nn::nex::qBuffer*);
void startUploadTimeBalloonNoticeData(al::IUseDataStore*, const TimeBalloon::NoticeDataServer&);
bool tryEndUploadTimeBalloonNoticeData(bool*, u64*, const al::IUseDataStore*);
bool tryStartAddTimeBalloonHistory(al::IUseDataStore*, u64, u32, u64);
bool tryStartAddTimeBalloonHistoryList(al::IUseDataStore*,
                                       const nn::nex::qVector<nn::nex::BufferQueueParam>&,
                                       const nn::nex::qVector<nn::nex::qBuffer>&);
bool tryEndAddTimeBalloonHistory(bool*, al::IUseDataStore*);
bool tryStartGetTimeBalloonHistory(al::IUseDataStore*, nn::nex::qVector<nn::nex::qBuffer>*, u64,
                                   u32);
bool tryStartGetTimeBalloonHistoryList(al::IUseDataStore*,
                                       const nn::nex::qVector<nn::nex::BufferQueueParam>&,
                                       const nn::nex::qVector<nn::nex::qBuffer>&);
bool tryEndGetTimeBalloonHistory(bool*, const al::IUseDataStore*);
void startUploadTimeBalloonBalloonData(al::IUseDataStore*, const TimeBalloon::BalloonDataServer&,
                                       nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>*);
bool tryEndUploadTimeBalloonBalloonData(bool*, u64*, const al::IUseDataStore*);
bool tryStartUploadBalloonFindBalloonData(
    al::IUseDataStore*, u16, const void*, s32,
    const nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>*);
bool tryStartUpdateRatingPlayedBalloon(al::IUseDataStore*, u64, u64, bool);
bool tryStartUpdateRatingBrokeBalloon(al::IUseDataStore*, u64, u64, u64, s32, s32);
bool tryEndUpdateRating(bool*, al::IUseDataStore*);
}  // namespace rs

static_assert(sizeof(NexRankingUploader) == 0x130);
