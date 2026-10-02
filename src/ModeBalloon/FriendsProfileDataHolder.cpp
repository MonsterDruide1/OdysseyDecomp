#include "ModeBalloon/FriendsProfileDataHolder.h"

#include <common/aglTextureData.h>
#include <common/aglTextureEnum.h>

#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Project/Memory/Util.h"

FriendsProfileData::FriendsProfileData() = default;

bool FriendsProfileData::isValidTextureData() const {
    return mId && mTextureData && mTextureData->get_0();
}

void FriendsProfileData::clear() {
    mIsValidNickName = false;
    mId = 0;
    mNickName.clear();
}

FriendsProfileDataHolder* FriendsProfileDataHolder::create(const al::LayoutInitInfo& initInfo,
                                                           s32 size,
                                                           nn::friends::ImageSize imageSize) {
    return new FriendsProfileDataHolder(size, imageSize, nullptr);
}

FriendsProfileDataHolder* FriendsProfileDataHolder::create(const al::ActorInitInfo& initInfo,
                                                           s32 size,
                                                           nn::friends::ImageSize imageSize) {
    return new FriendsProfileDataHolder(
        size, imageSize, initInfo.actorSceneInfo.graphicsSystemInfo->getDrawContext());
}

FriendsProfileDataHolder* FriendsProfileDataHolder::create(s32 size,
                                                           nn::friends::ImageSize imageSize) {
    return new FriendsProfileDataHolder(size, imageSize, nullptr);
}

static inline u32 getImageSize(nn::friends::ImageSize imageSize) {
    if (imageSize == nn::friends::ImageSize_64x64)
        return 64;

    if (imageSize == nn::friends::ImageSize_128x128)
        return 128;

    return 256;
}

FriendsProfileDataHolder::FriendsProfileDataHolder(s32 size, nn::friends::ImageSize imageSize,
                                                   agl::DrawContext* drawContext)
    : mImageSize(imageSize) {
    mProfiles.allocBuffer(size, nullptr);

    for (s32 i = 0; i < mProfiles.capacity(); i++) {
        FriendsProfileData* profile = new FriendsProfileData();
        profile->setId(0);

        profile->setGpuMemAddr({*agl::GPUMemBlockT<u8>::create(
                                    rs::getFriendsProfileImageDataSize(mImageSize),
                                    al::getCurrentHeap(), 0x1000, agl::MemoryAttribute::Default),
                                0});

        profile->setTextureData(agl::TextureData::create(
            agl::TextureType::cTextureType_2D, agl::TextureFormat::cTextureFormat_R8_G8_B8_A8_SRGB,
            getImageSize(mImageSize), getImageSize(mImageSize), 1, 1,
            agl::TextureAttribute::cTextureAttribute_1, agl::MultiSampleType::cMultiSampleType_0,
            true));

        profile->setTextureInfo(al::createTextureInfo());

        mProfiles.pushBack(profile);
    }
}

FriendsProfileData*
FriendsProfileDataHolder::findProfileData(const IUseFriendProfileDataHolder* holder) {
    return tryFind(holder->getPrincipalID());
}

FriendsProfileData* FriendsProfileDataHolder::tryFind(u64 id) const {
    for (auto it = mProfiles.begin(); it != mProfiles.end(); ++it)
        if (it->getId() == id)
            return &(*it);
    return nullptr;
}

bool FriendsProfileDataHolder::tryAddProfileData(const IUseFriendProfileDataHolder* holder) {
    return tryAdd(holder->getPrincipalID());
}

bool FriendsProfileDataHolder::tryAdd(u64 id) {
    if (tryFind(id))
        return false;

    for (auto it = mProfiles.begin(); it != mProfiles.end(); ++it) {
        if (it->getId() == 0) {
            it->setId(id);
            return true;
        }
    }

    return false;
}

void FriendsProfileDataHolder::clearAll() {
    for (auto it = mProfiles.begin(); it != mProfiles.end(); ++it)
        it->clear();
}
