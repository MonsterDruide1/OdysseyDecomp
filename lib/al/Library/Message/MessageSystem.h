#pragma once

#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>

namespace sead {
class MessageProject;
}

namespace al {
class MessageProjectEx;
class MessageHolder;

class MessageSystem {
    using MessageTreeMap = sead::StrTreeMap<256, MessageHolder*>;

public:
    MessageSystem();
    void initMessageForChangeLanguage();
    bool tryInitMessageHolder(MessageTreeMap*, const char*, const char*, const char*);

    sead::MessageProject* getMessageProject();
    MessageHolder* getMessageHolderCore(const char*, s32);
    MessageHolder* getMessageHolderCore(const char*, s32, const char*);
    MessageHolder* getSystemMessageHolder(const char*);
    MessageHolder* getSystemMessageHolder(const char*, const char*);
    MessageHolder* getLayoutMessageHolder(const char*);
    MessageHolder* getStageMessageHolder(const char*);

private:
    MessageProjectEx* mMessageProject;
    sead::PtrArray<MessageTreeMap> mTreeMaps;
    sead::Heap* mMessageHeap;
};
}  // namespace al
