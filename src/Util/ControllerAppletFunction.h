#pragma once

namespace al {
class GamePadSystem;
}

class ControllerAppletFunction {
public:
    void connectControllerSinglePlay(al::GamePadSystem*);
    void connectControllerSeparatePlay(al::GamePadSystem*);
};
