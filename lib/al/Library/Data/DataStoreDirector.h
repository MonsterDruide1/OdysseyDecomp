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

    void init(nn::nex::Credentials*, s32);
    bool startUpload(const void*, const nn::nex::DataStorePreparePostParam&);
    bool startUploadMetaBinary(const nn::nex::DataStorePreparePostParam&);
    bool startUploadMetaBinaryArray(const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>&);
    bool tryEndUpload(bool*, u64*);
    bool startUpdate(const void*, const nn::nex::DataStorePrepareUpdateParam&);
    bool startUpdateMetaBinary(const nn::nex::DataStoreChangeMetaParam&);
    bool tryEndUpdate(bool*);
    bool startDownload(void*, u32, const nn::nex::DataStorePrepareGetParam&);
    bool startDownloadMeta(nn::nex::DataStoreMetaInfo*, const nn::nex::DataStoreGetMetaParam&);
    bool startDownloadMetaArray(nn::nex::qVector<nn::nex::DataStoreMetaInfo>*,
                                const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>&);
    bool tryEndDownload(bool*);
    bool startSearch(const nn::nex::DataStoreSearchParam&);
    bool tryEndSearch(bool*, nn::nex::DataStoreSearchResult*);
    bool startDelete(const nn::nex::DataStoreDeleteParam&);
    bool startDeleteArray(const nn::nex::qVector<nn::nex::DataStoreDeleteParam>&);
    bool tryEndDelete(bool*);
    bool startDownloadRating(nn::nex::DataStoreRatingInfo*, u64, s8);
    bool
    startDownloadRatingArray(const nn::nex::qVector<u64>&,
                             nn::nex::qVector<nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>>*);
    bool tryEndDownloadRating(bool*);
    bool startUpdateRating(u64, s8);
    bool startUpdateRatingArray(const nn::nex::qVector<nn::nex::DataStoreRatingTarget>&);
    bool startUpdateRatingArray(const nn::nex::qVector<nn::nex::DataStoreRatingTarget>&,
                                const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>&, bool);
    bool tryEndUpdateRating(bool*);
    bool startDownloadPersistenceInfo(nn::nex::DataStorePersistenceInfo*, u64, u16);
    bool tryEndDownloadPersistenceInfo(bool*);
    bool startPerpetuateObject(u16, u64);
    bool tryEndPerpetuateObject(bool*);
    bool startSearchBalloon(const nn::nex::DataStoreSearchBalloonParam&,
                            nn::nex::qVector<nn::nex::DataStoreSearchBalloonResultSet>*);
    bool tryEndSearchBalloon(bool*);
    bool startFetchMyInfos(const nn::nex::DataStoreFetchMyInfosParam&,
                           nn::nex::DataStoreFetchMyInfosResult*);
    bool tryEndFetchMyInfos(bool*);
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

u64 getPrincipalID(const IUseDataStore*);
bool isAvailableDataStore(const IUseDataStore*);
void startDataStoreSearchPublic(const IUseDataStore*, u16, u32, u32);
void startDataStoreSearchOwn(const IUseDataStore*, u16, u32, u32);
void startDataStoreSearchOwnList(const IUseDataStore*, const nn::nex::qVector<u16>&, u32, u32);
bool tryEndDataStoreSearch(bool*, nn::nex::DataStoreSearchResult*, const IUseDataStore*);
void startDataStoreUploadPrivate(const IUseDataStore*, u16, const void*, u32);
void startDataStoreUploadPublicReadOnly(const IUseDataStore*, u16, const void*, u32);
void startDataStoreUploadMetaBinaryPublic(
    const IUseDataStore*, u16, const void*, u32, u16,
    const nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>*);
void startDataStoreUploadEmptyPublic(const IUseDataStore*, u16,
                                     const nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam>*);
bool tryEndDataStoreUpload(bool*, u64*, const IUseDataStore*);
bool startDataStoreUpdate(const IUseDataStore*, const void*, u32, u64);
bool startDataStoreUpdateMetaBinary(const IUseDataStore*, const void*, u32, u64);
bool startDataStoreUpdateMetaBinaryArray(
    const IUseDataStore*, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>&);
bool tryEndDataStoreUpdate(bool*, const IUseDataStore*);
bool startDataStoreDownload(const IUseDataStore*, void*, u32, u64);
bool startDataStoreDownloadPersistence(const IUseDataStore*, void*, u32, u16);
bool startDataStoreDownloadMeta(const IUseDataStore*, nn::nex::DataStoreMetaInfo*, u64);
bool startDataStoreDownloadMetaArray(const IUseDataStore*,
                                     nn::nex::qVector<nn::nex::DataStoreMetaInfo>*,
                                     const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>&);
bool tryEndDataStoreDownload(bool*, const IUseDataStore*);
bool startDataStoreDelete(const IUseDataStore*, u64);
bool startDataStoreDeleteArray(const IUseDataStore*, const u64*, s32);
bool tryEndDataStoreDelete(bool*, const IUseDataStore*);
bool startDataStoreDownloadRating(const IUseDataStore*, nn::nex::DataStoreRatingInfo*, s8, u64);
bool startDataStoreDownloadRatingArray(
    const IUseDataStore*, const nn::nex::qVector<u64>&,
    nn::nex::qVector<nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>>*);
bool tryEndDataStoreDownloadRating(bool*, const IUseDataStore*);
bool startDataStoreUpdateRating(const IUseDataStore*, s8, u64);
bool startDataStoreUpdateRatingArray(const IUseDataStore*,
                                     const nn::nex::qVector<nn::nex::DataStoreRatingTarget>&);
bool startDataStoreUpdateRatingArray(const IUseDataStore*,
                                     const nn::nex::qVector<nn::nex::DataStoreRatingTarget>&,
                                     const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>&,
                                     bool);
bool tryEndDataStoreUpdateRating(bool*, const IUseDataStore*);
bool startDataStoreDownloadPersistenceInfo(const IUseDataStore*, nn::nex::DataStorePersistenceInfo*,
                                           u16);
bool tryEndDownloadPersistenceInfo(bool*, const IUseDataStore*);
bool startDataStorePerpetuateObject(const IUseDataStore*, u16, u64);
bool tryEndDataStorePerpetuateObject(bool*, const IUseDataStore*);
bool startSearchBalloon(const IUseDataStore*, const nn::nex::DataStoreSearchBalloonParam&,
                        nn::nex::qVector<nn::nex::DataStoreSearchBalloonResultSet>*);
bool tryEndSearchBalloon(bool*, const IUseDataStore*);
bool startFetchMyInfos(const IUseDataStore*, const nn::nex::DataStoreFetchMyInfosParam&,
                       nn::nex::DataStoreFetchMyInfosResult*);
bool tryEndFetchMyInfos(bool*, const IUseDataStore*);
bool tryCancelDataStoreProcess(const IUseDataStore*);
s32 calcDataStoreLastNetworkErrorCode(const IUseDataStore*);
bool isDataStoreLastNetworkErrorCodeNotFound(const IUseDataStore*);
bool isDataStoreLastNetworkErrorCodeShouldHandling(const IUseDataStore*);

static_assert(sizeof(DataStoreDirector) == 0x38);

}  // namespace al
