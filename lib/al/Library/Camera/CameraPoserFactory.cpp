#include "Library/Camera/CameraPoserFactory.h"

#include "Library/Play/Camera/CameraPoserEntrance.h"
#include "Library/Play/Camera/SimpleCameraPoserFactory.h"

namespace al {
CameraPoserFactory::CameraPoserFactory(const char* factoryName) : Factory(factoryName) {}

CameraPoserEntrance* CameraPoserFactory::createEntranceCameraPoser() const {
    return new CameraPoserEntrance("スタート");
}
}  // namespace al

namespace alCameraPoserFactoryFunction {
void initAndCreateTableFromOtherTable2(
    al::CameraPoserFactory* factory,
    const al::NameToCreator<al::CameraPoserCreatorFunction>* table1, s32 table1Count,
    const al::NameToCreator<al::CameraPoserCreatorFunction>* table2, s32 table2Count) {
    s32 tableCount = table1Count + table2Count;
    al::NameToCreator<al::CameraPoserCreatorFunction>* table =
        new al::NameToCreator<al::CameraPoserCreatorFunction>[tableCount];

    for (s32 i = 0; i < table1Count; i++) {
        table[i].name = table1[i].name;
        table[i].creationFunction = table1[i].creationFunction;
    }

    for (s32 i = 0; i < table2Count; i++) {
        table[table1Count + i].name = table2[i].name;
        table[table1Count + i].creationFunction = table2[i].creationFunction;
    }

    factory->initFactory(table, tableCount);
}

// NON_MATCHING
void initAndCreateTableWithAnotherFactory(
    al::CameraPoserFactory* factory, const al::CameraPoserFactory* otherFactory,
    const al::NameToCreator<al::CameraPoserCreatorFunction>* table, s32 tableCount) {
    initAndCreateTableFromOtherTable2(factory, otherFactory->getFactoryEntries(),
                                      otherFactory->getNumFactoryEntries(), table, tableCount);
}

void initAndCreateTableWithPresetPosers(
    al::CameraPoserFactory* factory, const al::NameToCreator<al::CameraPoserCreatorFunction>* table,
    s32 tableCount) {
    al::SimpleCameraPoserFactory simpleFactory = {"シンプルカメラ生成"};
    initAndCreateTableWithAnotherFactory(factory, &simpleFactory, table, tableCount);
}
}  // namespace alCameraPoserFactoryFunction
