#pragma once

namespace al {
class LiveActor;

class ScreenPointKeeper {
public:
    void update();
};
}  // namespace al

namespace alScreenPointFunction {
void updateScreenPointAll(al::LiveActor*);
}
