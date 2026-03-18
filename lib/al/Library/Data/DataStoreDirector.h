#pragma once

#include <basis/seadTypes.h>

#include "Library/Data/IUseDataStore.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace nn::nex {
class Credentials;
class DataStoreChangeMetaParam;
template <typename T>
class DataStoreClientTemplate;
class DataStoreDeleteParam;
class DataStoreFetchMyInfosParam;
class DataStoreFetchMyInfosResult;
class DataStoreGetMetaParam;
class DataStoreLogicServerClient;
class DataStoreMetaInfo;
class DataStorePersistenceInfo;
class DataStorePrepareGetParam;
class DataStorePreparePostParam;
class DataStorePrepareUpdateParam;
class DataStoreProtocolCallContext;
class DataStoreRateObjectParam;
class DataStoreRatingInfo;
class DataStoreRatingInitParam;
class DataStoreRatingTarget;
class DataStoreSearchBalloonParam;
class DataStoreSearchBalloonResultSet;
class DataStoreSearchParam;
class DataStoreSearchResult;
class ProtocolCallContext;
class qResult;
template <typename T1, typename T2>
class qMap;
template <typename T>
class qVector;
}  // namespace nn::nex

namespace al {
class DataStoreDirector : public ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_alDataStoreDirector;

    DataStoreDirector();
    ~DataStoreDirector() override;

    void init(nn::nex::Credentials* credentials, s32 timeout);
    bool startUpload(const void* buffer, const nn::nex::DataStorePreparePostParam& param);
    bool startUploadMetaBinary(const nn::nex::DataStorePreparePostParam& param);
    bool
    startUploadMetaBinaryArray(const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>& params);
    bool tryEndUpload(bool* isError, u64* dataId);
    bool startUpdate(const void* buffer, const nn::nex::DataStorePrepareUpdateParam& param);
    bool startUpdateMetaBinary(const nn::nex::DataStoreChangeMetaParam& param);
    bool tryEndUpdate(bool* isError);
    bool startDownload(void* buffer, u32 bufferSize,
                       const nn::nex::DataStorePrepareGetParam& param);
    bool startDownloadMeta(nn::nex::DataStoreMetaInfo* metaInfo,
                           const nn::nex::DataStoreGetMetaParam& param);
    bool startDownloadMetaArray(nn::nex::qVector<nn::nex::DataStoreMetaInfo>* metaInfos,
                                const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params);
    bool tryEndDownload(bool* isError);
    bool startSearch(const nn::nex::DataStoreSearchParam& param);
    bool tryEndSearch(bool* isError, nn::nex::DataStoreSearchResult* result);
    bool startDelete(const nn::nex::DataStoreDeleteParam& param);
    bool startDeleteArray(const nn::nex::qVector<nn::nex::DataStoreDeleteParam>& params);
    bool tryEndDelete(bool* isError);
    bool startDownloadRating(nn::nex::DataStoreRatingInfo* ratingInfo, u64 dataId, s8 ratingSlot);
    bool startDownloadRatingArray(
        const nn::nex::qVector<u64>& dataIds,
        nn::nex::qVector<nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>>* ratingInfos);
    bool tryEndDownloadRating(bool* isError);
    bool startUpdateRating(u64 dataId, s8 ratingSlot);
    bool startUpdateRatingArray(const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets);
    bool startUpdateRatingArray(const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
                                const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& params,
                                bool useOption);
    bool tryEndUpdateRating(bool* isError);
    bool startDownloadPersistenceInfo(nn::nex::DataStorePersistenceInfo* persistenceInfo,
                                      u64 principalId, u16 slotId);
    bool tryEndDownloadPersistenceInfo(bool* isError);
    bool startPerpetuateObject(u16 slotId, u64 dataId);
    bool tryEndPerpetuateObject(bool* isError);
    bool startSearchBalloon(const nn::nex::DataStoreSearchBalloonParam& param,
                            nn::nex::qVector<nn::nex::DataStoreSearchBalloonResultSet>* resultSets);
    bool tryEndSearchBalloon(bool* isError);
    bool startFetchMyInfos(const nn::nex::DataStoreFetchMyInfosParam& param,
                           nn::nex::DataStoreFetchMyInfosResult* result);
    bool tryEndFetchMyInfos(bool* isError);
    s32 calcLastNetworkErrorCode() const;
    bool tryCancel();
    const char* getSceneObjName() const override;

private:
    nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>* mDataStoreClient =
        nullptr;
    nn::nex::ProtocolCallContext* mProtocolCallContext = nullptr;
    nn::nex::DataStoreSearchResult* mSearchResult = nullptr;
    u64 mDataId = 0;
    nn::nex::qResult* mLastResult = nullptr;
    bool mIsInitialized = false;
};

u64 getPrincipalID(const IUseDataStore* user);
bool isAvailableDataStore(const IUseDataStore* user);
void startDataStoreSearchPublic(const IUseDataStore* user, u16 slotId, u32 offset, u32 count);
void startDataStoreSearchOwn(const IUseDataStore* user, u16 slotId, u32 offset, u32 count);
void startDataStoreSearchOwnList(const IUseDataStore* user, const nn::nex::qVector<u16>& slotIds,
                                 u32 offset, u32 count);
bool tryEndDataStoreSearch(bool* isError, nn::nex::DataStoreSearchResult* result,
                           const IUseDataStore* user);
void startDataStoreUploadPrivate(const IUseDataStore* user, u16 slotId, const void* buffer,
                                 u32 bufferSize);
void startDataStoreUploadPublicReadOnly(const IUseDataStore* user, u16 slotId, const void* buffer,
                                        u32 bufferSize);
void startDataStoreUploadMetaBinaryPublic(
    const IUseDataStore* user, u16 slotId, const void* metaBinary, u32 metaBinarySize,
    u16 metaSlotId, const nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>* ratingInitParams);
void startDataStoreUploadEmptyPublic(
    const IUseDataStore* user, u16 slotId,
    const nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>* ratingInitParams);
bool tryEndDataStoreUpload(bool* isError, u64* dataId, const IUseDataStore* user);
bool startDataStoreUpdate(const IUseDataStore* user, const void* buffer, u32 bufferSize,
                          u64 dataId);
bool startDataStoreUpdateMetaBinary(const IUseDataStore* user, const void* metaBinary,
                                    u32 metaBinarySize, u64 dataId);
bool startDataStoreUpdateMetaBinaryArray(
    const IUseDataStore* user, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>& params);
bool tryEndDataStoreUpdate(bool* isError, const IUseDataStore* user);
bool startDataStoreDownload(const IUseDataStore* user, void* buffer, u32 bufferSize, u64 dataId);
bool startDataStoreDownloadPersistence(const IUseDataStore* user, void* buffer, u32 bufferSize,
                                       u16 slotId);
bool startDataStoreDownloadMeta(const IUseDataStore* user, nn::nex::DataStoreMetaInfo* metaInfo,
                                u64 dataId);
bool startDataStoreDownloadMetaArray(
    const IUseDataStore* user, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* metaInfos,
    const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params);
bool tryEndDataStoreDownload(bool* isError, const IUseDataStore* user);
bool startDataStoreDelete(const IUseDataStore* user, u64 dataId);
bool startDataStoreDeleteArray(const IUseDataStore* user, const u64* dataIds, s32 dataIdCount);
bool tryEndDataStoreDelete(bool* isError, const IUseDataStore* user);
bool startDataStoreDownloadRating(const IUseDataStore* user,
                                  nn::nex::DataStoreRatingInfo* ratingInfo, s8 ratingSlot,
                                  u64 dataId);
bool startDataStoreDownloadRatingArray(
    const IUseDataStore* user, const nn::nex::qVector<u64>& dataIds,
    nn::nex::qVector<nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>>* ratingInfos);
bool tryEndDataStoreDownloadRating(bool* isError, const IUseDataStore* user);
bool startDataStoreUpdateRating(const IUseDataStore* user, s8 ratingSlot, u64 dataId);
bool startDataStoreUpdateRatingArray(
    const IUseDataStore* user, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets);
bool startDataStoreUpdateRatingArray(
    const IUseDataStore* user, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
    const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& params, bool useOption);
bool tryEndDataStoreUpdateRating(bool* isError, const IUseDataStore* user);
bool startDataStoreDownloadPersistenceInfo(const IUseDataStore* user,
                                           nn::nex::DataStorePersistenceInfo* persistenceInfo,
                                           u16 slotId);
bool tryEndDownloadPersistenceInfo(bool* isError, const IUseDataStore* user);
bool startDataStorePerpetuateObject(const IUseDataStore* user, u16 slotId, u64 dataId);
bool tryEndDataStorePerpetuateObject(bool* isError, const IUseDataStore* user);
bool startSearchBalloon(const IUseDataStore* user,
                        const nn::nex::DataStoreSearchBalloonParam& param,
                        nn::nex::qVector<nn::nex::DataStoreSearchBalloonResultSet>* resultSets);
bool tryEndSearchBalloon(bool* isError, const IUseDataStore* user);
bool startFetchMyInfos(const IUseDataStore* user, const nn::nex::DataStoreFetchMyInfosParam& param,
                       nn::nex::DataStoreFetchMyInfosResult* result);
bool tryEndFetchMyInfos(bool* isError, const IUseDataStore* user);
bool tryCancelDataStoreProcess(const IUseDataStore* user);
s32 calcDataStoreLastNetworkErrorCode(const IUseDataStore* user);
bool isDataStoreLastNetworkErrorCodeNotFound(const IUseDataStore* user);
bool isDataStoreLastNetworkErrorCodeShouldHandling(const IUseDataStore* user);

static_assert(sizeof(DataStoreDirector) == 0x38);

}  // namespace al
