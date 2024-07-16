#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
class LayoutInitInfo;

class SimpleAudioUser : public IUseAudioKeeper {
public:
    SimpleAudioUser(const char* name, AudioDirector* director);
    SimpleAudioUser(const char* name, const ActorInitInfo& info);
    SimpleAudioUser(const char* name, const LayoutInitInfo& info);

    AudioKeeper* getAudioKeeper() const override;

private:
    u8 padding[0x10];
};
}  // namespace al
