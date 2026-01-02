#include "Util/TimeUtil.h"

namespace rs {
bool checkTimeReverseAndRestore(u64* startTime, u64 currentTime) {
    if (currentTime < *startTime + 10) {
        *startTime = currentTime;
        return true;
    }
    return false;
}
}  // namespace rs
