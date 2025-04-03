#include "Player/PlayerModelHolder.h"

#include "Library/Base/StringUtil.h"

PlayerModelHolder::PlayerModelHolder(u32 bufferSize) {
    mBuffer.allocBuffer(bufferSize, nullptr);
}

void PlayerModelHolder::registerModel(al::LiveActor* liveActor, const char* name) {
    Entry* entry = new Entry{liveActor};
    entry->name = name;
    mBuffer.pushBack(entry);
}

void PlayerModelHolder::changeModel(const char* name) {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it) {
        if (al::isEqualString(it->name, sead::SafeString(name))) {
            mCurrentModel = &*it;
            return;
        }
    }
}

al::LiveActor* PlayerModelHolder::findModelActor(const char* name) const {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it)
        if (al::isEqualString(it->name, sead::SafeString(name)))
            return it->actor;
    return mCurrentModel->actor;
}

al::LiveActor* PlayerModelHolder::tryFindModelActor(const char* name) const {
    for (auto it = mBuffer.begin(), end = mBuffer.end(); it != end; ++it)
        if (al::isEqualString(it->name, sead::SafeString(name)))
            return it->actor;
    return nullptr;
}

bool PlayerModelHolder::isCurrentModelLabel(const char* name) const {
    return al::isEqualString(mCurrentModel->name.cstr(), name);
}

bool PlayerModelHolder::isCurrentModelLabelSubString(const char* name) const {
    return al::isEqualSubString(mCurrentModel->name.cstr(), name);
}
