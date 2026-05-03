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

void initAndCreateTableWithAnotherFactory(
    al::CameraPoserFactory* factory, const al::CameraPoserFactory* otherFactory,
    const al::NameToCreator<al::CameraPoserCreatorFunction>* table, s32 tableCount) {
    s32 otherCount = otherFactory->getNumFactoryEntries();

    al::NameToCreator<al::CameraPoserCreatorFunction>* newTable =
        new al::NameToCreator<al::CameraPoserCreatorFunction>[otherCount + tableCount];

    const al::NameToCreator<al::CameraPoserCreatorFunction>* otherTable =
        otherFactory->getFactoryEntries();

    for (s32 i = 0; i < otherFactory->getNumFactoryEntries(); i++) {
        newTable[i].name = otherTable[i].name;
        newTable[i].creationFunction = otherTable[i].creationFunction;
    }

    for (s32 i = 0; i < tableCount; i++) {
        newTable[otherCount + i].name = table[i].name;
        newTable[otherCount + i].creationFunction = table[i].creationFunction;
    }

    factory->initFactory(newTable, otherCount + tableCount);
}

void initAndCreateTableWithPresetPosers(
    al::CameraPoserFactory* factory, const al::NameToCreator<al::CameraPoserCreatorFunction>* table,
    s32 tableCount) {
    al::SimpleCameraPoserFactory simpleFactory = {"シンプルカメラ生成"};
    initAndCreateTableWithAnotherFactory(factory, &simpleFactory, table, tableCount);
}
}  // namespace alCameraPoserFactoryFunction
