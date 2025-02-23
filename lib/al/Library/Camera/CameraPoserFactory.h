#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class CameraPoser;
class CameraPoserEntrance;

using CameraPoserCreatorFunction = CameraPoser* (*)(const char* cameraPoserName);

class CameraPoserFactory : public Factory<CameraPoserCreatorFunction> {
public:
    CameraPoserFactory(const char* factoryName);

    CameraPoserEntrance* createEntranceCameraPoser() const;
};
}  // namespace al

namespace alCameraPoserFactoryFunction {
void initAndCreateTableFromOtherTable2(
    al::CameraPoserFactory* out, const al::NameToCreator<al::CameraPoserCreatorFunction>* table1,
    s32 table1Count, const al::NameToCreator<al::CameraPoserCreatorFunction>* table2,
    s32 table2Count);
void initAndCreateTableWithAnotherFactory(
    al::CameraPoserFactory* out, const al::CameraPoserFactory* factory,
    const al::NameToCreator<al::CameraPoserCreatorFunction>* table, s32 tableCount);
void initAndCreateTableWithPresetPosers(
    al::CameraPoserFactory* out, const al::NameToCreator<al::CameraPoserCreatorFunction>* table,
    s32 tableCount);
}  // namespace alCameraPoserFactoryFunction
