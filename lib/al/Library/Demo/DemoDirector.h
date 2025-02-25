#pragma once

namespace al {

class LiveActor;

class DemoDirector {
public:
    bool requestStartDemo(const char* name);
    void requestEndDemo(const char* name);
    void addDemoActor(LiveActor* actor);
};
}  // namespace al
