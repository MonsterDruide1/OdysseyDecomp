#include "al/include/Library/Area/AreaObj.h"

namespace al {
class TrafficArea : public AreaObj {
private:
    bool mCarFull = false;
    bool mNpcFull = false;
    bool mNpcUnavailable = false;
    char mUnkChar = '\0';

public:
    TrafficArea(const char* name);

    bool tryPermitEnterCar();
    bool tryPermitEnterNpc();
};
}  // namespace al
