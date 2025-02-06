#pragma once

#include <basis/seadTypes.h>

namespace al {
class Resource;
class IUseAudioKeeper;

class BgmDataBase {
public:
    static BgmDataBase* create(const char*, const char*);

    BgmDataBase(const Resource*, const Resource*);
};

void startBgmSituation(const IUseAudioKeeper*, const char*, bool, bool);
void startBgm(const IUseAudioKeeper*, const char*, s32, s32);
void stopBgm(const IUseAudioKeeper*, const char*, s32);
void stopAllBgm(const IUseAudioKeeper*, s32);
bool isRunningBgm(const IUseAudioKeeper*, const char*);
}  // namespace al
