#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class CameraPoser;
class CameraPoserEntrance;

using CameraPoserCreatorFunction = CameraPoser* (*)(const char* cameraPoserName);

class CameraPoserFactory : public Factory<CameraPoserCreatorFunction> {
public:
    CameraPoserFactory(const char* factoryName);

    virtual CameraPoserEntrance* createEntranceCameraPoser() const;
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

template <s32 N1, s32 N2>
inline void initAndCreateTableFromOtherTable(
    al::CameraPoserFactory* out,
    const al::NameToCreator<al::CameraPoserCreatorFunction> (&table1)[N1],
    const al::NameToCreator<al::CameraPoserCreatorFunction> (&table2)[N2]) {
    initAndCreateTableFromOtherTable2(out, table1, N1, table2, N2);
}
}  // namespace alCameraPoserFactoryFunction
