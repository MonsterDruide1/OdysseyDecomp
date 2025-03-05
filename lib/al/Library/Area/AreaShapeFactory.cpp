#include "Library/Area/AreaShapeFactory.h"

#include "Library/Area/AreaShapes.h"
#include "Library/Area/CreateAreaShapeFunction.h"

namespace al {
const NameToCreator<AreaShapeCreatorFunction> sAreaShapeFactoryEntries[] = {
    {"AreaCubeBase", createAreaShapeFunction<AreaShapeCubeBase>},
    {"AreaCubeCenter", createAreaShapeFunction<AreaShapeCubeCenter>},
    {"AreaCubeTop", createAreaShapeFunction<AreaShapeCubeTop>},
    {"AreaSphere", createAreaShapeFunction<AreaShapeOval>},
    {"AreaCylinder", createAreaShapeFunction<AreaShapeCylinderBase>},
    {"AreaCylinderCenter", createAreaShapeFunction<AreaShapeCylinderCenter>},
    {"AreaCylinderTop", createAreaShapeFunction<AreaShapeCylinderTop>},
    {"AreaInfinite", createAreaShapeFunction<AreaShapeInfinite>}};

AreaShapeFactory::AreaShapeFactory(const char* factoryName) : Factory(factoryName) {
    initFactory(sAreaShapeFactoryEntries);
}
}  // namespace al
