#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class GameDataFile;

class HintPhotoData : public ByamlSave {
public:
    HintPhotoData(const GameDataFile* game_data_file);
    void init();
    void saveHintPhoto(const char*);
    bool checkSavedHintPhoto(const char*) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding[0x3];
};

static_assert(sizeof(HintPhotoData) == 0x20);
