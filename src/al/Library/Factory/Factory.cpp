#include "al/factory/Factory.h"

namespace al {
template <typename T>
const char* Factory<T>::convertName(const char* pName) const {
    return pName;
}

}  // namespace al
