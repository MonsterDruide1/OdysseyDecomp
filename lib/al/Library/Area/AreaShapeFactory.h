#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class AreaShape;

using AreaShapeCreatorFunction = AreaShape* (*)();

class AreaShapeFactory : public Factory<AreaShapeCreatorFunction> {
public:
    AreaShapeFactory(const char* factoryName);
};
}  // namespace al
