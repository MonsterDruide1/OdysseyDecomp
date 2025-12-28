#include "Util/TimeUtil.h"

namespace rs {
bool checkTimeReverseAndRestore(u64* a, u64 b) {
    if (b < *a + 10) {
        *a = b;
        return true;
    }
    return false;
}
}  // namespace rs
