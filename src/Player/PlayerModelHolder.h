#pragma once

#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}

class PlayerModelHolder {
public:
    struct Entry {
        sead::FixedSafeString<128> mName;
        al::LiveActor* mLiveActor;
    };

    PlayerModelHolder(u32);
    void registerModel(al::LiveActor*, const char*);
    void changeModel(const char*);
    al::LiveActor* findModelActor(const char*) const;
    al::LiveActor* tryFindModelActor(const char*) const;
    bool isCurrentModelLabel(const char*) const;
    bool isCurrentModelLabelSubString(const char*) const;

public:
    sead::PtrArray<Entry> mBuffer;
    Entry* mCurrentModel = nullptr;
    sead::FixedSafeString<128> _10 = sead::FixedSafeString<128>("");
};
