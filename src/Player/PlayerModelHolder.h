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

    PlayerModelHolder(unsigned int);
    void registerModel(al::LiveActor*, char const*);
    void changeModel(char const*);
    al::LiveActor* findModelActor(char const*) const;
    al::LiveActor* tryFindModelActor(char const*) const;
    bool isCurrentModelLabel(char const*) const;
    bool isCurrentModelLabelSubString(char const*) const;

private:
    sead::PtrArray<Entry> mBuffer;
    Entry* currentModel = nullptr;
    sead::FixedSafeString<128> _10 = sead::FixedSafeString<128>("");
};
