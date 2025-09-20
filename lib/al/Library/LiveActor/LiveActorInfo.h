#pragma once

#include <basis/seadTypes.h>

// TODO: bring this up to sanae
// temp solution to figure out a solution to bitflag enums (stole this right from nvn headers)
#define AL_BITS(CLASS)                                                                             \
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
    bool isSet(const Enum& e) const {                                                              \
        return (m_value & e) != 0;                                                                 \
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
    CLASS& operator=(const CLASS& c) {                                                             \
        m_value = c.m_value;                                                                       \
        return *this;                                                                              \
    }                                                                                              \
    void set(const Enum& e) {                                                                      \
        m_value |= e;                                                                              \
    }                                                                                              \
    void unset(const Enum& e) {                                                                    \
        if (isSet(e))                                                                              \
            m_value &= ~e;                                                                         \
    }                                                                                              \
                                                                                                   \
    s32 m_value;

namespace al {
class LiveActor;
struct ActorInitInfo;

struct SubActorSync {
    enum Enum {
        cNone = 0,                                       // 0
        cAppear = 1 << 0,                                // 1
        cClipping = 1 << 1,                              // 2
        cHide = 1 << 2,                                  // 4
        cAlphaMask = 1 << 3,                             // 8
        cAll = cAppear | cClipping | cHide | cAlphaMask  // 15
    };

    AL_BITS(SubActorSync)
};

struct SubActorInfo {
public:
    SubActorInfo();

    SubActorInfo(LiveActor* actor, SubActorSync syncType) : subActor(actor), syncType(syncType) {}

    LiveActor* subActor = nullptr;
    void* field_8 = nullptr;
    SubActorSync syncType = SubActorSync::cNone;
};

}  // namespace al
