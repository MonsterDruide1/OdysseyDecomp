#pragma once

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
    void restartAction(bool);
    void startAction(const char*);
    void checkAndPlayStart(float, float, float, bool);
    void update(float, float, float, bool);
    void resetState();
};
}  // namespace al
