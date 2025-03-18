#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}

class PlayerModelHolder {
public:
    struct Entry {
        Entry(al::LiveActor* actor) : actor(actor) {}

        sead::FixedSafeString<128> name;
        al::LiveActor* actor;
    };

    PlayerModelHolder(u32);
    void registerModel(al::LiveActor*, const char*);
    void changeModel(const char*);
    al::LiveActor* findModelActor(const char*) const;
    al::LiveActor* tryFindModelActor(const char*) const;
    bool isCurrentModelLabel(const char*) const;
    bool isCurrentModelLabelSubString(const char*) const;

private:
    sead::PtrArray<Entry> mBuffer;
    Entry* mCurrentModel = nullptr;
    sead::FixedSafeString<128> _10 = sead::FixedSafeString<128>("");
};
