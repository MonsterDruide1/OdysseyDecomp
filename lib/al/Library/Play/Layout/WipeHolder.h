#pragma once

namespace al {
class WipeSimple;

class WipeHolder {
public:
    WipeHolder(s32 maxWipes);
    void registerWipe(const char* name, WipeSimple* wipe);
    void startClose(const char* name, s32 frames = -1);
    s32 findWipe(const char* name) const;
    void startCloseByInfo(const char* infoName);
    s32 findInfo(const char* infoName) const;
    void tryStartClose(const char* name, s32 frames = -1);
    void tryStartCloseByInfo(const char* infoName);
    void startCloseEnd(const char* infoName);
    void startOpen(s32 frames = -1);
    bool isExistInfo(const char* infoName) const;
    s32 tryFindInfo(const char* infoName) const;
    s32 getCloseTimeByInfo(const char* infoName) const;
    bool isCloseEnd() const;
    bool isOpenEnd() const;
    bool isCloseWipe(const char* name) const;

private:
    s32 mMaxWipeCount;
    s32 mWipeCount;
    void* _8;
    WipeSimple** mWipes;
    void* filler[4];
};
}  // namespace al

static_assert(sizeof(al::WipeHolder) == 0x38, "al::WipeHolder size");
