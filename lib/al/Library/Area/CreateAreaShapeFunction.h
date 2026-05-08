#pragma once

namespace al {
class AreaShape;

template <typename T>
AreaShape* createAreaShapeFunction() {
    return new T();
}
}  // namespace al
