#pragma once

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

    PlayerModelHolder(u32 bufferSize);
    void registerModel(al::LiveActor* actor, const char* name);
    void changeModel(const char* name);
    al::LiveActor* findModelActor(const char* name) const;
    al::LiveActor* tryFindModelActor(const char* name) const;
    bool isCurrentModelLabel(const char* name) const;
    bool isCurrentModelLabelSubString(const char* name) const;

private:
    sead::PtrArray<Entry> mBuffer;
    Entry* mCurrentModel = nullptr;
    sead::FixedSafeString<128> _10 = sead::FixedSafeString<128>("");
};
