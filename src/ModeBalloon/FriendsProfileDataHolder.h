#pragma once

#include <basis/seadTypes.h>
#include <common/aglGPUMemAddr.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <nn/friends.h>
#include <prim/seadSafeString.h>

namespace nn::ui2d {
class TextureInfo;
}  // namespace nn::ui2d

namespace agl {
class TextureData;
class DrawContext;

class GPUMemBlockBase;
}  // namespace agl

namespace agl::Detail {
class MemoryPool;
}  // namespace agl::Detail

namespace al {
class LiveActor;
class LayoutInitInfo;
struct ActorInitInfo;
class IUseSceneObjHolder;
class FriendsProfileDownloader;
}  // namespace al

class IUseFriendProfileDataHolder {
public:
    virtual u64 getPrincipalID() const = 0;
    virtual const sead::Vector3f& getPos() const = 0;
    virtual s32 getPlayerRank() const = 0;
    virtual const char* getOwnerName() const = 0;
    virtual void played() = 0;
    virtual void broke() = 0;
    virtual void getDataId() const = 0;
    virtual bool isFriendBalloon() const = 0;
    virtual void getOwnerAchievementDataId() const = 0;
    virtual void getPrizeCoin() const = 0;
    virtual void getEntryCoin() const = 0;
    virtual void getRetryCoin() const = 0;
    virtual void setImageTo(al::LiveActor*) const = 0;
    virtual void getTextureForLayout() const = 0;
};

class IFriendsProfileData {
public:
    virtual bool isValidTextureData() const = 0;
    virtual bool isValidNickName() const;
    virtual const char* getNickName() const;
    virtual agl::TextureData* getTextureData() const;
    virtual nn::ui2d::TextureInfo* getTextureInfo() const;
};

class FriendsProfileData : public IFriendsProfileData {
public:
    FriendsProfileData();

    bool isValidTextureData() const override;

    bool isValidNickName() const override { return mIsValidNickName; }

    const char* getNickName() const override { return mNickName.cstr(); }

    agl::TextureData* getTextureData() const override { return mTextureData; }

    nn::ui2d::TextureInfo* getTextureInfo() const override { return mTextureInfo; }

    void clear();

    u64 getId() const { return mId; }

    void setId(u64 id) { mId = id; }

    void setGpuMemAddr(const agl::GPUMemAddrBase& memAddr) { mGpuMemAddr = {memAddr, 0}; }

    void setTextureData(agl::TextureData* data) { mTextureData = data; }

    void setTextureInfo(nn::ui2d::TextureInfo* info) { mTextureInfo = info; }

private:
    u64 mId = 0;
    agl::GPUMemAddrBase mGpuMemAddr;
    agl::TextureData* mTextureData = nullptr;
    nn::ui2d::TextureInfo* mTextureInfo = nullptr;
    sead::FixedSafeString<64> mNickName;
    bool mIsValidNickName = false;
};

class IFriendsProfileDataHolder {
public:
    virtual FriendsProfileData* findProfileData(const IUseFriendProfileDataHolder*) = 0;
    virtual bool tryAddProfileData(const IUseFriendProfileDataHolder*) = 0;
};

class FriendsProfileDataHolder : public IFriendsProfileDataHolder {
public:
    static FriendsProfileDataHolder* create(const al::LayoutInitInfo&, s32, nn::friends::ImageSize);
    static FriendsProfileDataHolder* create(const al::ActorInitInfo&, s32, nn::friends::ImageSize);
    static FriendsProfileDataHolder* create(s32, nn::friends::ImageSize);

    FriendsProfileDataHolder(s32, nn::friends::ImageSize, agl::DrawContext*);
    FriendsProfileData* findProfileData(const IUseFriendProfileDataHolder*) override;
    FriendsProfileData* tryFind(u64) const;
    bool tryAddProfileData(const IUseFriendProfileDataHolder*) override;
    bool tryAdd(u64);
    void clearAll();

private:
    nn::friends::ImageSize mImageSize;
    sead::PtrArray<FriendsProfileData> mProfiles;
};

static_assert(sizeof(FriendsProfileDataHolder) == 0x20);

namespace rs {
void tryCreateFriendsProfileDownloader(const al::IUseSceneObjHolder*);
void tryGetFriendsProfileDownloader(const al::IUseSceneObjHolder*);
void requestDownloadFriendsProfile(void*, const al::IUseSceneObjHolder*, u64,
                                   nn::friends::ImageSize);
void requestDownloadFriendsProfile(void*, al::FriendsProfileDownloader*, u64,
                                   nn::friends::ImageSize);
void isDoneDownloadFriendsProfile(const al::IUseSceneObjHolder*);
void isDoneDownloadFriendsProfile(const al::FriendsProfileDownloader*);
void getFriendsProfileNickname(const al::IUseSceneObjHolder*);
void getFriendsProfileNickname(const al::FriendsProfileDownloader*);
s32 getFriendsProfileImageDataSize(nn::friends::ImageSize);
}  // namespace rs
