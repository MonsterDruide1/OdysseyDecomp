#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioKeeper;
class SeKeeper;
struct SeUserInfo;

class ActionSeCtrl {
public:
    static ActionSeCtrl* tryCreate(AudioKeeper*);

    ActionSeCtrl(SeKeeper*, const SeUserInfo*);

    void init();
    void resetAction(bool);
    void restartAction();
    void startAction(const char*);
    void checkAndPlayStart(f32, f32, f32, bool);
    void update(f32, f32, f32, bool);
    void resetState();
};
}  // namespace al
