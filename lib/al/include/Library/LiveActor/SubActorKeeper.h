#pragma once

#include <basis/seadTypes.h>
#include "Library/LiveActor/ActorInitInfo.h"

// TODO: bring this up to sanae
// temp solution to figure out a solution to bitflag enums (stole this right from nvn headers)
#define AL_BITS(CLASS)                                                                             \
public:                                                                                            \
    CLASS() : m_value(-1) {}                                                                       \
    CLASS(const CLASS& c) : m_value(c.m_value) {}                                                  \
    CLASS(s32 i) : m_value(i) {}                                                                   \
    CLASS(Enum e) : m_value(e) {}                                                                  \
    operator s32() const {                                                                         \
        return m_value;                                                                            \
    }                                                                                              \
    bool operator==(const CLASS& c) const {                                                        \
        return m_value == c.m_value;                                                               \
    }                                                                                              \
    bool operator==(const Enum& e) const {                                                         \
        return m_value == e;                                                                       \
    }                                                                                              \
    bool operator!=(const CLASS& c) const {                                                        \
        return m_value != c.m_value;                                                               \
    }                                                                                              \
    bool operator!=(const Enum& e) const {                                                         \
        return m_value != e;                                                                       \
    }                                                                                              \
    CLASS operator|(const CLASS& c) const {                                                        \
        return CLASS(m_value | c.m_value);                                                         \
    }                                                                                              \
    CLASS operator|(const Enum& e) const {                                                         \
        return CLASS(m_value | e);                                                                 \
    }                                                                                              \
    CLASS operator&(const CLASS& c) const {                                                        \
        return CLASS(m_value & c.m_value);                                                         \
    }                                                                                              \
    CLASS operator&(const Enum& e) const {                                                         \
        return CLASS(m_value & e);                                                                 \
    }                                                                                              \
    CLASS operator^(const CLASS& c) const {                                                        \
        return CLASS(m_value ^ c.m_value);                                                         \
    }                                                                                              \
    CLASS operator^(const Enum& e) const {                                                         \
        return CLASS(m_value ^ e);                                                                 \
    }                                                                                              \
    CLASS& operator|=(const CLASS& c) {                                                            \
        m_value |= c.m_value;                                                                      \
        return *this;                                                                              \
    }                                                                                              \
    CLASS& operator|=(const Enum& e) {                                                             \
        m_value |= e;                                                                              \
        return *this;                                                                              \
    }                                                                                              \
    CLASS& operator&=(const CLASS& c) {                                                            \
        m_value &= c.m_value;                                                                      \
        return *this;                                                                              \
    }                                                                                              \
    CLASS& operator&=(const Enum& e) {                                                             \
        m_value &= e;                                                                              \
        return *this;                                                                              \
    }                                                                                              \
    CLASS& operator^=(const CLASS& c) {                                                            \
        m_value ^= c.m_value;                                                                      \
        return *this;                                                                              \
    }                                                                                              \
    CLASS& operator^=(const Enum& e) {                                                             \
        m_value ^= e;                                                                              \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    s32 m_value;

namespace al {
class LiveActor;

struct SubActorSync {
    enum Enum {
        cNone = 0,                                       // 0
        cAppear = 1 << 0,                                // 1
        cClipping = 1 << 1,                              // 2
        cHide = 1 << 2,                                  // 4
        cAlphaMask = 1 << 3,                             // 8
        cAll = cAppear | cClipping | cHide | cAlphaMask  // 15
    };
    AL_BITS(SubActorSync);
};

class SubActorInfo {
public:
    SubActorInfo();
    SubActorInfo(LiveActor* actor, SubActorSync syncType) : mSubActor(actor), mSyncType(syncType){};

    LiveActor* mSubActor = nullptr;
    void* field_8 = nullptr;
    SubActorSync mSyncType = SubActorSync::cNone;
};

class SubActorKeeper {
    friend class alSubActorFunction;

public:
    SubActorKeeper(LiveActor*);
    void registerSubActor(LiveActor*, u32);
    void init(const ActorInitInfo&, const char*, s32);

    static SubActorKeeper* create(LiveActor*);
    static SubActorKeeper* tryCreate(LiveActor*, const char*, s32);

private:
    LiveActor* mRootActor;
    s32 mMaxActorCount = 0;
    s32 mCurActorCount = 0;
    SubActorInfo** mBuffer = nullptr;
};

void isExistSubActorKeeper(const LiveActor*);
const LiveActor* getSubActor(const LiveActor*, const char*);  // NOTE: unknown return type
void tryGetSubActor(const LiveActor*, const char*);
void getSubActor(const LiveActor*, s32);
void getSubActorNum(const LiveActor*);
void offSyncClippingSubActor(LiveActor*, const LiveActor*);
void offSyncClippingSubActor(LiveActor*, const char*);
void offSyncClippingSubActorAll(LiveActor*);
void onSyncClippingSubActor(LiveActor*, const LiveActor*);
void onSyncClippingSubActor(LiveActor*, const char*);
void onSyncClippingSubActorAll(LiveActor*);
void offSyncAppearSubActor(LiveActor*, const LiveActor*);
void offSyncAppearSubActor(LiveActor*, const char*);
void offSyncAppearSubActorAll(LiveActor*);
void onSyncAppearSubActor(LiveActor*, const LiveActor*);
void onSyncAppearSubActor(LiveActor*, const char*);
void onSyncAppearSubActorAll(LiveActor*);
void offSyncHideSubActor(LiveActor*, const LiveActor*);
void offSyncHideSubActor(LiveActor*, const char*);
void offSyncHideSubActorAll(LiveActor*);
void onSyncHideSubActor(LiveActor*, const LiveActor*);
void onSyncHideSubActor(LiveActor*, const char*);
void onSyncHideSubActorAll(LiveActor*);
void isSyncHideSubActor(const LiveActor*, const LiveActor*);
void offSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void offSyncAlphaMaskSubActor(LiveActor*, const char*);
void offSyncAlphaMaskSubActorAll(LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const char*);
void onSyncAlphaMaskSubActorAll(LiveActor*);
void isSyncAlphaMaskSubActor(const LiveActor*, const LiveActor*);
void startActionSubActor(const LiveActor*, const char*, const char*);
void isActionEndSubActor(const LiveActor*, const char*);
void isActionPlayingSubActor(const LiveActor*, const char*, const char*);
void isActionOneTimeSubActor(const LiveActor*, const char*);
void isActionOneTimeSubActor(const LiveActor*, const char*, const char*);
void tryStartActionSubActorAll(const LiveActor*, const char*);
void makeActorDeadSubActorAll(const LiveActor*);
void tryValidateCollisionPartsSubActorAll(LiveActor*);
void tryInvalidateCollisionPartsSubActorAll(LiveActor*);

}  // namespace al

namespace alSubActorFunction {
void trySyncAlive(al::SubActorKeeper*);
void trySyncDead(al::SubActorKeeper*);
void trySyncClippingStart(al::SubActorKeeper*);
void trySyncClippingEnd(al::SubActorKeeper*);
void trySyncShowModel(al::SubActorKeeper*);
void trySyncHideModel(al::SubActorKeeper*);
void trySyncModelAlphaMask(al::SubActorKeeper*, f32);
void findSubActor(const al::SubActorKeeper*, const char*);
}  // namespace alSubActorFunction
