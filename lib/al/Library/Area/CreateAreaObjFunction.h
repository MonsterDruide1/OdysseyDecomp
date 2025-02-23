#pragma once

namespace al {
class AreaObj;

template <typename T>
AreaObj* createAreaObjFunction(const char* areaObjName) {
    return new T(areaObjName);
}
}  // namespace al
