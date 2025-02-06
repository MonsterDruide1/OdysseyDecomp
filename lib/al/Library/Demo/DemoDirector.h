#pragma once

namespace al {

class LiveActor;

class DemoDirector {
public:
    bool requestStartDemo(const char* name);
    bool requestEndDemo(const char* name);
    void addDemoActor(LiveActor* actor);
};
}  // namespace al
