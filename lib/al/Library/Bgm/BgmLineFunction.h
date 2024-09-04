#pragma once

namespace al {
class Resource;
class IUseAudioKeeper;

class BgmDataBase {
public:
    static BgmDataBase* create(const char*, const char*);

    BgmDataBase(const Resource*, const Resource*);
};

void startBgmSituation(const IUseAudioKeeper*, const char*, bool, bool);
}  // namespace al
