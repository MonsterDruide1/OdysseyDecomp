#include "game/Player/PlayerModelHolder.h"

#include "al/Library/Base/String.h"

PlayerModelHolder::PlayerModelHolder(unsigned int bufferSize) {
    mBuffer.allocBuffer(bufferSize, nullptr);
}

void PlayerModelHolder::registerModel(al::LiveActor* liveActor, const char* name) {
    mBuffer.pushBack(new Entry{sead::FixedSafeString<128>(name), liveActor});
}

void PlayerModelHolder::changeModel(const char* name) {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it) {
        if (al::isEqualString(it->mName, sead::SafeString(name))) {
            currentModel = &*it;
            return;
        }
    }
}

al::LiveActor* PlayerModelHolder::findModelActor(const char* name) const {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it) {
        if (al::isEqualString(it->mName, sead::SafeString(name))) {
            return it->mLiveActor;
        }
    }
    return currentModel->mLiveActor;
}

al::LiveActor* PlayerModelHolder::tryFindModelActor(const char* name) const {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it) {
        if (al::isEqualString(it->mName, sead::SafeString(name))) {
            return it->mLiveActor;
        }
    }
    return nullptr;
}

bool PlayerModelHolder::isCurrentModelLabel(const char* name) const {
    return al::isEqualString(currentModel->mName.cstr(), name);
}

bool PlayerModelHolder::isCurrentModelLabelSubString(const char* name) const {
    return al::isEqualSubString(currentModel->mName.cstr(), name);
}
